#include "config.h"
#include "renderserver.h"
#include "render/passes/geometrypass.h"
#include "render/camera.h"
#include "render/window.h"

#define G_WIREFRAME false

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

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        skybox->Draw(cam->view, cam->projection);

        //glAlphaFunc(GL_ALWAYS, 0.0f);
        //glDepthFunc(GL_LESS);


        this->window->SwapBuffers();
    }

    void RenderServer::Init(Display::Window* window )
    {
        this->window = window;

        glGenFramebuffers(1, &gBuffer);
        skybox = new Skybox::Skybox(1500);
        lPass = new Passes::LightPass();
        gPass = new Passes::GeometryPass();
        dofPass = new Passes::DofPass();


        dPass = new Passes::DrawPass();

        lPass->Setup();
        gPass->Setup();
        dofPass->Setup();

        dPass->Setup();
 
        passes.push_back(gPass);
        passes.push_back(lPass);
        passes.push_back(dofPass);

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
        lPass->UpdateResolution();
    }

    void RenderServer::BindGBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    }

    void RenderServer::ReadGBuffer()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    }
    void RenderServer::DrawGBuffer()
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);
    }
}
