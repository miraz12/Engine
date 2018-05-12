//------------------------------------------------------------------------------
// application.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <imgui.h>
#include <algorithm>


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

	void Application::LightSetup()
	{
		//LigtNode
		lNode = std::make_shared<LightManager>(0.5f, 15.0f);
        lNode->AddDirectionalLight(vector3D(1.0f, 1.0f, 1.0f), 0.05f, 0.2f, vector3D(0.0f, -1.0, 0.0));
	}

	void Application::UpdateLights(double time)
	{
        //empty
	}

	void Application::ObjectSetup()
	{
        this->entityManager->AddNewEntity("content/cat.obj");
        //this->entityManager->AddNewEntity("content/ocrytek_sponza/sponza.obj");
	}

	//------------------------------------------------------------------------------
/**
*/
    bool
    Application::Open()
    {
        App::Open();

        this->window = new Display::Window;
		this->window->SetSize(int32(1024), int32(768));
        this->mainCamera = Camera::GetInstance();
        this->keyHanderl = new KeyHandler();
        this->entityManager = new EntityManager();
        this->ui = new UserInterface(this);

        if (this->window->Open())
        {
			//Setup everyting
			LightSetup();
            entityManager->Init(lNode);
			ObjectSetup();
            keyHanderl->Init(window);


            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                ui->Run();
            });
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
			UpdateLights(currentTime);
            entityManager->DrawEntitys();
            lastTime = currentTime;

            this->window->SwapBuffers();
        }
    }

    void Application::CalculateFPS(double& currentTime, double& lastTimeFPS, int& nbFrames)
    {
        if (currentTime - lastTimeFPS >= 1.0) { // If last prinf() was more than 1 sec ago
            FPS = nbFrames;
            nbFrames = 0;
            lastTimeFPS += 1.0;
        }
    }
} // namespace Example