#include "config.h"
#include "userinterface.h"
#include "imgui.h"
#include "../../../projects/example/code/application.h"
#include "render/servers/renderserver.h"


using namespace Core;

namespace Toolkit
{

    UserInterface::UserInterface(Example::Application* app)
    {
        this->app = app;
		srv = Servers::RenderServer::GetInstance();
		dof_type = &srv->dof_type;
    }
    void UserInterface::Run()
    {
        ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
        ImGui::Begin("Debug", &show, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("FPS: (%.1f FPS)", ceilf(ImGui::GetIO().Framerate));
        Display::Camera* cam = Display::Camera::GetInstance();

        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::Text("Position:");
            ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->position.x(), cam->position.y(), cam->position.z());
            ImGui::Text("Front:");
            ImGui::Text("x: %.2f y: %.2f z: %.2f", cam->camFront.x(), cam->camFront.y(), cam->camFront.z());
        }

        
		ImGui::Combo("Depth of field", dof_type, "None\0Stochastic\0Gaussian\0CircularConvolution\0");

        if (*dof_type != 0)
        {
			ImGui::SliderFloat("Distance to plane in focus", &cam->distToFocus, 0.0f, 100.0f, "ratio = %.4f");
			ImGui::SliderFloat("Focal length", &cam->focalLen, 0.0f, 100.0f, "ratio = %.4f");
			ImGui::SliderFloat("Aperture", &cam->aperture, 0.0f, 100.0f, "ratio = %.4f");

			if(ImGui::Checkbox("Downsample", &srv->downssample))
			{
				srv->resUpdated = true;
			}

        }
    	

        if (ImGui::CollapsingHeader("G-Buffer"))
        {
            int imageSize = 180;
            ImTextureID albedo = (void *)(intptr_t)srv->gBuffer->gAlbedoSpec;
            ImGui::Image(albedo, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            ImGui::SameLine(200);
            ImTextureID normal = (void *)(intptr_t)srv->gBuffer->gNormal;
            ImGui::Image(normal, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));


            ImTextureID pos = (void *)(intptr_t)srv->gBuffer->gPosition;
            ImGui::Image(pos, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
            ImGui::SameLine(200);
            ImTextureID depth = (void *)(intptr_t)srv->gBuffer->gDepth;
            ImGui::Image(depth, ImVec2(imageSize, imageSize), ImVec2(0, 1), ImVec2(1, 0), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));

        }
        ImGui::End();

        /*ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
        ImGui::Begin("Lights", &show, ImGuiWindowFlags_AlwaysAutoResize);

        if (ImGui::Button("New DirectionalLight"))
        {
            showNewDir = true;
            dirLight = new DirectionalLight();

        }
        if (ImGui::Button("New PointLight"))
        {
            showNewPoint = true;
            pointLight = new PointLight();
        }
        if (ImGui::Button("New SpotLight"))
        {
            
        }
        if (showNewDir)
        {
            ImGui::Begin("DirectionalLight", &showNewDir, ImGuiWindowFlags_AlwaysAutoResize);
            Managers::LightManager::GetInstance()->AddDirectionalLight(dirLight);

            vector3D color;
            float ambientintensity;
            float diffuseintensity;
            vector3D direction;

            ImGui::SliderFloat3("Color", dirLight->Color.Varray, 0, 255);
            ImGui::SliderFloat3("Direction", dirLight->Direction.Varray, -1, 1);
            ImGui::SliderFloat("Ambient", &dirLight->AmbientIntensity, 0, 0.001f);
            ImGui::SliderFloat("Diffuse", &dirLight->DiffuseIntensity, 0, 0.001f);
            if (ImGui::Button("Done"))
            {
                showNewDir = false;
            }
            ImGui::End();
        }
        if (showNewPoint)
        {
            ImGui::Begin("DirectionalLight", &showNewPoint, ImGuiWindowFlags_AlwaysAutoResize);
            Managers::LightManager::GetInstance()->AddPointLight(pointLight);

            ImGui::SliderFloat3("Color", pointLight->Color.Varray, 0, 255);
            ImGui::SliderFloat3("Pos", pointLight->Position.Varray, -100, 100);

            ImGui::SliderFloat("Ambient", &pointLight->AmbientIntensity, 0, 0.001f);
            ImGui::SliderFloat("Diffuse", &pointLight->DiffuseIntensity, 0, 0.001f);

            ImGui::SliderFloat("Constant", &pointLight->Attenuation.Constant, 0, 0.001f);
            ImGui::SliderFloat("Linear", &pointLight->Attenuation.Linear, 0, 0.001f);
            ImGui::SliderFloat("Exp", &pointLight->Attenuation.Exp, 0, 0.001f);
            if (ImGui::Button("Done"))
            {
                showNewPoint = false;
            }
            ImGui::End();
        }
        ImGui::End();*/

    }
}