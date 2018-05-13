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


        if (ImGui::Button("Reload Shader"))
        {
            app->GetEntityManager()->ReloadShaders();
        }

        ImGui::End();
    }

}