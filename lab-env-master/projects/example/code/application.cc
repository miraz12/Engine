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
    void Application::ObjectSetup()
    {
        Base::Entity* temp = new Base::Entity();
        Properties::GraphicsProperty* prop = new Properties::GraphicsProperty();
        //prop->mesh->LoadMesh("content/ocrytek_sponza/sponza.obj");
        //prop->mesh->LoadMesh("content/sponzafixed/sponza.obj");
	prop->mesh->LoadMesh("content/cat.obj");
        temp->AttachProperty(prop);
        this->entityManager->AttachEntity(temp);
    }

    //------------------------------------------------------------------------------
    /**
    */
    bool
    Application::Open()
    {
        App::Open();
        this->window = new Window;
        this->window->SetSize(int32(1024), int32(768));
        this->keyHandler = Input::KeyHandler::GetInstance();
        this->entityManager = Managers::EntityManager::GetInstance();
        this->renderServer = Servers::RenderServer::GetInstance();
        this->ui = new Toolkit::UserInterface(this);
        this->lightManager = Managers::LightManager::GetInstance();

        if (this->window->Open())
        {
            //Setup everyting
            this->lightManager->AddDirectionalLight(vector3D(1.0f, 1.0f, 1.0f), 0.05f, 0.2f,
                                                                       vector3D(0.0f, -1.0, 0.0));
            ObjectSetup();
            renderServer->Init(this->window);
            keyHandler->Init(window);

            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                ui->Run();
            });
            // Set window resize function
            window->SetWindowResizeCallback([this](float w, float h)
            {
                renderServer->GetInstance()->UpdateResolution(w, h);
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
        while (!glfwWindowShouldClose(this->window->window))
        {
            this->window->Update();

            //Render
            renderServer->Render();
        }
    }


} // namespace Example
