#pragma once
#include "render/window.h"


namespace Input
{
    class KeyHandler
    {

    public:
        KeyHandler();
        void Init(Display::Window* window);


    private:

        float rad(float d);


    };
}


