#include "config.h"
#include "depthoffieldgausspass.h"
#include "render/servers/renderserver.h"
#include "render/camera.h"
#include "core/math/vector2D.h"

namespace Passes
{
	DofGaussPass::DofGaussPass()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		downsampleShader = std::make_shared<Resources::ShaderObject>("content/Shader/downsamplepass.vs", "content/Shader/downsamplepass.fs");
		gaussX = std::make_shared<Resources::ShaderObject>("content/Shader/dofgausspassX.vs", "content/Shader/dofgausspassX.fs");
		gaussY = std::make_shared<Resources::ShaderObject>("content/Shader/dofgausspassY.vs", "content/Shader/dofgausspassY.fs");
		composit = std::make_shared<Resources::ShaderObject>("content/Shader/composit.vs", "content/Shader/composit.fs");
		
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downTex, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		gaussX->bind();
		gaussX->mod1i("inDownSampled", 0);
		gaussX->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		gaussX->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		vector2D v[7];
		//Setup sample offsets
		float dx = 0.5f / ((svr->width ) );
		float dy = 0.5f / ((svr->height ) );
		v[0] = vector2D(0.0f, 0.0f);
		v[1] = vector2D(1.3366f * dx, 0.0f);
		v[2] = vector2D(3.4295f * dx, 0.0f);
		v[3] = vector2D(5.4264f * dx, 0.0f);
		v[4] = vector2D(7.4359f * dx,0.0f);
		v[5] = vector2D(9.4436f * dx,0.0f);
		v[6] = vector2D(11.4401f * dx,0.0f);
		gaussX->modVector2fArray("sampleArrayX", 7, v);

		gaussY->bind();
		gaussY->mod1i("inDownSampled", 0);
		gaussY->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		gaussY->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));
		v[1] = vector2D(0.0f, 1.3366f * dy);
		v[2] = vector2D(0.0f, 3.4295f * dy);
		v[3] = vector2D(0.0f, 5.4264f * dy);
		v[4] = vector2D(0.0f, 7.4359f * dy);
		v[5] = vector2D(0.0f, 9.4436f * dy);
		v[6] = vector2D(0.0f, 11.4401f * dy);
		gaussY->modVector2fArray("sampleArrayY", 7, v);


		composit->bind();
		composit->mod1i("inFullRes", 0);
		composit->mod1i("inDownSampled", 1);

		
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

		if (svr->dof_type != 2)
			return;

		//Downsample screen--------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, downFBO);
		this->downsampleShader->bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gDepth); //depth
		RenderQuad();

		//Gauss X------------------------
		
		gaussX->bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, downTex); //Downscaled color
		RenderQuad();

		//Gauss Y------------------------
		gaussY->bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, downTex); //Downscaled color
		RenderQuad();


		//Dof---------------------------
		svr->pBuffer->DrawBuffer();
		composit->bind();
		Display::Camera* cam = Display::Camera::GetInstance();
		composit->mod1f("resX", svr->width);
		composit->mod1f("resY", svr->height);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->pBuffer->fragColor); //Fullscale color
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, downTex); //Downscaled blurred color


		//Render quad that covers the whole screen
		RenderQuad();
		glUseProgram(0);
    }

	void DofGaussPass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		downsampleShader->bind();
		downsampleShader->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		downsampleShader->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		gaussX->bind();
		vector2D v[7];
		//Setup sample offsets
		float dx = 0.5f / svr->width;
		float dy = 0.5f / svr->height;
		v[0] = vector2D(0.0f, 0.0f);
		v[1] = vector2D(1.3366f * dx, 0.0f);
		v[2] = vector2D(3.4295f * dx, 0.0f);
		v[3] = vector2D(5.4264f * dx, 0.0f);
		v[4] = vector2D(7.4359f * dx, 0.0f);
		v[5] = vector2D(9.4436f * dx, 0.0f);
		v[6] = vector2D(11.4401f * dx, 0.0f);
		gaussX->modVector2fArray("sampleArrayX", 7, v);

		gaussY->bind();
		v[0] = vector2D(0.0f, 0.0f);
		v[1] = vector2D(0.0f, 1.3366f * dy);
		v[2] = vector2D(0.0f, 3.4295f * dy);
		v[3] = vector2D(0.0f, 5.4264f * dy);
		v[4] = vector2D(0.0f, 7.4359f * dy);
		v[5] = vector2D(0.0f, 9.4436f * dy);
		v[6] = vector2D(0.0f, 11.4401f * dy);
		gaussY->modVector2fArray("sampleArrayY", 7, v);

		glBindTexture(GL_TEXTURE_2D, downTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, Servers::RenderServer::GetInstance()->width*0.5f, Servers::RenderServer::GetInstance()->height*0.5f, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glUseProgram(0);
    }

}
