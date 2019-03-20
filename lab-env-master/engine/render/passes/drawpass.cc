#include "config.h"
#include "drawpass.h"
#include "render/servers/renderserver.h"

namespace Passes
{
    DrawPass::DrawPass()
    {
        shader = std::make_shared<Resources::ShaderObject>("content/Shader/drawpass.vs", "content/Shader/drawpass.fs");
    }

    DrawPass::~DrawPass()
    {
    }

    void DrawPass::Setup()
    {
        shader->bind();
        shader->mod1i("gColor", 4); //0:position, 1:normal, 2:albedoSpec, 3:depth, 4:Fragcolor 
        glUseProgram(0);
    }

    void DrawPass::Execute()
    {

        Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to screen
        glBlitFramebuffer(0, 0, svr->width, svr->height, 0, 0, svr->width, svr->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST); //Copy depth

        //Bind lighting shader
        this->shader->bind();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, svr->getlPass()->gColor);

        //Render quad that covers the whole screen
		RenderQuad();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
