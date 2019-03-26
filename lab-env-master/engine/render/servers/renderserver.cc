#include "config.h"
#include "renderserver.h"
#include "render/passes/geometrypass.h"
#include "render/camera.h"
#include "render/window.h"
#include "render/framebuffers/gbufferobject.h"
#include "../../../projects/example/code/application.h"

#define G_WIREFRAME false
#define DOF true

namespace Servers
{
    RenderServer* RenderServer::instance = 0;

    RenderServer::RenderServer()
    {
        width = 1024;
        height = 768;
    }

    void RenderServer::Render()
    {

        //Render as wireframe
        if (!G_WIREFRAME)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_TEXTURE_2D);
        }

        Display::Camera* cam = Display::Camera::GetInstance();
        cam->UpdateCamera(width, height);


        for (int i = 0; i < passes.size(); ++i)
        {
            passes[i]->Execute();
        }

        this->window->SwapBuffers();
    }

    void RenderServer::Init(Display::Window* window )
    {
        this->window = window;

		//Setup framebuffers
    	gBuffer = new FrameBuffers::GBufferObject(this);
		pBuffer = new FrameBuffers::PostBuffer(this);

		//Setup passes
        lPass = new Passes::LightPass();
        gPass = new Passes::GeometryPass();
        dofPass = new Passes::DofPass();
		skyPass = new Passes::SkyboxPass(1500);
        dPass = new Passes::DrawPass();
		dGPass = new Passes::DofGaussPass();

		//Geometry pass -> light pass -> skybox pass -> pos processing (DoF) -> Draw to screen
        passes.push_back(gPass);
        passes.push_back(lPass);
		//passes.push_back(dofPass);
		passes.push_back(dGPass);
        passes.push_back(dPass);

    }

    RenderServer* RenderServer::GetInstance()
    {
        if (instance == 0)
        {
            instance = new RenderServer();
        }
        return instance;
    }

    void RenderServer::UpdateResolution(int w, int h)
    {
        width = w;
        height = h;
        gBuffer->UpdateResolution();
        pBuffer->UpdateResolution();
        dofPass->UpdateResolution();
    }
}
