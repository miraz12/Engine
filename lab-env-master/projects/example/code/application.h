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
#include "core/toolkit/sceneloader.h"
#include "render/servers/renderserver.h"
#include "render/managers/lightmanager.h"


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

    private:
        Display::Window* window;
        Display::Camera* mainCamera;
        Input::KeyHandler* keyHandler;
        Managers::EntityManager* entityManager;
		Managers::LightManager* lightManager;
    	Servers::RenderServer* renderServer;
        Toolkit::UserInterface* ui;
		Toolkit::SceneLoader* sceneLoader;
    };
} // namespace Example
