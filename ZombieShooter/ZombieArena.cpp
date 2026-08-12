#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main()
{
    // The game will always be in one of for states
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING};

    // Start with the GAME_OVER state
    State state = State::GAME_OVER;

    // Get the screen resolution and create an SFML window
    Vector2f resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    RenderWindow window(VideoMode(resolution.x, resolution.y),
                        "Zombie Arena", State::Fullscreen);

    // Create a SFML view for the main action
    View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

    // Here is our clock for timing everything
    Clock clock;
    
    // How long has the PLAYING state been active
    Time gameTimeTotal;

    // Where is the mouse in
    // relation to world coordinates
    Vector2f mouseWorldPosition;

    // Where is the mouse in
    // relation to screen coordinates
    Vector2i mouseScreenPosition;

    // Create an instance of Player class
    Player player;

    // The boundaries of the arena
    IntRect arena;

    // The main game loop
    while (window.isOpen())
    {
        /*
        ************
        Handle input
        ************
        */
        // Handle events by polling
        while (window.pollEvent(Event))
        {
            if (event.type == Event::KeyPressed)
            {
                // Pause a game while playing
                if (event.key.code == Keyboard::Key::Enter &&
                    state == State::PLAYING)
                {
                    state = State::PAUSED;
                }
                // Restart while paused
                else if(event.key.code == Keyboard::Key::Enter &&
                    state == State::PAUSED)
                {
                    state = State::PLAYING;
                    // Reset the clock so there isnt a frame jump
                    clock.restart();
                }  
                // Start a new game while in GAME_OVER state
                else if (event.key.code == Keyboard::Key::Enter &&
                    state = State::GAME_OVER)
                {
                    state = State::LEVELING_UP;
                }
                if (state == State::PLAYING)
                {
                }
            }
        }// End the event polling
        // Handle the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        // Handle WASD while playing
        if (state == State::PLAYING)
        {
            // Handle the pressing and relasing of WASD keys
            if (Keyboard::isKeyPressed(Keyboard::Key::W))
            {
                play.moveUp();
            }
            else
            {
                player.stopUp();
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::S))
            {
                play.moveDown();
            }
            else
            {
                play.stopDown();
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
        }// End WASD while playing
        // Handle the LEVELING up state
        if (state == State::LEVELING_UP)
        {
            // Handle the player LEVELING up
            if (event.key.code == Keyboard::Key::Num1)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Key::Num2)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Key::Num3)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Key::Num4)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Key::Num5)
            {
                state = State::PLAYING;
            }
            if (event.key.code == Keyboard::Key::Num6)
            {
                state = State::PLAYING;
            }
            if (state == State::PLAYING)
            {
                // Prepare the level
                // We will modify the next two lines later
                arena.size.x = 500;
                arena.size.y = 500;
                arena.position.x = 0;
                arena.position.y = 0;
                // We will modify this line of code later
                int tileSize = 50;
                // Spawn the player in middle of the arena
                player.spawn(arena, resolution, tileSize);

                // Reset clock so there isnt a frame jump
                clock.restart();
            }
        }// End LEVELING up
    }// End game Loop

    
    return 0;
}