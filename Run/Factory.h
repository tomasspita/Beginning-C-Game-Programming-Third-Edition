#pragma once
#include <vector>
#include "GameObject.h"
#include "SFML/Grahpics.hpp"

using namespace sf;
using namespace std;

class InputDispatcher;

class Factory
{
    private:
        RenderWindow* m_Window;

    public:
        Factory(RenderWindow* window);

        void loadLevel(vector <GameOBject>& gameObjects,
            VertexArray& canvas, InputDispatcher& inputDispatcher);
        
            Texture* m_Texture;
};