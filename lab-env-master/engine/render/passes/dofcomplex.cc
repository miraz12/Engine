#include "config.h"
#include "dofcomplex.h"
#include "render/servers/renderserver.h"


namespace Passes
{

	DofComplex::DofComplex()
    {
		svr = Servers::RenderServer::GetInstance();

		xpass = std::make_shared<Resources::ShaderObject>("content/Shader/complexdof_x.vs", "content/Shader/complexdof_x.fs");
		ypass = std::make_shared<Resources::ShaderObject>("content/Shader/complexdof_y.vs", "content/Shader/complexdof_y.fs");
		composit = std::make_shared<Resources::ShaderObject>("content/Shader/composit.vs", "content/Shader/composit.fs");

		float scale = 1.0f;
		if (svr->downssample)
		{
			scale = 2.0f;
		}
		//Setup sample offsets
		float dx = 1.f / ((svr->width + scale - 1) / scale);
		float dy = 1.f / ((svr->height + scale - 1) / scale);

		xpass->bind();
		xpass->mod1f("pixelSizeX", dx);
		xpass->mod1f("pixelSizeY", dy);
		
		xpass->mod1i("inFullRes", 0);

		vector2D ve1;
		vector2D ve2;
		vector2D v1[17];
		vector2D v2[17];

		//static const float2 Kernel0Weights_RealX_ImY = float2(5.268909, -0.886528);
		//ve1 = vector2D(5.268909f, -0.886528f); //Weights
		//v1[0] = vector2D(-0.001243f, 0.0229809f);
		//v1[1] = vector2D(0.009042f, 0.026677f);
		//v1[2] = vector2D(0.020492f, 0.026577f);
		//v1[3] = vector2D(0.031342f, 0.023078f);
		//v1[4] = vector2D(0.040364f, 0.017362f);
		//v1[5] = vector2D(0.047031f, 0.010937f);
		//v1[6] = vector2D(0.051384f, 0.005236f);
		//v1[7] = vector2D(0.053764f, 0.001365f);
		//v1[8] = vector2D(0.054511f, 0.000000f);
		//v1[9] = vector2D(0.053764f, 0.001365f);
		//v1[10] = vector2D(0.051384f, 0.005236f);
		//v1[11] = vector2D(0.047031f, 0.010937f);
		//v1[12] = vector2D(0.040364f, 0.017362f);
		//v1[13] = vector2D(0.031342f, 0.023078f);
		//v1[14] = vector2D(0.020492f, 0.026577f);
		//v1[15] = vector2D(0.009042f, 0.026677f);
		//v1[16] = vector2D(-0.001243f, 0.0229809f);

		//Component 1
		ve1 = vector2D(0.411259f, -0.548794f); //Weights
		v1[0] = vector2D(0.014096f, -0.022658f);
		v1[1] = vector2D(-0.020612f, -0.025574f);
		v1[2] = vector2D(-0.038708f, 0.006957f);
		v1[3] = vector2D(-0.021449f, 0.040468f);
		v1[4] = vector2D(0.013015f, 0.050223f);
		v1[5] = vector2D(0.042178f, 0.038585f);
		v1[6] = vector2D(0.057972f, 0.019812f);
		v1[7] = vector2D(0.063647f, 0.005252f);
		v1[8] = vector2D(0.064754f, 0.000000f);
		v1[9] = vector2D(0.063647f, 0.005252f);
		v1[10] = vector2D(0.057972f, 0.019812f);
		v1[11] = vector2D(0.042178f, 0.038585f);
		v1[12] = vector2D(0.013015f, 0.050223f);
		v1[13] = vector2D(-0.021449f, 0.040468f);
		v1[14] = vector2D(-0.038708f, 0.006957f);
		v1[15] = vector2D(-0.020612f, -0.025574f);
		v1[16] = vector2D(0.014096f, -0.022658f);


		//Component 2 
		ve2 = vector2D(0.513282f, 4.561110f);; //Weights
		v2[0] = vector2D(0.000115f, 0.009116f);
		v2[1] = vector2D(0.005324f, 0.013416f);
		v2[2] = vector2D(0.013753f, 0.016519f);
		v2[3] = vector2D(0.024700f, 0.017215f);
		v2[4] = vector2D(0.036693f, 0.015064f);
		v2[5] = vector2D(0.047976f, 0.010684f);
		v2[6] = vector2D(0.057015f, 0.005570f);
		v2[7] = vector2D(0.062782f, 0.001529f);
		v2[8] = vector2D(0.064754f, 0.000000f);
		v2[9] = vector2D(0.062782f, 0.001529f);
		v2[10] = vector2D(0.057015f, 0.005570f);
		v2[11] = vector2D(0.047976f, 0.010684f);
		v2[12] = vector2D(0.036693f, 0.015064f);
		v2[13] = vector2D(0.024700f, 0.017215f);
		v2[14] = vector2D(0.013753f, 0.016519f);
		v2[15] = vector2D(0.005324f, 0.013416f);
		v2[16] = vector2D(0.000115f, 0.009116f);

		xpass->modVector2fArray("kernelArray0", 17, v1);
		xpass->modVector2fArray("kernelArray1", 17, v2);
		
		ypass->bind();
		ypass->mod1i("inFullRes", 0);
		ypass->mod1i("colorOutRed", 1);
		ypass->mod1i("colorOutGreen", 2);
		ypass->mod1i("colorOutBlue", 3);

		ypass->mod1f("pixelSizeX", dx);
		ypass->mod1f("pixelSizeY", dy);

		ypass->modVector2fArray("kernelArray0", 17, v1);
		ypass->modVector2fArray("kernelArray1", 17, v2);
		ypass->modVector2f("kernellWeights0", ve1);
		ypass->modVector2f("kernellWeights1", ve2);

		composit->bind();
		composit->mod1i("inFullRes", 0);
		composit->mod1i("inDownSampled", 1);


		GLsizei texW = (GLsizei)(svr->width / scale);
		GLsizei texH = (GLsizei)(svr->height / scale);

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// red texture
		glGenTextures(1, &colorOutRed);
		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorOutRed, 0);
		//Blue texture
		glGenTextures(1, &colorOutGreen);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorOutGreen, 0);
		//Green texture 
		glGenTextures(1, &colorOutBlue);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorOutBlue, 0);
		//final color texture 
		glGenTextures(1, &fragColor);
		glBindTexture(GL_TEXTURE_2D, fragColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, fragColor, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		glDrawBuffers(4, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	DofComplex::~DofComplex()
    {
		//Emtpy
    }


    void DofComplex::Execute()
    {

		if (svr->dof_type != 3)
			return;

		//Xpass screen--------------
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		this->xpass->bind();

		glActiveTexture(GL_TEXTURE0);
		if (svr->downssample)
			glBindTexture(GL_TEXTURE_2D, svr->downPass->downTex);
		else
			glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		RenderQuad();

		//Ypass
		this->ypass->bind();
		glActiveTexture(GL_TEXTURE0);
		if (svr->downssample)
			glBindTexture(GL_TEXTURE_2D, svr->downPass->downTex);
		else
			glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);
		RenderQuad();

		//composit 
		svr->pBuffer->DrawBuffer();
		composit->bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor); //Fullscale color
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fragColor); //Downscaled blurred color
		RenderQuad();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
    }

	void DofComplex::UpdateResolution()
	{

		float scale = 1.0f;
		if (svr->downssample)
		{
			scale = 2.0f;
		}

		//Setup sample offsets
		float dx = 1.f / ((svr->width + scale - 1) / scale);
		float dy = 1.f / ((svr->height + scale - 1) / scale);


		xpass->bind();
		xpass->mod1f("pixelSizeX", dx);
		xpass->mod1f("pixelSizeY", dy);
		
		ypass->bind();
		ypass->mod1f("pixelSizeX", dx);
		ypass->mod1f("pixelSizeY", dy);

		GLsizei texW = (GLsizei)(svr->width / scale);
		GLsizei texH = (GLsizei)(svr->height / scale);


		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, fragColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, texW, texH, 0, GL_RGBA, GL_FLOAT, NULL);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

	}

}
