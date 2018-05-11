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
#include "core/math/matrix4D.h"
#include "render/resources/graphicsnode.h"
#include "render/particles/particlesystem.h"
#include "render/input/keyhandler.h"


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
private:

	void CreatePlane(vector3D a, vector3D b, vector3D c, vector3D d); //Create a plane
	void CreateRay(vector3D a, vector3D b);		//Create a ray
	void RayFromMouse();	//Cast ray from mouse
	void RenderUI(); 	//UI handler, draws UI

	void LightSetup(); // Setup all the lights TODO: fix an easier way to add light
	void ObjectSetup(); // Setup all the objects TODO: Fix easier way to do this.

	void UpdateLights(double time);	//Update all lights that should be updated
	void UpdateObjects(double time, double deltatime);	//Update all objects that should be updated

    Display::Window* window;
    float aspect;
    float wWidht, wHeight;

    Camera* mainCamera; 
    KeyHandler* keyHanderl;

	int FPS;
	double deltaTime;

	GraphicsNode* box1;
	GraphicsNode* box2;
	GraphicsNode* box3;

	std::vector<GraphicsNode*> objList;
	std::shared_ptr<LightResource> lNode;


};
} // namespace Example