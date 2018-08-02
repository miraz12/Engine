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
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 120), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
        ImGui::Begin("Debug", &show, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("FPS: (%.1f FPS)", ImGui::GetIO().Framerate);

        if (ImGui::CollapsingHeader("Camera"))
        {
            Display::Camera* cam = Display::Camera::GetInstance();
            ImGui::Text("Position:");
            ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->position.x(), cam->position.y(), cam->position.z());
            ImGui::Text("Front:");
            ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->camFront.x(), cam->camFront.y(), cam->camFront.z());
        }

        if (ImGui::CollapsingHeader("Help"))
        {
            int imageSize = 180;
            ImTextureID my_tex_id = (void *)(intptr_t)Servers::RenderServer::GetInstance()->getlPass()->gAlbedoSpec;
            ImGui::Image(my_tex_id, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            ImTextureID my_tex_id2 = (void *)(intptr_t)Servers::RenderServer::GetInstance()->getlPass()->gNormal;
            ImGui::Image(my_tex_id2, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            ImTextureID my_tex_id3 = (void *)(intptr_t)Servers::RenderServer::GetInstance()->getlPass()->gPosition;
            ImGui::Image(my_tex_id3, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        }
        ImGui::End();
    }
}