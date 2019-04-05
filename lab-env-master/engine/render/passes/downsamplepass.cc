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

		downsampleShader = std::make_shared<Resources::ShaderObject>("content/Shader/downsamplepass.vs", "content/Shader/downsamplepass.fs");
		
		downsampleShader->bind();
		downsampleShader->mod1i("inFragColor", 0);
		downsampleShader->mod1i("gDepth", 1);
		downsampleShader->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		downsampleShader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

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
		glUseProgram(0);

    }

	DownsamplePass::~DownsamplePass()
    {
	}

    void DownsamplePass::Execute()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		if ((svr->dof_type != 2))
			return;

		//Downsample screen--------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, downFBO);
		this->downsampleShader->bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gDepth); //depth
		RenderQuad();

		glUseProgram(0);
    }

	void DownsamplePass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		downsampleShader->bind();
		downsampleShader->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		downsampleShader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glUseProgram(0);
    }

}
