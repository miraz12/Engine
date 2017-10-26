//------------------------------------------------------------------------------
// application.cc
// (C) 2015-2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "application.h"
#include <cstring>
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
        this->window->SetSize(wWidht, wHeight);

        KeyInput();		//Litsen on keys
				
        if (this->window->Open())
        {
			//LigtNode
			lNode = std::make_shared<LightNode>();

			lNode->setPos(vector3D(0, 5, 0));

			//Meshes---

			//box1 = new GraphicsNode();
			//box1->getMesh()->LoadMesh("cat.obj");
			//box1->setLight(lNode);
			//box1->getLightNode()->setPos(vector3D(5, 5, 0));
			//objList.push_back(box1);


			//box2 = new GraphicsNode();
			//box2->getMesh()->LoadMesh("sphere.obj");
			//box2->setLight(lNode);
			//box2->getLightNode()->setPos(lNode->getPos());
			//objList.push_back(box2);

			box3 = new GraphicsNode();
			box3->getMesh()->LoadMesh("ocrytek_sponza/sponza.obj");
			//box3->getMesh()->LoadMesh("cat.obj");
			box3->setLight(lNode);
			objList.push_back(box3);


			///-------
				

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

        projection = projection.setPerspective(45.0f, (wWidht / wHeight), 1.0f, 2.0f);


		matrix4D boxMat; 
        matrix4D sphereMat;
		
		sphereMat.setPos(lNode->getPos());

    	matrix4D pv;

        //Time variables for calculationg alg time
        double lastTime = glfwGetTime();

        int iter = 0;

        glEnable(GL_DEPTH_TEST);
		//glEnable(GL_MULTISAMPLE);
        //Wireframe
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        while (!glfwWindowShouldClose(this->window->window))
        {
			//lNode->setPos(camera);
			//matrix4D sphereMove = box2->getMesh()->getMM().setPos(camera.x(), camera.y(), camera.z());
			//box2->getMesh()->setMM(sphereMove);


			//Calculate deltaTime
			double currentTime = glfwGetTime();
			deltaTime = fabs(currentTime - lastTime);
			FPS = 1 / deltaTime;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            this->window->Update();


			
            view = view.LookAtRH(camera, camera + camFront, headUp);
			//view = view.LookAtRH(vector3D(0, 0, 2), vector3D(0, 0, 2) + camFront, headUp);

            pv = view * projection;

            lastTime = currentTime;

			//box1->drawOBJ(projection, view,  boxMat);
			//sphere->drawOBJ(pv, sphereMat);

			for (int i = 0; i < objList.size(); i++)
			{
				matrix4D m = objList[i]->getMesh()->getMM();
				objList[i]->drawOBJ(projection, view, objList[i]->getMesh()->getMM());
			}


            this->window->SwapBuffers();
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
			
			if (ImGui::Button("Toggle normalmapping"))
			{
				for (int i = 0; i < objList.size(); i++)
				{
					objList[i]->activateNormal *= -1;
				}
			}
			if (ImGui::Button("Reload Shader"))
			{
				for (int i = 0; i < objList.size(); i++)
				{
					objList[i]->getShader()->ReloadShader();
				}
			}

        	
        	ImGui::End();
        }
    }

    void Application::KeyInput()
    {
        camera.setValues(0, 0, 2);
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
            if (GLFW_KEY_1 == key && action == GLFW_RELEASE)
            {
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

            GLfloat xoffset = xpos - oldPosX;
            GLfloat yoffset = oldPosY - ypos;

            float sensitivity = 0.05;
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