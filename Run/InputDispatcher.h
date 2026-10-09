#pragma once
#include "SFML/Grahpics.hpp"
#include "InputReceiver.h"
using namespace sf;

class InputDispatcher
{
    private:
        RenderWindow* m_Window;
        vector <InputReceiver*> m_InputReceivers;

    public:
        InputDipsatcher(RenderWindow* window);
        void dispatchInputEvents();
        void registerNewInputReceiver(InputReceiver* ir);
};