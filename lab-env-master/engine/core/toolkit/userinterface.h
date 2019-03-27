#pragma once 
#include "core/app.h"
#include "render/resources/lighttypes.h"

namespace Example
{
    class Application;
}

namespace Servers
{
	class RenderServer;
}


namespace Toolkit
{
    class UserInterface
    {
    public:
        /*TODO: This is not very nice.. Make a super class that example app can derive from instead
        **TODO: or even better, create some kind of ui settings file.
        */
        UserInterface::UserInterface(Example::Application* app);
        void Run();

    private:
        Example::Application* app;
        bool gBufferWindow = false;

        bool show = true;
        bool showNewDir = false;
        bool showNewPoint = false;
        bool showNewSpot = false;

		int* dof_type;

		Servers::RenderServer* srv;

        DirectionalLight* dirLight;
        PointLight* pointLight;
        SpotLight* spotLight;
    };
}
