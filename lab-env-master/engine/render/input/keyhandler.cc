#include "config.h"
#include "keyhandler.h"
#include "core/math/matrix3D.h"

KeyHandler::KeyHandler()
{

}


void KeyHandler::Init(Display::Window* window)
{
    Camera* cam = Camera::GetInstance();

    window->SetKeyPressFunction([this, window, cam](int32 key, int32, int32 action, int32)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            window->Close();
        }
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        {
            cam->camSpeed = 100.f;
        }
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        {
            cam->camSpeed = 2.5f;
        }
        if (GLFW_KEY_1 == key && action == GLFW_RELEASE)
        {
        }
        if (GLFW_KEY_G == key)
        {

        }
        if (GLFW_KEY_R == key)
        {
            //glfwSetInputMode(this->window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            //mouseLocked = false;
        }
        if (GLFW_KEY_W == key)
        {
            cam->position[0] -= (cam->camFront * cam->camSpeed * cam->timeStep)[0];
            cam->position[1] -= (cam->camFront * cam->camSpeed * cam->timeStep)[1];
            cam->position[2] -= (cam->camFront * cam->camSpeed * cam->timeStep)[2];
        }
        if (GLFW_KEY_A == key)
        {
            cam->position[0] -= (cam->camFront.cross(cam->headUp).normalizeRe() * cam->camSpeed * cam->timeStep)[0];
            cam->position[1] -= (cam->camFront.cross(cam->headUp).normalizeRe() * cam->camSpeed * cam->timeStep)[1];
            cam->position[2] -= ((cam->camFront.cross(cam->headUp).normalizeRe()) * cam->camSpeed * cam->timeStep)[2];
        }
        if (GLFW_KEY_S == key)
        {
            cam->position[0] += (cam->camFront * cam->camSpeed * cam->timeStep)[0];
            cam->position[1] += (cam->camFront * cam->camSpeed * cam->timeStep)[1];
            cam->position[2] += (cam->camFront * cam->camSpeed * cam->timeStep)[2];
        }
        if (GLFW_KEY_D == key)
        {
            cam->position[0] += (cam->camFront.cross(cam->headUp).normalizeRe() * cam->camSpeed * cam->timeStep)[0];
            cam->position[1] += (cam->camFront.cross(cam->headUp).normalizeRe() * cam->camSpeed * cam->timeStep)[1];
            cam->position[2] += ((cam->camFront.cross(cam->headUp).normalizeRe()) * cam->camSpeed * cam->timeStep)[2];
        }

    });
    window->SetMousePressFunction([this, cam](int32 key, int32 state, int32)
    {
        if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
        {
            cam->mousePressedLeft = true;
        }
        if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
        {
            cam->mousePressedLeft = false;
        }
        if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)
        {
            cam->mousePressed = true;
        }
        if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_RELEASE)
        {
            cam->mousePressed = false;
        }
    });

    window->SetMouseScrollFunction([this, cam](float64 x, float64 y)
    {
        cam->scrollX += 0.1f * y;
    });

    window->SetMouseMoveFunction([this, cam](float64 xpos, float64 ypos)
    {
        if (!cam->mousePressed)
        {
            cam->oldPosX = xpos;
            cam->oldPosY = ypos;
            return;
        }

        GLfloat xoffset = GLfloat(xpos - cam->oldPosX);
        GLfloat yoffset = GLfloat(cam->oldPosY - ypos);

        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        cam->yaw += xoffset;
        cam->pitch -= yoffset;

        if (cam->pitch > 89.0f)
            cam->pitch = 89.0f;
        if (cam->pitch < -89.0f)
            cam->pitch = -89.0f;

        vector3D front;
        front[0] = (cos(rad(cam->yaw)) * cos(rad(cam->pitch)));
        front[1] = (sin(rad(cam->pitch)));
        front[2] = -(sin(rad(cam->yaw)) * cos(rad(cam->pitch)));
        cam->camFront = front.normalizeRe();

        cam->oldPosX = xpos;
        cam->oldPosY = ypos;
    });


}

float KeyHandler::rad(float d)  //Degrees to rad
{
    float rad = (PI / 180.f) * d;
    return rad;
}
