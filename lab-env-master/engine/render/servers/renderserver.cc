#include "config.h"
#include "renderserver.h"
#include "render/passes/geometrypass.h"
#include "render/camera.h"

namespace Servers
{
    RenderServer* RenderServer::instance = 0;

    RenderServer::RenderServer()
    {

    }

    void RenderServer::Render()
    {
        Display::Camera* cam = Display::Camera::GetInstance();
        for (int i = 0; i < passes.size(); ++i)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            passes[i]->Execute();
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        glBlitFramebuffer(0, 0, cam->m_width, cam->m_height, 0, 0, cam->m_width, cam->m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        //glBlitNamedFramebuffer(gBuffer, 0, 0, 0, cam->m_width, cam->m_height, 0, 0, cam->m_width, cam->m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void RenderServer::Setup()
    {
        lPass = new Passes::LightPass();
        lPass->Setup();
        gPass = new Passes::GeometryPass();
        gPass->Setup();
 
        passes.push_back(gPass);
        passes.push_back(lPass);
    }


    RenderServer* RenderServer::GetInstance()
    {
        if (instance == 0)
        {
            instance = new RenderServer();
        }
        return instance;
    }

}
