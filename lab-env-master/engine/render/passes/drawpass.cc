#include "config.h"
#include "drawpass.h"
#include "render/servers/renderserver.h"

namespace Passes
{
    DrawPass::DrawPass()
    {

    }

    DrawPass::~DrawPass()
    {
    }

    void DrawPass::Execute()
    {

        Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
		svr->pBuffer->ReadBuffer();
    	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to screen
		glBlitFramebuffer(0, 0, svr->width, svr->height, 0, 0, svr->width, svr->height, GL_COLOR_BUFFER_BIT, GL_NEAREST); //Copy color

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
