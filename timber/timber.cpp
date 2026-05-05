// Include important libraries here
#include <SFML/Graphics.hpp>
#include <optional> 

// Make code easier to type with "using namespace" using namespace sf;
using namespace sf;

// This is where our game starts from int main()
int main ()
{
    // Create a video mode object VideoMode vm(1920, 1080);
    VideoMode vm({1920,1080});
    
    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", State::Fullscreen);
    
    //create a texture to save a graphic in the GPU
    Texture textureBackground;

    //Charge a graphic in the texture
    textureBackground.loadFromFile("graphics/background.png");

    //Create a sprite, Attach the texture to the sprite
    Sprite spriteBackground(textureBackground);
    

    // Set the spritebackground to cover the screen
    spriteBackground.setPosition({0,0});

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
                window.close();
        }

        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
           window.close();
        }
        
        /*
        ****************************************
        Update the scene
        ****************************************
        */
       
       
       
       /*
       ****************************************
       Draw the scene
       ****************************************
       */

        
        
       // Clear everything from the last frame
       window.clear();
       
       // Draw our game scene here
       window.draw(spriteBackground);
       
       // Show everything we just drew
       window.display();

    }
    return 0; 
}

