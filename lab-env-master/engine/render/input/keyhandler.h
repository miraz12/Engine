#pragma once
#include "render/window.h"


namespace Input
{
    class KeyHandler
    {

    public:
        KeyHandler();
        void Init(Display::Window* window);
        static KeyHandler* GetInstance();
        KeyHandler(const KeyHandler&) = delete;
        void operator=(const KeyHandler) = delete;

        bool W = false;
        bool S = false;
        bool A = false;
        bool D = false;
        bool Z = false;
        bool Y = false;

        bool leftShift = false;


    private:

        float rad(float d);
        static KeyHandler* instance;


    };
}


