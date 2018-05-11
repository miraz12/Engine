#pragma once
#include "config.h"
#include "core/math/vector3D.h"
#include "core/math/matrix4D.h"

class Camera
{
    
public:

    Camera(float width, float height);

    float m_width, m_height;

    float camSpeed = 1.0f;//100.f;

    float64 posX = 0.0f;
    float64 posY = 0.0f;
    float64 oldPosX = 0.0f;
    float64 oldPosY = 0.0f;
    float64 x;
    float64 y;
    float64 z = 0.0f;
    float64 scrollX = 0.0f;

    //Move cam
    vector3D position;
    vector3D origin;
    vector3D headUp;
    vector3D camFront;
    bool firstM = true;
    float pitch;
    float yaw;
    //---

    float timeStep = 1.0f / 16.0f;

    bool mousePressed = false;
    bool mousePressedLeft = false;

    matrix4D projection;
    matrix4D view;

private:




};
