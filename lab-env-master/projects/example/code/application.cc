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
        this->mainCamera = Camera::GetInstance();
        mainCamera->m_width = 1024;
        mainCamera->m_height = 768;
        this->keyHandler = new Input::KeyHandler();
        this->entityManager = Managers::EntityManager::GetInstance();
        this->renderServer = Servers::RenderServer::GetInstance();
        this->ui = new Toolkit::UserInterface(this);


        if (this->window->Open())
        {
            //Setup everyting
            Managers::LightManager::GetInstance()->AddDirectionalLight(vector3D(1.0f, 1.0f, 1.0f), 0.05f, 0.2f,
                                                                       vector3D(0.0f, -1.0, 0.0));

            renderServer->Setup();
            ObjectSetup();
            keyHandler->Init(window);
            skybox = new Skybox::Skybox(1500);


            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                ui->Run();
            });
            // Set window resize function
            window->SetWindowResizeCallback([this](float w, float h)
            {
                mainCamera->UpdatePerspective(w, h);
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
        //Time variables for calculationg alg time
        double lastTime = glfwGetTime();
        double lastTimeFPS = glfwGetTime();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        //glEnable(GL_MULTISAMPLE);
        //Wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int nbFrames = 0;
        while (!glfwWindowShouldClose(this->window->window))
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->window->Update();
            //Calculate deltaTime
            double currentTime = glfwGetTime();
            deltaTime = fabs(currentTime - lastTime);

            // Measure fps
            nbFrames++;
            CalculateFPS(currentTime, lastTimeFPS, nbFrames);

            //Update things
            renderServer->Render();
            skybox->Draw(this->mainCamera->view, this->mainCamera->projection);
            lastTime = currentTime;

            this->window->SwapBuffers();
        }
    }

    //------------------------------------------------------------------------------
    /**
    */
    void Application::CalculateFPS(double& currentTime, double& lastTimeFPS, int& nbFrames)
    {
        if (currentTime - lastTimeFPS >= 1.0)
        {
            // If last prinf() was more than 1 sec ago
            FPS = nbFrames;
            nbFrames = 0;
            lastTimeFPS += 1.0;
        }
    }

    //------------------------------------------------------------------------------
    /**
    */
} // namespace Example
