#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "render/camera.h"
#include "render/input/keyhandler.h"
#include "render/managers/entitymanager.h"
#include "core/toolkit/userinterface.h"
#include "render/skybox/skybox.h"
#include "render/servers/renderserver.h"


namespace Example
{
    class Application : public Core::App
    {
    public:
        /// constructor
        Application();
        /// destructor
        ~Application();

        /// open app
        bool Open() override;
        /// run app
        void Run() override;

        int GetFPS() { return FPS; }

        Managers::EntityManager* GetEntityManager() const
        {
            return entityManager;
        }

    private:

        void ObjectSetup(); // Setup all the objects 
        void CalculateFPS(double& currentTime, double& lastTimeFPS, int& nbFrames);

        //skybox
        Skybox::Skybox* skybox;
        Display::Window* window;
        Display::Camera* mainCamera;
        Input::KeyHandler* keyHandler;
        Managers::EntityManager* entityManager;
        Servers::RenderServer* renderServer;
        Toolkit::UserInterface* ui;

        int FPS{0};
        double deltaTime{0};
    };
} // namespace Example
