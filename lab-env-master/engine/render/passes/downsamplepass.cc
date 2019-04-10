#include "config.h"
#include "downsamplepass.h"
#include "render/servers/renderserver.h"
#include "render/camera.h"

namespace Passes
{
	DownsamplePass::DownsamplePass()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		glGenFramebuffers(1, &downFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, downFBO);
		// position buffer
		glGenTextures(1, &downTex);
		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downTex, 0);

		glGenTextures(1, &downTexX);
		glBindTexture(GL_TEXTURE_2D, downTexX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, downTexX, 0);

		glGenTextures(1, &downTexY);
		glBindTexture(GL_TEXTURE_2D, downTexY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, downTexY, 0);



		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

    }

	DownsamplePass::~DownsamplePass()
    {
	}

    void DownsamplePass::Execute()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		if ((svr->dof_type != 2)&&(svr->dof_type !=3))
			return;

		//Downsample screen--------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, downFBO);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, svr->pBuffer->fragColor, 0);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downTex, 0);

		glBlitFramebuffer(0, 0, svr->width, svr->height, 0, 0, svr->width*0.5, svr->height*0.5,	GL_COLOR_BUFFER_BIT, GL_LINEAR);

    }

	void DownsamplePass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();


		glBindTexture(GL_TEXTURE_2D, downTexX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, downTexY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

    }

}
