#include "config.h"
#include "depthoffieldgausspass.h"
#include "render/servers/renderserver.h"
#include "render/camera.h"
#include "core/math/vector2D.h"

namespace Passes
{
	DofGaussPass::DofGaussPass()
    {
		downsampleShader = std::make_shared<Resources::ShaderObject>("content/Shader/downsamplepass.vs", "content/Shader/downsamplepass.fs");
		shader = std::make_shared<Resources::ShaderObject>("content/Shader/dofgausspass.vs", "content/Shader/dofgausspass.fs");

		shader->bind();
		shader->mod1i("gColor", 0); 
		shader->mod1i("inDownSampled", 1);


		downsampleShader->bind();
		downsampleShader->mod1i("inFragColor", 0); 


		glGenFramebuffers(1, &downFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, downFBO);

		// position buffer
		glGenTextures(1, &downTex);
		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downTex, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);

    }

	DofGaussPass::~DofGaussPass()
    {
	}

    void DofGaussPass::Setup()
    {

    }

    void DofGaussPass::Execute()
    {

		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		//Downsample screen-----------------------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, downFBO);
		this->downsampleShader->bind();
		downsampleShader->mod1f("resDownX", 1.f/((svr->width+1) * 0.5f));
		downsampleShader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->gBuffer->gColor);

		RenderQuad();


		//Dof----------------------
		svr->pBuffer->DrawBuffer();
		this->shader->bind();
		Display::Camera* cam = Display::Camera::GetInstance();
		shader->mod1f("resX", svr->width);
		shader->mod1f("resY", svr->height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->gBuffer->gColor); //Fullscale color
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, downTex); //Downscaled color


		//Render quad that covers the whole screen
		RenderQuad();
			   
		glUseProgram(0);
    }

	void DofGaussPass::UpdateResolution()
    {

		glUseProgram(0);
    }

}
