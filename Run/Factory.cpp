#include "Factory.h"
#include <iostream>

using namespace std;

Factory::Factory(RenderWindow* window)
{
    m_Window = window;
    m_Texture = new Texture();
    if (!m_Texture->loadFromFIle("graphics/texture.png"))
    {
        cout << "TExture not loaded";
        return;
    }
}

void Factory::loadLevel(vetor<GameObject>& gameObjects,
    VertexArray& canvas, InputDispatcher& inputDispatcher)
{
}