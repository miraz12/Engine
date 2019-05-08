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
		shader = std::make_shared<Resources::ShaderObject>("content/Shader/downsamplepass.vs", "content/Shader/downsamplepass.fs");

		shader->bind();
		shader->mod1i("inFullRes", 0);
		shader->mod1i("inFullResDepth", 1);
		shader->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		shader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		glGenFramebuffers(1, &downFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, downFBO);
		// position buffer
		glGenTextures(1, &downTex);
		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downTex, 0);

		glGenTextures(1, &downTexX);
		glBindTexture(GL_TEXTURE_2D, downTexX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, downTexX, 0);

		glGenTextures(1, &downTexY);
		glBindTexture(GL_TEXTURE_2D, downTexY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, downTexY, 0);

		// depth buffer
		glGenTextures(1, &downDepth);
		glBindTexture(GL_TEXTURE_2D, downDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, svr->width*0.5f, svr->height*0.5f, 0, GL_RG, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, downDepth, 0);



		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, attachments);

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

		if (!svr->downssample)
			return;

		//Downsample screen--------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, downFBO);

		this->shader->bind();

		//Bind lighting shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gDepth);
		//Render quad that covers the whole screen
		RenderQuad();

		glUseProgram(0);


    }

	void DownsamplePass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		shader->bind();
		shader->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		shader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, downTexX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, downTexY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, svr->width*0.5f, svr->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, downDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, svr->width*0.5f, svr->height*0.5f, 0, GL_RG, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
    }

}
