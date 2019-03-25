#include "config.h"
#include "depthoffieldpass.h"
#include "render/servers/renderserver.h"
#include "render/camera.h"
#include "core/math/vector2D.h"

namespace Passes
{
    DofPass::DofPass()
    {
        shader = std::make_shared<Resources::ShaderObject>("content/Shader/dofpass.vs", "content/Shader/dofpass.fs");

		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();


		shader->bind();
		shader->mod1i("gColor", 0); 
		shader->mod1i("gDepth", 1);

		vector2D v[12];
		//Setup sample offsets
		float dx = 0.5f / svr->width;
		float dy = 0.5f / svr->height;
		v[0] = vector2D(-0.326212f * dx, -0.40581f * dy);
		v[1] = vector2D(-0.840144f * dx, -0.07358f * dy);
		v[2] = vector2D(-0.695914f * dx, 0.457137f * dy);
		v[3] = vector2D(-0.203345f * dx, 0.620716f * dy);
		v[4] = vector2D(0.96234f * dx, -0.194983f * dy);
		v[5] = vector2D(0.473434f * dx, -0.480026f * dy);
		v[6] = vector2D(0.519456f * dx, 0.767022f * dy);
		v[7] = vector2D(0.185461f * dx, -0.893124f * dy);
		v[8] = vector2D(0.507431f * dx, 0.064425f * dy);
		v[9] = vector2D(0.89642f * dx, 0.412458f * dy);
		v[10] = vector2D(-0.32194f * dx, -0.932615f * dy);
		v[11] = vector2D(-0.791559f * dx, -0.59771f * dy);
		shader->modVector2fArray("sampleArray", 12, v);

		glUseProgram(0);
    }

    DofPass::~DofPass()
    {
    }



    void DofPass::Execute()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();
		svr->pBuffer->DrawBuffer();

		this->shader->bind();
		Display::Camera* cam =  Display::Camera::GetInstance();
		shader->mod1f("resX", svr->width);
		shader->mod1f("resY", svr->height);

		//Bind lighting shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->gBuffer->gColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->gBuffer->gDepth);
		

		//Render quad that covers the whole screen
		RenderQuad();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

	void DofPass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		shader->bind();

		vector2D v[12];
		//Setup sample offsets
		float dx = 0.5f / svr->width;
		float dy = 0.5f / svr->height;
		v[0] = vector2D(-0.326212f * dx, -0.40581f * dy);
		v[1] = vector2D(-0.840144f * dx, -0.07358f * dy);
		v[2] = vector2D(-0.695914f * dx, 0.457137f * dy);
		v[3] = vector2D(-0.203345f * dx, 0.620716f * dy);
		v[4] = vector2D(0.96234f * dx, -0.194983f * dy);
		v[5] = vector2D(0.473434f * dx, -0.480026f * dy);
		v[6] = vector2D(0.519456f * dx, 0.767022f * dy);
		v[7] = vector2D(0.185461f * dx, -0.893124f * dy);
		v[8] = vector2D(0.507431f * dx, 0.064425f * dy);
		v[9] = vector2D(0.89642f * dx, 0.412458f * dy);
		v[10] = vector2D(-0.32194f * dx, -0.932615f * dy);
		v[11] = vector2D(-0.791559f * dx, -0.59771f * dy);
		shader->modVector2fArray("sampleArray", 12, v);
		glUseProgram(0);
    }

}
