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
    }

	DofGaussPass::~DofGaussPass()
    {
	}

    void DofGaussPass::Setup()
    {
		downsampleShader->bind();
		downsampleShader->mod1i("gColor", 4); //0:position, 1:normal, 2:albedoSpec, 3:depth, 4:Fragcolor 

		glUseProgram(0);
    }

    void DofGaussPass::Execute()
    {
		this->downsampleShader->bind();
		glActiveTexture(GL_TEXTURE4);
		//glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gColor);

		RenderQuad();



		glUseProgram(0);
    }

	void DofGaussPass::UpdateResolution()
    {

		glUseProgram(0);
    }

}
