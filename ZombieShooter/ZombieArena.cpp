#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"


using namespace sf;
using namespace std;

int main()
{

    // Here is the instance of TextureHolder
    TextureHolder holder;

    // The game will always be in one of four states
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

    // Start with the GAME_OVER state
    State state = State::GAME_OVER;

    // Get the screen resolution and create an SFML window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().size.x;
    resolution.y = VideoMode::getDesktopMode().size.y;

    RenderWindow window(VideoMode(VideoMode::getDesktopMode().size),
                        "Zombie Arena", sf::State::Fullscreen);

    // Create a SFML view for the main action
    View mainView({0, 0}, {resolution.x, resolution.y});

    // Here is our clock for timing everything
    Clock clock;

    // How long has the PLAYING state been active
    Time gameTimeTotal;

    // Where is the mouse in relation to world coordinates
    Vector2f mouseWorldPosition;

    // Where is the mouse in relation to screen coordinates
    Vector2i mouseScreenPosition;

    // Create an instance of Player class
    Player player;

    // The boundaries of the arena
    IntRect arena;

    // Create the background
    VertexArray background;
    // Load the texture for our background vertex array
    Texture textureBackground = TextureHolder::GetTexture(
        "graphics/background_sheet.png");

    // Prepare for a horde of zombies
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies = nullptr;

    // 100 bullets should do
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    // When was the fire button lasts pressed?
    Time lastPressed;
    // Hide the mouse pointer and replace it with crosshair
    window.setMouseCursorVisible(true);
    Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    Sprite spriteCrosshair(textureCrosshair);
    spriteCrosshair.setOrigin({25, 25});

    // Create a couple of pickups
    Pickup healthPickup(1);
    Pickup ammoPickup(2);

    // About the game
    int score = 0;
    int hiScore = 0; 

    // For the home/game over screen
    Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
    Sprite spriteGameOver(textureGameOver);
    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition({0, 0});

    // Create a view for the HUD
    View hudView(sf::FloatRect({0, 0}, {1920, 1080}));

    // Create a sprite for the ammo icon
    Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
    Sprite spriteAmmoIcon(textureAmmoIcon);
    spriteAmmoIcon.setTexture(textureAmmoIcon);
    spriteAmmoIcon.setPosition({20, 980});

    // Load the font
    Font font;
    font.openFromFile("fonts/zombiecontrol.ttf");

    // Paused
    Text pausedText(font);
    pausedText.setCharacterSize(155);
    pausedText.setFillColor(Color::White);
    pausedText.setPosition({400, 400});
    pausedText.setString("Press Enter \nto continue");

    // Game Over
    Text gameOverText(font);
    gameOverText.setCharacterSize(125);
    gameOverText.setFillColor(Color::White);
    gameOverText.setPosition({250, 860});
    gameOverText.setString("Press Enter to play");

    // LEVELING UP
    Text levelUpText(font);
    levelUpText.setCharacterSize(80);
    levelUpText.setFillColor(Color::White);
    levelUpText.setPosition({150, 250});
    std::stringstream levelUpStream;
   levelUpStream <<
    "1- Increased rate of fire" <<
    "\n2- Increased clip size(next reaload)" <<
    "\n3- Increased max health" <<
    "\n4 Increased run speed" <<
    "\n5 More and better health pickups" <<
    "\n6 More and better ammo pickups";
    levelUpText.setString(levelUpStream.str());

    // Ammo
    Text ammoText(font);
    ammoText.setCharacterSize(55);
    ammoText.setFillColor(Color::White);
    ammoText.setPosition({200, 980});

    // Score
    Text scoreText(font);
    scoreText.setCharacterSize(55);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition({20 ,0});
    
    
    // Load the high score form a text file
    std::ifstream inputFile("gamedata/scores.txt");
    if (inputFile.is_open())
    {
        // >> Reads the data
        inputFile >> hiScore;
        inputFile.close();
    }

    // Hi score
    Text hiScoreText(font);
    hiScoreText.setCharacterSize(55);
    hiScoreText.setFillColor(Color::White);
    hiScoreText.setPosition({1400, 0});
    std::stringstream s;
    s << "Hi score:" << hiScore;
    hiScoreText.setString(s.str());



    // Zombies remaining
    Text zombiesRemainingText(font);

    zombiesRemainingText.setCharacterSize(55);
    zombiesRemainingText.setFillColor(Color::White);
    zombiesRemainingText.setPosition({1500, 980});
    zombiesRemainingText.setString("Zombies: 100");

    // Wave number
    int wave = 10;
    Text waveNumberText(font);
    waveNumberText.setCharacterSize(55);
    waveNumberText.setFillColor(Color::White);
    waveNumberText.setPosition({1250, 980});
    waveNumberText.setString("Wave: 0");

    // Health bar
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition({450, 980});

    // When did we last update the HUD?
    int framesSinceLastHUDUpdate = 0;
    // How often (in frames) should we update the HUD
    int fpsMeasurementFrameInterval = 1000;
    

    // The main game loop
    while (window.isOpen())
    {
        /*
        ************
        Handle input
        ************
        */

        // Handle events by polling
        while (const std::optional event = window.pollEvent())
        {
            if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
            {
                // Pause a game while playing
                if (Keyboard::isKeyPressed(Keyboard::Key::Enter) &&
                    state == State::PLAYING)
                {
                    state = State::PAUSED;
                }

                // Restart while paused
                else if (Keyboard::isKeyPressed(Keyboard::Key::Enter) &&
                         state == State::PAUSED)
                {
                    state = State::PLAYING;

                    // Reset the clock so there isnt a frame jump
                    clock.restart();
                }

                // Start a new game while in GAME_OVER state
                else if (Keyboard::isKeyPressed(Keyboard::Key::Enter) &&
                         state == State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }

                if (state == State::PLAYING)
                {
                    //Reloading
                    if (keyPressed-> code == Keyboard::Key::R)
                    {
                        if (bulletsSpare >= clipSize)
                        {
                            // Plenty of bullets. Reload.
                            bulletsInClip = clipSize;
                            bulletsSpare -= clipSize;
                        }
                        else if (bulletsSpare > 0)
                        {
                            // Only few bullets left
                            bulletsInClip = bulletsSpare;
                            bulletsSpare = 0;
                        }
                        else
                        {
                            // More here soon?!
                        }
                    }
                }

                // Handle the player LEVELING up
                if (state == State::LEVELING_UP)
                {
                    if (keyPressed->code == Keyboard::Key::Num1)
                    {
                        state = State::PLAYING;
                         if (state == State::PLAYING)
                            {
                                // Prepare the level
                                // We will modify the next two lines later
                                arena.size.x = 1250;
                                arena.size.y = 1250;
                                arena.position.x = 0;
                                arena.position.y = 0;

                                // Pass the vertex array by reference
                                // to the createBackground function
                                int tileSize =createBackground(background, arena);

                                // We will modify this line of code later
                                // int tileSize = 50;

                                // Spawn the player in middle of the arena
                                player.resetPlayerStats();

                                bulletsInClip = clipSize;
                                bulletsSpare = 24;
                                
                                player.spawn(arena, resolution, tileSize);

                                // Configure the pick-ups
                                healthPickup.setArena(arena);
                                ammoPickup.setArena(arena);

                                // Create horde of zombies
                                numZombies = 6;
                                // Delete the previously allocated memory (if it exists)
                                delete[] zombies;
                                zombies = createHorde(numZombies, arena);
                                numZombiesAlive = numZombies;

                                // Reset clock so there isnt a frame jump
                                clock.restart();
                            } // End LEVELING up
                    }

                    if (keyPressed->code == Keyboard::Key::Num2)
                    {
                        state = State::PLAYING;
                    }

                    if (keyPressed->code == Keyboard::Key::Num3)
                    {
                        state = State::PLAYING;
                    }

                    if (keyPressed->code == Keyboard::Key::Num4)
                    {
                        state = State::PLAYING;
                    }

                    if (keyPressed->code == Keyboard::Key::Num5)
                    {
                        state = State::PLAYING;
                    }

                    if (keyPressed->code == Keyboard::Key::Num6)
                    {
                        state = State::PLAYING;
                    }
                }
            }
        } // End the event polling

        // Handle the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }

        // Handle WASD while playing
        if (state == State::PLAYING)
        {
            // Handle the pressing and releasing of WASD keys
            if (Keyboard::isKeyPressed(Keyboard::Key::W))
            {
                player.moveUp();
            }
            else
            {
                player.stopUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::S))
            {
                player.moveDown();
            }
            else
            {
                player.stopDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::A))
            {
                player.moveLeft();
            }
            else
            {
                player.stopLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::D))
            {
                player.moveRight();
            }
            else
            {
                player.stopRight();
            }

            // Fire a bullet
            if (Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                if (gameTimeTotal.asMilliseconds()
                    - lastPressed.asMilliseconds()
                    > 1000 / fireRate && bulletsInClip > 0)
                {
                    // Pass the centre of the player
                    // and the centre of the cross-hair
                    // to the shoot function
                    bullets[currentBullet].shoot(player.getCenter().x,
                        player.getCenter().y,mouseWorldPosition.x, mouseWorldPosition.y);
                    currentBullet++;
                    if (currentBullet > 99)
                    {
                        currentBullet = 0;
                    }
                    lastPressed = gameTimeTotal;
                    bulletsInClip--;
                }
            }// End fire a bullet
        


        } // End WASD while playing

        // Handle the LEVELING up state
        // if (state == State::PLAYING)
        // {
        //     // Prepare the level
        //     // We will modify the next two lines later
        //     arena.size.x = 500;
        //     arena.size.y = 500;
        //     arena.position.x = 0;
        //     arena.position.y = 0;

        //     // We will modify this line of code later
        //     int tileSize = 50;

        //     // Spawn the player in middle of the arena
        //     player.spawn(arena, resolution, tileSize);

        //     // Reset clock so there isnt a frame jump
        //     clock.restart();
        // } // End LEVELING up

        /*
        ***************
        UPDATE THE FRAME
        ***************
        */

        if (state == State::PLAYING)
        {
            // Update the delta time
            Time dt = clock.restart();

            // Update the total game time
            gameTimeTotal += dt;

            // Make a fraction of 1 from the delta time
            float dtAsSeconds = dt.asSeconds();

            // Where is the mouse pointer
            mouseScreenPosition = Mouse::getPosition();

            // Convert mouse position to world
            // based coordinates of mainView
            mouseWorldPosition = window.mapPixelToCoords(
                Mouse::getPosition(),
                mainView
            );

            // Set the crosshair to the mouse world Location
            spriteCrosshair.setPosition(mouseWorldPosition);

            // Update the player
            // player.update(dtAsSeconds, mouseWorldPosition);
            player.update(dtAsSeconds, Mouse::getPosition());

            // Make a note of the players new position
            Vector2f playerPosition(player.getCenter());

            // Make the view centre
            // the around player
            mainView.setCenter(player.getCenter());

            // Loop through each Zombie and update them
            for (int i = 0; i < numZombies; i++)
            {
                if (zombies[i].isAlive())
                {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }

            // Update any bullets that are in-flight
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
            }

            // update the pickups
            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);

            // Collision detection
            // Have any zombies been shot?
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() &&
                        zombies[j].isAlive())
                    {
                        if (bullets[i].getPosition().findIntersection
                            (zombies[j].getPosition()))
                        {
                            // Stop the bullet
                            bullets[i].stop();
                            // Register the hit and see if it was a kill
                            if (zombies[j].hit())
                            {
                                // Not just a hit but a kill too
                                score += 10;
                                if (score >= hiScore)
                                {
                                    hiScore = score;
                                }
                                numZombiesAlive--;
                                // When all the zombies are dead (again)
                                if (numZombiesAlive == 0) {
                                    state = State::LEVELING_UP;
                                }
                            }
                        }
                    }
                }
            } // End zombie being shot

            // Have any zombies touched the player
            for (int i = 0; i < numZombies; i++)
            {
                if (player.getPosition().findIntersection(
                    zombies[i].getPosition()) && zombies[i].isAlive())
                {
                    if (player.hit(gameTimeTotal))
                    {
                        // More here later
                    }
                    if (player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;
                        score = 0;
                        std::ofstream outputFile("gamedata/scores.txt");
                        // << writes the data
                        outputFile << hiScore;
                        outputFile.close();
                    }
                }
            }// End player touched

            // Has the player touched health pickup
            if (player.getPosition().findIntersection(healthPickup.getPosition())
                 && healthPickup.isSpawned())
            {
                player.increaseHealthLevel(healthPickup.gotIt());
            }

            // Has the player touched ammo pickup
            if (player.getPosition().findIntersection(ammoPickup.getPosition())
                 && ammoPickup.isSpawned())
            {
                bulletsSpare += ammoPickup.gotIt();
            }

            // size up the health bar
            healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
            // Increment then umber of frames since the previous update
            framesSinceLastHUDUpdate++;
            // re-calculate every fpsMeasurementFrameinterval frames
            if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
            {
                // Update game HUD text
                stringstream ssAmmo;
                stringstream ssScore;
                stringstream ssHiScore;
                stringstream ssWave;
                stringstream ssZombiesAlive;
                // Update the ammo text
                ssAmmo << bulletsInClip << "/" << bulletsSpare;
                ammoText.setString(ssAmmo.str());
                // Update the score text
                ssScore << "Score:" << score;
                scoreText.setString(ssScore.str());
                // Update the high score text
                ssHiScore << "Hi Score:" << hiScore;
                hiScoreText.setString(ssHiScore.str());
                // Update the wave
                ssWave << "Wave:" << wave;
                waveNumberText.setString(ssWave.str());
                // Update the high score text
                ssZombiesAlive << "Zombies:" << numZombiesAlive;
                zombiesRemainingText.setString(ssZombiesAlive.str());
                framesSinceLastHUDUpdate = 0;
            }// End HUD update

        } // End updating the scene

        /*
        **************
        Draw the scene
        **************
        */

        window.clear();

        if (state == State::PLAYING || state == State::PAUSED)
        {
            // set the mainView to be displayed in the window
            // And draw everything related to it
            window.setView(mainView);

            // Draw the background
            window.draw(background, &textureBackground);

            // Draw the zombies
            for (int i = 0; i < numZombies; i++)
            {
                window.draw(zombies[i].getSprite());
            }

            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }

            // Draw the player
            window.draw(player.getSprite());

            // Draw the pick-ups, if currently spawned
            if (ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }

            if (healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }

            // Draw the crosshair
            window.draw(spriteCrosshair);

            // Switch the HUD view
            window.setView(hudView);
            // Draw all the HUD elements
            window.draw(spriteAmmoIcon);
            window.draw(ammoText);
            window.draw(scoreText);
            window.draw(hiScoreText);
            window.draw(healthBar);
            window.draw(waveNumberText);
            window.draw(zombiesRemainingText);
        }

        if (state == State::LEVELING_UP)
        {
            window.setView(hudView);

            window.draw(spriteGameOver);
            window.draw(levelUpText);
        }

        if (state == State::PAUSED)
        {
            window.draw(pausedText);
        }

        if (state == State::GAME_OVER)
        {
            window.setView(hudView);
            window.draw(spriteGameOver);
            window.draw(gameOverText);
            window.draw(scoreText);
            window.draw(hiScoreText);
        }

        window.display();

    } // End game Loop

    // Delete the previously allocated memory (if it exists)
    delete[] zombies;

    return 0;
} 