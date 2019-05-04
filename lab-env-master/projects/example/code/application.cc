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
    //------------------------------------------------------------------------------WWWWWW
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
        Base::Entity* temp2 = new Base::Entity();
        Base::Entity* temp3 = new Base::Entity();
        Base::Entity* temp4 = new Base::Entity();
        Properties::GraphicsProperty* prop = new Properties::GraphicsProperty();
        Properties::GraphicsProperty* prop2 = new Properties::GraphicsProperty();
        Properties::GraphicsProperty* prop3 = new Properties::GraphicsProperty();
        Properties::GraphicsProperty* prop4 = new Properties::GraphicsProperty();
        /*prop->mesh->LoadMesh("content/teapot.obj");
		prop3->mesh->LoadMesh("content/teapot.obj");
		prop3->model.setPos(vector3D(50., 0, 100.));
		prop4->mesh->LoadMesh("content/teapot.obj");
		prop4->model.setPos((vector3D(100, 0, 200.)));
        prop2->mesh->LoadMesh("content/cubeNormal.obj");
		prop2->model.scale(100.f, 100.f, 100.f);*/
        //prop->mesh->LoadMesh("content/ama/Interior/interior.obj");
        prop->mesh->LoadMesh("content/ocrytek_sponza/sponza.obj");
		//prop->mesh->LoadMesh("content/cat.obj");
        temp->AttachProperty(prop);
        temp2->AttachProperty(prop2);
		temp3->AttachProperty(prop3);
		temp4->AttachProperty(prop4);
        this->entityManager->AttachEntity(temp);
        //this->entityManager->AttachEntity(temp2);
        //this->entityManager->AttachEntity(temp3);
        //this->entityManager->AttachEntity(temp4);
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
			this->lightManager->AddDirectionalLight(vector3D(1.0f, 1.0f, 1.0f), .05f, .2f, vector3D(0.0f, -1.0, 0.0));
			//this->lightManager->AddPointLight(vector3D(1, 0, 0), 0.8, vector3D(0, 10, 0), 1, 0.09, 0.032);
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
        while (!glfwWindowShouldClose(this->window->window))
        {
            this->window->Update();

            //Render
            renderServer->Render();
        }
    }


} // namespace Example
