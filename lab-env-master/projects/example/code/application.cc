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
		for (unsigned int i = 0; i < objList.size(); i++)
		{
			delete objList[i];
		}

    }

	void Application::LightSetup()
	{
		//LigtNode
		lNode = std::make_shared<LightResource>();

		DirectionalLight dirLight = DirectionalLight();
		dirLight.Color = vector3D(1.0f, 1.0f, 1.0f);
		dirLight.AmbientIntensity = 0.05f;
		dirLight.DiffuseIntensity = 0.2f;
		dirLight.Direction = vector3D(0.0f, -1.0, 0.0);
		lNode->m_dLights.push_back(dirLight);

		//PointLight pLight1 = PointLight();
		//pLight1.DiffuseIntensity = 5.0f; // 0.25f;
		//pLight1.Color = vector3D(1.0f, 1.0f, 1.0f);
		//pLight1.Position = vector3D(0.0f, 50.0f, 5.f);
		//pLight1.Attenuation.Linear = 0.1f;
		//PointLight pLight2 = PointLight();
		//pLight2.DiffuseIntensity = 5.0f; // 0.25f;
		//pLight2.Color = vector3D(1.0f, 1.f, 1.0f);
		//pLight2.Position = vector3D(7.0f, 1.0f, 40.f * (sinf(0.0057f) + 1.0f));
		//pLight2.Attenuation.Linear = 0.1f;
		//lNode->m_pLights.push_back(pLight1);
		//lNode->m_pLights.push_back(pLight2);

		//SpotLight sLight1 = SpotLight();
		//sLight1.DiffuseIntensity = 0.9f;
		//sLight1.Color = vector3D(1.0f, 1.0f, 1.0f);
		//sLight1.Position = vector3D(camera.x(), camera.y(), -camera.z());
		//sLight1.Direction = camFront;
		//sLight1.Attenuation.Linear = 0.1f;
		//sLight1.Cutoff = 10.0f;

		//SpotLight sLight2 = SpotLight();
		//sLight2.DiffuseIntensity = 0.9f;
		//sLight2.Color = vector3D(1.0f, 1.0f, 1.0f);
		//sLight2.Position = vector3D(0.0f, 3.0f, 0.0f);
		//sLight2.Direction = vector3D(0.0f, 1.0f, 0.0f);
		//sLight2.Attenuation.Linear = 0.1f;
		//sLight2.Cutoff = 20.0f;

		//lNode->m_sLights.push_back(sLight1);
		//lNode->m_sLights.push_back(sLight2);

		lNode->specIntensity = 0.5f;
		lNode->specPower = 15.0f;
	}

	void Application::UpdateLights(double time)
	{
		//matrix4D boxMat1;
		//matrix4D boxMat2;

		//vector3D newPos1 = vector3D(70.0f, 500.0f, 0.f);
		//vector3D newPos2 = vector3D(-70.0f, 500.0f, 0.f);

		//lNode->m_pLights[0].Position = newPos1;
		//lNode->m_pLights[1].Position = newPos2;
		//boxMat1.setPos(newPos1);
		//boxMat2.setPos(newPos2);
		//box1->getMesh()->setMM(boxMat1);
		//box2->getMesh()->setMM(boxMat2);

		//lNode->m_sLights[0].Position = camera;
		//lNode->m_sLights[0].Direction = camFront;
	}

	void Application::ObjectSetup()
	{

		//box1 = new GraphicsNode();
		//box1->getMesh()->LoadMesh("content/box.obj");
		//box1->setLight(lNode);
		//box1->getLightNode()->setPos(vector3D(5, 5, 0));
		//objList.push_back(box1);

		//box2 = new GraphicsNode();
		//box2->getMesh()->LoadMesh("content/Flame Core Outside/Volcano Lava.obj");
		//box2->setLight(lNode);
		//objList.push_back(box2);



		box3 = new GraphicsNode();
		//box3->getMesh()->LoadMesh("content/cat.obj");
		box3->getMesh()->LoadMesh("content/ocrytek_sponza/sponza.obj");
		box3->setLight(lNode);
		objList.push_back(box3);



	}

	void Application::UpdateObjects(double time, double deltatime)
	{
        mainCamera->view = mainCamera->view.LookAtRH(mainCamera->position, mainCamera->position + mainCamera->camFront, mainCamera->headUp);

		for (unsigned int i = 0; i < objList.size(); i++)
		{
			objList[i]->camera = mainCamera->position;
                        matrix4D modelM = objList[i]->getMesh()->getMM();
                        //modelM.scale(0.3, 0.3, 0.3);

			objList[i]->drawOBJ(mainCamera->projection, mainCamera->view, modelM);
		}
	}

	//------------------------------------------------------------------------------
/**
*/
    bool
    Application::Open()
    {
        App::Open();
        wWidht = 1024;
        wHeight = 768;
        aspect = 4.0f/3.0f; // 3:2

        this->window = new Display::Window;
		this->window->SetSize(int32(wWidht), int32(wHeight));
        this->mainCamera = new Camera(wWidht, wHeight);
        this->keyHanderl = new KeyHandler();

        window->SetWindowResizeCallback([this](void)
        {
            wWidht = window->GetWidth();
            wHeight = window->GetHeight();
            mainCamera->projection = mainCamera->projection.setPerspective(45.0f, (wWidht / wHeight), 1.0f, 100.0f);
        });


				
        if (this->window->Open())
        {
			//Setup everyting
            mainCamera->projection = mainCamera->projection.setPerspective(45.0f, (wWidht / wHeight), 1.0f, 100.0f);
			LightSetup();
			ObjectSetup();
            keyHanderl->Init(window, mainCamera);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            // set ui rendering function
            this->window->SetUiRender([this]()
            {
                this->RenderUI();
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
        FPS = 0;

        while (!glfwWindowShouldClose(this->window->window))
        {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
        	//Calculate deltaTime
			double currentTime = glfwGetTime();
			deltaTime = fabs(currentTime - lastTime);

            // Measure fps
            nbFrames++;
            if (currentTime - lastTimeFPS >= 1.0) { // If last prinf() was more than 1 sec ago
                                                    // printf and reset timer
                FPS = nbFrames;
                nbFrames = 0;
                lastTimeFPS += 1.0;
            }

			//Update things
			UpdateLights(currentTime);
			UpdateObjects(currentTime, deltaTime);

            this->window->SwapBuffers();


			lastTime = currentTime;
        }
    }

    void Application::RenderUI()
    {
        if (this->window->IsOpen())
        {
            bool show = true;
            ImGui::Begin("Debug", &show);
			ImGui::Text("FPS: %i", FPS);


			if (ImGui::Button("Reload Shader"))
			{
				for (unsigned int i = 0; i < objList.size(); i++)
				{
					objList[i]->getShader()->ReloadShader();
				}
			}

        	ImGui::End();
        }
    }



} // namespace Example