#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "MeshResource.h"
#include "matrix4D.h"
#include "GraphicsNode.h"

namespace Example
{
class Application : public Core::App
{
public:
	/// constructor
	Application();
	/// destructor
	~Application();

    float rad(float d);

    /// open app
	bool Open();
	/// run app
	void Run();
private:

	void KeyInput();
	void CreatePlane(vector3D a, vector3D b, vector3D c, vector3D d); //Create a plane
	void CreateRay(vector3D a, vector3D b);		//Create a ray
	void RayFromMouse();	//Cast ray from mouse
	void RenderUI(); 	//UI handler, draws UI

	void LightSetup(); // Setup all the lights TODO: fix an easier way to add light
	void ObjectSetup(); // Setup all the objects TODO: Fix easier way to do this.

	void UpdateLights(double time);	//Update all lights that should be updated
	void UpdateObjects(double time);	//Update all objects that should be updated


	bool showUI = true;
	int uiIt = 0;

	float rayForce = 0.25f;

    bool mousePressed = false;
    bool mousePressedLeft = false;
	matrix4D projection;
	matrix4D view;

    Display::Window* window;
    float64 posX = 0.0f;
    float64 posY = 0.0f;
    float64 oldPosX = 0.0f;
    float64 oldPosY = 0.0f;
    float64 x;
    float64 y;
    float64 z = 0.0f;
    float64 scrollX = 0.0f;

    //Move cam
    vector3D camera;
    vector3D origin;
    vector3D headUp;
    vector3D camFront;
    bool firstM = true;
    float pitch;
    float yaw;
    //---

	int FPS;
	double deltaTime;
	float camSpeed = 1.0f;//100.f;

	GraphicsNode* box1;
	GraphicsNode* box2;
	GraphicsNode* box3;

	std::vector<GraphicsNode*> objList;

	std::shared_ptr<LightNode> lNode;

    float mX, mY;
    float moX, moY;

	float wWidht, wHeight;

    float timeStep = 1.0f/16.0f;





};
} // namespace Example