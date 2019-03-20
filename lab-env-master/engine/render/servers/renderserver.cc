#include "config.h"
#include "renderserver.h"
#include "render/passes/geometrypass.h"
#include "render/camera.h"
#include "render/window.h"

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

        glGenFramebuffers(1, &gBuffer);
        lPass = new Passes::LightPass();
        gPass = new Passes::GeometryPass();
        dofPass = new Passes::DofPass();

        dPass = new Passes::DrawPass();
        skyPass = new Passes::SkyboxPass(1500);

        lPass->Setup();
        gPass->Setup();
        dofPass->Setup();

        dPass->Setup();
        skyPass->Setup();

        passes.push_back(gPass);
        passes.push_back(lPass);
        if (DOF)
			passes.push_back(dofPass);
        passes.push_back(dPass);

        passes.push_back(skyPass);
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
        lPass->UpdateResolution();
    }
}
