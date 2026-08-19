#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"

using namespace sf;

int main()
{
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
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background_sheet.png");

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
                                arena.size.x = 500;
                                arena.size.y = 500;
                                arena.position.x = 0;
                                arena.position.y = 0;

                                // Pass the vertex array by reference
                                // to the createBackground function
                                int tileSize =createBackground(background, arena);

                                // We will modify this line of code later
                                // int tileSize = 50;

                                // Spawn the player in middle of the arena
                                player.spawn(arena, resolution, tileSize);

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

            // Update the player
            // player.update(dtAsSeconds, mouseWorldPosition);
            player.update(dtAsSeconds, Mouse::getPosition());

            // Make a note of the players new position
            Vector2f playerPosition(player.getCenter());

            // Make the view centre
            // the around player
            mainView.setCenter(player.getCenter());

        } // End updating the scene

        /*
        **************
        Draw the scene
        **************
        */

        window.clear();

        if (state == State::PLAYING)
        {
            // set the mainView to be displayed in the window
            // And draw everything related to it
            window.setView(mainView);

            // Draw the background
            window.draw(background, &textureBackground);

            // Draw the player
            window.draw(player.getSprite());
        }

        if (state == State::LEVELING_UP)
        {
        }

        if (state == State::PAUSED)
        {
        }

        if (state == State::GAME_OVER)
        {
        }

        window.display();

    } // End game Loop

    return 0;
} 