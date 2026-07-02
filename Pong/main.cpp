#include "Bat.h"
#include "Ball.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    // Create a video mode object
    VideoMode vm({1920, 1080});

    // Create and open a window for the game
    RenderWindow window(vm, "pong", State::Fullscreen);
    
    int score = 0;
    int lives = 3;

    // Create a bat a the bottom center of the screen
    Bat bat(1920 / 2, 1080 -20);
    // Create a ball
    Ball ball(1920 / 2,0);
    // We will add a ball in the next chapter
    // A cool retry-style font
    Font font;
    if (!font.openFromFile("fonts/DS-DIGIT.ttf"))
    {
        std::cout << "No se pudo cargar la fuente\n";
        return 1;
    }
     // Create a Text object called HUD
    Text hud(font);
    // Make it nice and big
    hud.setCharacterSize(75);
    // Choose a color
    hud.setFillColor(Color::White);
    hud.setPosition({20,20});
    // Here is our clock for timing everything
    Clock clock;
    while (window.isOpen())
    {
        /*
        Handle the player input
        *************************
        *************************
        *************************
        */

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();

        }

        // Handel the player quitting
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        // Handle the pressing and releasing of the arrow keys
        if (Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            bat.moveLeft();
        }
        else
        {
            bat.stopLeft();
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            bat.moveRight();
        }
        else
        {
            bat.stopRight();
        }
        /*
        Update the bat, the ball and the HUD
        *************************
        *************************
        *************************
        */
       Time dt = clock.restart();
       bat.update(dt);
       ball.update(dt);
       // Update the HUD text
       std::stringstream ss;
       ss << "score:" << score << " lives:" << lives;
       hud.setString(ss.str());

        /*
        Draw the bat, the ball and the HUD
        *************************
        *************************
        *************************
        */

        window.clear();
        window.draw(hud);
        window.draw(bat.getShape());
        window.draw(ball.getShape());
        window.display();


    }
    return 0;
}