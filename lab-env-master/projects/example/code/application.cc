//------------------------------------------------------------------------------
// application.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <imgui.h>
#include <algorithm>
#include "render/properties/graphicsproperty.h"
#include "render/servers/renderserver.h"

using namespace Display;

namespace Example
{
    //------------------------------------------------------------------------------
    /**
    */
    Application::Application()
    {
        // empty
    }
    //------------------------------------------------------------------------------
    /**
    */
    Application::~Application()
    {
        // empty
    }
    //------------------------------------------------------------------------------
    /**
    */
    bool
    Application::Open()
    {
        App::Open();
        window = new Window;
        window->SetSize(int32(1024), int32(768));
        keyHandler = Input::KeyHandler::GetInstance();
        entityManager = Managers::EntityManager::GetInstance();
        renderServer = Servers::RenderServer::GetInstance();
        ui = new Toolkit::UserInterface(this);
        lightManager = Managers::LightManager::GetInstance();
		sceneLoader = Toolkit::SceneLoader::GetInstance();

        if (window->Open())
        {
            //Setup everyting
			lightManager->AddDirectionalLight(vector3D(1.0f, 1.0f, 1.0f), .05f, .2f, vector3D(0.0f, -1.0, 0.0));
			//this->lightManager->AddPointLight(vector3D(1, 0, 0), 0.8, vector3D(0, 10, 0), 1, 0.09, 0.032);
            renderServer->Init(window);
            keyHandler->Init(window);
			sceneLoader->Init();
        	
            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                ui->Run();
            });
            // Set window resize function
            window->SetWindowResizeCallback([this](int w, int h)
            {
                renderServer->GetInstance()->ResolutionUpdated(w, h);
            });
            return true;
        }
        return false;
    }

    //------------------------------------------------------------------------------
    /**
    */
    void
    Application::Run()
    {
        while (!glfwWindowShouldClose(window->window))
        {
            this->window->Update();

            //Render
            renderServer->Render();
        }
    }


} // namespace Example
