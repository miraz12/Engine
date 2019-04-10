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

		Display::Camera::GetInstance()->UpdateCamera(width, height);
	    if (resUpdated)
	    {
			UpdateResolution(width, height);
			resUpdated = false;
	    }

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
		cdPass = new Passes::DofComplex();
		downPass = new Passes::DownsamplePass();

		//Geometry pass -> light pass -> skybox pass -> pos processing (DoF) -> Draw to screen
        passes.push_back(gPass);
        passes.push_back(lPass);
		passes.push_back(downPass);
		passes.push_back(dofPass);
		passes.push_back(dGPass);
		passes.push_back(cdPass);
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

	void RenderServer::ResolutionUpdated(int w, int h)
    {
		width = w;
		height = h;
		this->resUpdated = true;
    }

    void RenderServer::UpdateResolution(int w, int h)
    {
        gBuffer->UpdateResolution();
        pBuffer->UpdateResolution();
        dofPass->UpdateResolution();
		dGPass->UpdateResolution();
		cdPass->UpdateResolution();
		downPass->UpdateResolution();

    }
}
