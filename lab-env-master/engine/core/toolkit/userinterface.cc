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

        ImTextureID my_tex_id = (void *)(intptr_t)Servers::RenderServer::GetInstance()->lPass->gAlbedoSpec;
        ImGui::Image(my_tex_id, ImVec2(150, 150), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        ImTextureID my_tex_id2 = (void *)(intptr_t)Servers::RenderServer::GetInstance()->lPass->gNormal;
        ImGui::Image(my_tex_id2, ImVec2(150, 150), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        ImTextureID my_tex_id3 = (void *)(intptr_t)Servers::RenderServer::GetInstance()->lPass->gPosition;
        ImGui::Image(my_tex_id3, ImVec2(150, 150), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));


        ImGui::End();
    }

}