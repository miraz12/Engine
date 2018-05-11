#pragma once
#include "render/window.h"
#include "render/camera.h"

class KeyHandler
{

public:
    KeyHandler();
    void Init(Display::Window* window, Camera* camera);


private:

    Camera * cam;

    float rad(float d);


};



