#include "config.h"
#include "userinterface.h"
#include "imgui.h"
#include "../../../projects/example/code/application.h"


using namespace Core;

namespace Toolkit
{

    UserInterface::UserInterface(Example::Application* app)
    {
        this->app = app;
    }

    void UserInterface::Run()
    {
        bool show = true;
        ImGui::Begin("Debug", &show);
        ImGui::Text("FPS: %i", app->GetFPS());


        //Camera
        Display::Camera* cam = Display::Camera::GetInstance();
        ImGui::Text("Position:");
        ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->position.x(), cam->position.y(), cam->position.z());
        ImGui::Text("Front:");
        ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->camFront.x(), cam->camFront.y(), cam->camFront.z());

        ImGui::End();
    }

}