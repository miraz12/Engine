//------------------------------------------------------------------------------
// application.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <imgui.h>
#include <algorithm>
#include <ctime>


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
		for (auto i = 0; i < objList.size(); i++)
		{
			delete objList[i];
		}
    }

	void Application::LightSetup()
	{
		//LigtNode
		lNode = std::make_shared<LightNode>();

		DirectionalLight dirLight = DirectionalLight();
		dirLight.Color = vector3D(1.0f, 1.0f, 1.0f);
		dirLight.AmbientIntensity = 0.00f;
		dirLight.DiffuseIntensity = 0.2f;
		dirLight.Direction = vector3D(0.0f, 0.0, 1.0);
		lNode->m_dLights.push_back(dirLight);

		PointLight pLight1 = PointLight();
		pLight1.DiffuseIntensity = 5000.0f; // 0.25f;
		pLight1.Color = vector3D(1.0f, 1.0f, 1.0f);
		pLight1.Position = vector3D(0.0f, 25000.0f, 0.f *(cosf(0.0057f) + 1.0f));
		pLight1.Attenuation.Linear = 0.1f;
		PointLight pLight2 = PointLight();
		pLight2.DiffuseIntensity = 5.0f; // 0.25f;
		pLight2.Color = vector3D(1.0f, 1.f, 1.0f);
		pLight2.Position = vector3D(7.0f, 1, 40.f * (sinf(0.0057f) + 1.0f));
		pLight2.Attenuation.Linear = 0.1f;
		lNode->m_pLights.push_back(pLight1);
		//lNode->m_pLights.push_back(pLight2);

		SpotLight sLight1 = SpotLight();
		sLight1.DiffuseIntensity = 0.9f;
		sLight1.Color = vector3D(1.0f, 1.0f, 1.0f);
		sLight1.Position = vector3D(camera.x(), camera.y(), -camera.z());
		sLight1.Direction = camFront;
		sLight1.Attenuation.Linear = 0.1f;
		sLight1.Cutoff = 10.0f;

		SpotLight sLight2 = SpotLight();
		sLight2.DiffuseIntensity = 0.9f;
		sLight2.Color = vector3D(1.0f, 1.0f, 1.0f);
		sLight2.Position = vector3D(0.0f, 3.0f, 0.0f);
		sLight2.Direction = vector3D(0.0f, 1.0f, 0.0f);
		sLight2.Attenuation.Linear = 0.1f;
		sLight2.Cutoff = 20.0f;

		//lNode->m_sLights.push_back(sLight1);
		//lNode->m_sLights.push_back(sLight2);

		lNode->specIntensity = 1.0f;
		lNode->specPower = 32.0f;
	}

	void Application::UpdateLights(double time)
	{
		matrix4D boxMat1;
		matrix4D boxMat2;

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
		terrainSeed = 12345;
		frequency = 1;
		octave = 8;
		lacunarity = 2.0;
		persistence = 0.5;
		tWidth = 256;
		tHeigth= 256;

		tScaleX = 1.0f;
		tScaleY = 0.25f;
		tScaleZ = 1.0f;

		noiseType = 0;

		terrainObject = new GraphicsNode();
		terrainObject->getMesh()->GenerateTerrain(	tWidth, tHeigth, frequency, octave,
													lacunarity, persistence, terrainSeed, tScaleX,
													tScaleY, tScaleZ, noiseType);
		terrainObject->setLight(lNode);


		//box1 = new GraphicsNode();
		//box1->getMesh()->LoadMesh("content/box.obj");
		//box1->setLight(lNode);
		//box1->getLightNode()->setPos(vector3D(5, 5, 0));
		//objList.push_back(box1);

		//box2 = new GraphicsNode();
		//box2->getMesh()->LoadMesh("content/box.obj");
		//box2->setLight(lNode);
		//objList.push_back(box2);

		box3 = new GraphicsNode();
		box3->getMesh()->LoadMesh("content/cat.obj");
		//box3->getMesh()->LoadMesh("content/ocrytek_sponza/sponza.obj");
		box3->setLight(lNode);
		objList.push_back(box3);
	}

	void Application::UpdateObjects(double time)
	{
		view = view.LookAtRH(camera, camera + camFront, headUp);

		terrainObject->camera = camera;
		terrainObject->drawTerrain(projection, view, terrainObject->getMesh()->getMM());

		for (unsigned int i = 0; i < objList.size(); i++)
		{
			objList[i]->camera = camera;
			objList[i]->drawOBJ(projection, view, objList[i]->getMesh()->getMM());
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

        this->window = new Display::Window;
		this->window->SetSize(int32(wWidht), int32(wHeight));

        KeyInput();	//Litsen on keys
				
        if (this->window->Open())
        {
			//Setup everyting
			projection = projection.setPerspective(45.0f, (wWidht / wHeight), 1.0f, 100.0f);
			LightSetup();
			ObjectSetup();

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

        glEnable(GL_DEPTH_TEST);
		//glEnable(GL_MULTISAMPLE);
        //Wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        while (!glfwWindowShouldClose(this->window->window))
        {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
        	//Calculate deltaTime
			double currentTime = glfwGetTime();
			deltaTime = fabs(currentTime - lastTime);
			FPS = int(1.0 / deltaTime);

			//Update things
			UpdateLights(currentTime);
			UpdateObjects(currentTime);

            this->window->SwapBuffers();
			lastTime = currentTime;
        }
    }

    float Application::rad(float d)  //Degrees to rad
    {
        float rad = (PI / 180.f) * d;
        return rad;
    }

    void Application::RenderUI()
    {
        if (this->window->IsOpen() && showUI)
        {
            bool show = true;
            ImGui::Begin("Debug", &show, ImGuiWindowFlags_NoSavedSettings);
			ImGui::Text("FPS: %i", FPS);
			ImGui::Text("Camera position:");
			ImGui::Text("x: %f, y: %f, z: %f", camera.x(), camera.y(), camera.z());

			/* initialize random seed: */
			srand(time(NULL));
			
        	//ImGui::CollapsingHeader("Buttons");

			/*if (ImGui::Button("Toggle normalmapping"))
			{
				for (unsigned int i = 0; i < objList.size(); i++)
				{
					objList[i]->activateNormal *= -1;
				}
			}*/
			/*if (ImGui::Button("Reload Shader"))
			{
				for (unsigned int i = 0; i < objList.size(); i++)
				{
					objList[i]->getShader()->ReloadShader();
				}
			}*/
			static float freq = frequency;
			static int oct = octave;
			static float lacu = lacunarity;
			static float per = persistence;
			static int dim[2];
        	dim[0] = tWidth;
			dim[1] = tHeigth;
			static float scal[3];
			scal[0] = tScaleX;
			scal[1] = tScaleY;
			scal[2] = tScaleZ;
			static int noise_type = noiseType;

			if (ImGui::Button("Generate new Terrain"))
			{
				terrainSeed = rand() % 10000 + 1;
				terrainObject->getMesh()->GenerateTerrain(	tWidth, tHeigth, frequency, octave,
															lacunarity, persistence, terrainSeed, tScaleX,
															tScaleY, tScaleZ, noise_type);
			}

			if (ImGui::Combo("Noise type", &noise_type, "Perlin\0Voronni\0Ridged-multifractal\0Billow\0Checkerboard\0Const\0Cylinders\0Spheres\0"))
			{
				noiseType = noise_type;
				terrainObject->getMesh()->GenerateTerrain(	tWidth, tHeigth, frequency, octave,
															lacunarity, persistence, terrainSeed, tScaleX,
															tScaleY, tScaleZ, noise_type);
			}

			if (ImGui::SliderInt2("Width and Height", dim, 1, 1080))
			{
				tWidth = dim[0];
				tHeigth = dim[1];

				terrainObject->getMesh()->GenerateTerrain(	tWidth, tHeigth, frequency, octave,
															lacunarity, persistence, terrainSeed, tScaleX,
															tScaleY, tScaleZ, noise_type);
			}
			if (ImGui::SliderFloat3("Scale x, y, z", scal, 0.0, 1.0))
			{
				tScaleX = scal[0];
				tScaleY = scal[1];
				tScaleZ = scal[2];

				terrainObject->getMesh()->GenerateTerrain(	tWidth, tHeigth, frequency, octave,
															lacunarity, persistence, terrainSeed, tScaleX,
															tScaleY, tScaleZ, noise_type);
			}

			if (noise_type == 1)
			{
				/*Use oct to not have an even more insane amount of arguments*/
				if (ImGui::SliderInt("Displacement", &oct, 0, 10))
				{
					octave = oct;
					terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
						lacunarity, persistence, terrainSeed, tScaleX,
						tScaleY, tScaleZ, noise_type);
				}
			}

			if (noise_type == 5)
			{
				/*Use oct to not have an even more insane amount of arguments*/
				if (ImGui::SliderFloat("Const Value", &freq, -1.0, 1.0))
				{
					frequency = freq;
					terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
						lacunarity, persistence, terrainSeed, tScaleX,
						tScaleY, tScaleZ, noise_type);
				}
			}

			if (noise_type != 4 && noise_type != 5)
			{
				if (ImGui::SliderFloat("Frequency", &freq, 0.001, 5))
				{
					frequency = freq;
					terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
						lacunarity, persistence, terrainSeed, tScaleX,
						tScaleY, tScaleZ, noise_type);
				}
			}

			if (noise_type == 0 || noise_type == 2 || noise_type == 3)
			{
				if (ImGui::SliderInt("Octaves", &oct, 1, 30))
				{
					octave = oct;
					terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
						lacunarity, persistence, terrainSeed, tScaleX,
						tScaleY, tScaleZ, noise_type);
				}
				if (ImGui::SliderFloat("Lacunarity", &lacu, 1.5, 3.5))
				{
					lacunarity = lacu;
					terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
						lacunarity, persistence, terrainSeed, tScaleX,
						tScaleY, tScaleZ, noise_type);
				}
				if (noise_type != 2)
				{

					if (ImGui::SliderFloat("Persistence", &per, 0.0, 1.0))
					{
						persistence = per;
						terrainObject->getMesh()->GenerateTerrain(tWidth, tHeigth, frequency, octave,
							lacunarity, persistence, terrainSeed, tScaleX,
							tScaleY, tScaleZ, noise_type);
					}
				}
			}
			ImTextureID my_tex_id = (void *)(intptr_t) terrainObject->getMesh()->tTextrue->m_texture;
			ImGui::Image(my_tex_id, ImVec2(tWidth, tHeigth), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255), ImColor(255, 255, 255, 128));
        	
			ImGui::End();
        }
    }


	void Application::KeyInput()
    {
        camera.setValues(0, 100, 2);
        origin.setValues(0, 0, 0);
        headUp.setValues(0, 1, 0);
        camFront.setValues(0.0f, 0.0f, 1.0f);
        yaw = -90.0f;    // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
        pitch = 0.0f;

        oldPosX = wWidht / 2.0f;
        oldPosY = wHeight / 2.0f;

        window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            {
                this->window->Close();
            }
            if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
            {
                camSpeed = 100.f;
            }
            if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
            {
                camSpeed = 2.5f;
            }
            if (GLFW_KEY_SPACE == key)
            {
				camera[1] += camSpeed;
            }
            if (GLFW_KEY_G == key)
            {
                if (uiIt > 1)
                {
                    if (showUI)
                    {
                        showUI = false;
                        uiIt = 0;
                    } else
                    {
                        showUI = true;
                        uiIt = 0;
                    }
                }
                uiIt++;
            }
            if (GLFW_KEY_R == key)
            {
                //glfwSetInputMode(this->window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                //mouseLocked = false;
            }
            if (GLFW_KEY_W == key)
            {
                camera[0] -= (camFront * camSpeed * timeStep)[0];
                camera[1] -= (camFront * camSpeed * timeStep)[1];
                camera[2] -= (camFront * camSpeed * timeStep)[2];
            }
            if (GLFW_KEY_A == key)
            {
                camera[0] -= (camFront.cross(headUp).normalizeRe() * camSpeed * timeStep)[0];
                camera[1] -= (camFront.cross(headUp).normalizeRe() * camSpeed * timeStep)[1];
                camera[2] -= ((camFront.cross(headUp).normalizeRe()) * camSpeed * timeStep)[2];
            }
            if (GLFW_KEY_S == key)
            {
                camera[0] += (camFront * camSpeed * timeStep)[0];
                camera[1] += (camFront * camSpeed * timeStep)[1];
                camera[2] += (camFront * camSpeed * timeStep)[2];
            }
            if (GLFW_KEY_D == key)
            {
                camera[0] += (camFront.cross(headUp).normalizeRe() * camSpeed * timeStep)[0];
                camera[1] += (camFront.cross(headUp).normalizeRe() * camSpeed * timeStep)[1];
                camera[2] += ((camFront.cross(headUp).normalizeRe()) * camSpeed * timeStep)[2];
            }


        });
        window->SetMousePressFunction([this](int32 key, int32 state, int32)
        {
            if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS)
            {
                mousePressedLeft = true;
            }
            if (key == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_RELEASE)
            {
                mousePressedLeft = false;
            }
            if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_PRESS)
            {
                mousePressed = true;
            }
            if (key == GLFW_MOUSE_BUTTON_RIGHT && state == GLFW_RELEASE)
            {
                mousePressed = false;
            }
        });

        window->SetMouseScrollFunction([this](float64 x, float64 y)
        {
            scrollX += 0.1f * y;
        });

        window->SetMouseMoveFunction([this](float64 xpos, float64 ypos)
        {
            if (!mousePressed)
            {
                oldPosX = xpos;
                oldPosY = ypos;
                return;
            }

            GLfloat xoffset = GLfloat(xpos - oldPosX);
            GLfloat yoffset = GLfloat(oldPosY - ypos);

            float sensitivity = 0.05f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            yaw += xoffset;
            pitch -= yoffset;

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            vector3D front;
            front[0] = (cos(rad(yaw)) * cos(rad(pitch)));
            front[1] = (sin(rad(pitch)));
            front[2] = -(sin(rad(yaw)) * cos(rad(pitch)));
            camFront = front.normalizeRe();

            oldPosX = xpos;
            oldPosY = ypos;
        });
    }

} // namespace Example