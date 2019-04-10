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

		gaussX = std::make_shared<Resources::ShaderObject>("content/Shader/dofgausspass_x.vs", "content/Shader/dofgausspass_x.fs");
		gaussY = std::make_shared<Resources::ShaderObject>("content/Shader/dofgausspass_y.vs", "content/Shader/dofgausspass_y.fs");
		composit = std::make_shared<Resources::ShaderObject>("content/Shader/composit.vs", "content/Shader/composit.fs");
		
		gaussX->bind();
		gaussX->mod1i("inDownSampled", 0);
		gaussX->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		gaussX->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));

		vector2D v[7];
		//Setup sample offsets
		float dx = 1.0f / svr->width;
		float dy = 1.0f / svr->height;
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

    void DofGaussPass::Execute()
    {


		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		if (svr->dof_type != 2)
			return;

		//Gauss X------------------------
		gaussX->bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->downPass->downTex); //Downscaled color
		RenderQuad();

		//Gauss Y------------------------
		gaussY->bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->downPass->downTexX); //Downscaled with convolution in X color
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
		glBindTexture(GL_TEXTURE_2D, svr->downPass->downTexY); //Downscaled blurred 
		RenderQuad();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
    }

	void DofGaussPass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		gaussX->bind();
		gaussX->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		gaussX->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));
		vector2D v[7];
		//Setup sample offsets
		float dx = 1.0f / svr->width;
		float dy = 1.0f / svr->height;
		v[0] = vector2D(0.0f, 0.0f);
		v[1] = vector2D(1.3366f * dx, 0.0f);
		v[2] = vector2D(3.4295f * dx, 0.0f);
		v[3] = vector2D(5.4264f * dx, 0.0f);
		v[4] = vector2D(7.4359f * dx, 0.0f);
		v[5] = vector2D(9.4436f * dx, 0.0f);
		v[6] = vector2D(11.4401f * dx, 0.0f);
		gaussX->modVector2fArray("sampleArrayX", 7, v);

		gaussY->bind();
		gaussY->mod1f("resDownX", 1.f / ((svr->width + 1) * 0.5f));
		gaussY->mod1f("resDownY", 1.f / ((svr->height + 1) * 0.5f));
		v[0] = vector2D(0.0f, 0.0f);
		v[1] = vector2D(0.0f, 1.3366f * dy);
		v[2] = vector2D(0.0f, 3.4295f * dy);
		v[3] = vector2D(0.0f, 5.4264f * dy);
		v[4] = vector2D(0.0f, 7.4359f * dy);
		v[5] = vector2D(0.0f, 9.4436f * dy);
		v[6] = vector2D(0.0f, 11.4401f * dy);
		gaussY->modVector2fArray("sampleArrayY", 7, v);
		glUseProgram(0);
    }

}
