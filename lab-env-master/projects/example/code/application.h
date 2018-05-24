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
#include "render/resources/graphicsnode.h"
#include "render/input/keyhandler.h"
#include "render/managers/entitymanager.h"
#include "core/toolkit/userinterface.h"
#include "render/skybox/skybox.h"


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
	bool Open();
	/// run app
	void Run();

    int GetFPS() { return FPS; }

    Managers::EntityManager* GetEntityManager() const
    {
        return entityManager;
    }

private:

	void LightSetup(); // Setup all the lights 
	void ObjectSetup(); // Setup all the objects 
	void UpdateLights(double time);	//Update all lights that should be updated

    void CalculateFPS(double& currentTime, double& lastTimeFPS, int& nbFrames);

    //skybox
    Skybox::Skybox* skybox;
    Display::Window* window;
    Display::Camera* mainCamera;
    Input::KeyHandler* keyHandler;
    Managers::EntityManager* entityManager;
    Toolkit::UserInterface* ui;

    std::shared_ptr<Managers::LightManager> lNode;

	int FPS{ 0 };
	double deltaTime{ 0 };

};
} // namespace Example