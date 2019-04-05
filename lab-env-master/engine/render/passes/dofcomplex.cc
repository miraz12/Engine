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

		//Setup sample offsets
		float dx = 0.5f / ((svr->width));
		float dy = 0.5f / ((svr->height));

		xpass->bind();
		xpass->mod1i("nrComp", comp);
		xpass->mod1f("pixelSizeX", dx);
		xpass->mod1f("pixelSizeY", dy);
		
		xpass->mod1i("inFullRes", 0);

		vector2D ve1;
		vector2D ve2;
		vector2D v1[17];
		vector2D v2[17];
		if (comp == 1) //NOTE: No need for the zw components as they can be calculated with the weights of the real and imaginary components
		{
			//Component 1
			ve1 = vector2D(5.268909, -0.886528);
			v1[0] = vector2D(0.028949, -0.017701);
			v1[1] = vector2D(0.028133, -0.012569);
			v1[2] = vector2D(0.027004, -0.008611);
			v1[3] = vector2D(0.025805, -0.005618);
			v1[4] = vector2D(0.024691, -0.003409);
			v1[5] = vector2D(0.023758, -0.001837);
			v1[6] = vector2D(0.023062, -0.000791);
			v1[7] = vector2D(0.022634, -0.000194);
			v1[8] = vector2D(0.022490, -0.000000);
			v1[9] = vector2D(0.022634, -0.000194);
			v1[10] = vector2D(0.023062, -0.000791);
			v1[11] = vector2D(0.023758, -0.001837);
			v1[12] = vector2D(0.024691, -0.003409);
			v1[13] = vector2D(0.025805, -0.005618);
			v1[14] = vector2D(0.027004, -0.008611);
			v1[15] = vector2D(0.028133, -0.012569);
			v1[16] = vector2D(0.028949, -0.017701);
		}
		else if (comp == 2)
		{
			//Component 1
			ve1 = vector2D(0.411259, -0.548794); //Weights
			v1[0] = vector2D(0.014096, -0.022658);
			v1[1] = vector2D(-0.020612, -0.025574);
			v1[2] = vector2D(-0.038708, 0.006957);
			v1[3] = vector2D(-0.021449, 0.040468);
			v1[4] = vector2D(0.013015, 0.050223);
			v1[5] = vector2D(0.042178, 0.038585);
			v1[6] = vector2D(0.057972, 0.019812);
			v1[7] = vector2D(0.063647, 0.005252);
			v1[8] = vector2D(0.064754, 0.000000);
			v1[9] = vector2D(0.063647, 0.005252);
			v1[10] = vector2D(0.057972, 0.019812);
			v1[11] = vector2D(0.042178, 0.038585);
			v1[12] = vector2D(0.013015, 0.050223);
			v1[13] = vector2D(-0.021449, 0.040468);
			v1[14] = vector2D(-0.038708, 0.006957);
			v1[15] = vector2D(-0.020612, -0.025574);
			v1[16] = vector2D(0.014096, -0.022658);

			//Component 2 
			ve2 = vector2D(0.513282, 4.561110);; //Weights
			v2[0] = vector2D(0.000115, 0.009116);
			v2[1] = vector2D(0.005324, 0.013416);
			v2[2] = vector2D(0.013753, 0.016519);
			v2[3] = vector2D(0.024700, 0.017215);
			v2[4] = vector2D(0.036693, 0.015064);
			v2[5] = vector2D(0.047976, 0.010684);
			v2[6] = vector2D(0.057015, 0.005570);
			v2[7] = vector2D(0.062782, 0.001529);
			v2[8] = vector2D(0.064754, 0.000000);
			v2[9] = vector2D(0.062782, 0.001529);
			v2[10] = vector2D(0.057015, 0.005570);
			v2[11] = vector2D(0.047976, 0.010684);
			v2[12] = vector2D(0.036693, 0.015064);
			v2[13] = vector2D(0.024700, 0.017215);
			v2[14] = vector2D(0.013753, 0.016519);
			v2[15] = vector2D(0.005324, 0.013416);
			v2[16] = vector2D(0.000115, 0.009116);
		}

		xpass->modVector2fArray("kernelArray0", 17, v1);
		xpass->modVector2fArray("kernelArray1", 17, v2);

		
		ypass->bind();
		ypass->mod1i("inFullRes", 0);
		ypass->mod1i("colorOutRed", 1);
		ypass->mod1i("colorOutGreen", 2);
		ypass->mod1i("colorOutBlue", 3);

		ypass->mod1i("nrComp", comp);
		ypass->mod1f("pixelSizeX", dx);
		ypass->mod1f("pixelSizeY", dy);

		ypass->modVector2fArray("kernelArray0", 17, v1);
		ypass->modVector2fArray("kernelArray1", 17, v2);
		ypass->modVector2f("kernellWeights0", ve1);
		ypass->modVector2f("kernellWeights1", ve2);

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		// red texture
		glGenTextures(1, &colorOutRed);
		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Servers::RenderServer::GetInstance()->width, Servers::RenderServer::GetInstance()->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorOutRed, 0);
		//Blue texture
		glGenTextures(1, &colorOutGreen);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Servers::RenderServer::GetInstance()->width, Servers::RenderServer::GetInstance()->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorOutGreen, 0);
		//Green texture 
		glGenTextures(1, &colorOutBlue);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Servers::RenderServer::GetInstance()->width, Servers::RenderServer::GetInstance()->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorOutBlue, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);

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
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor); //maybe downsample?
		RenderQuad();

		//Ypass
		svr->pBuffer->DrawBuffer();
		this->ypass->bind();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);
		RenderQuad();


    }

	void DofComplex::UpdateResolution()
	{

		float dx = 1.f / ((svr->width));
		float dy = 1.f / ((svr->height));

		xpass->bind();
		xpass->mod1f("pixelSizeX", dx);
		xpass->mod1f("pixelSizeY", dy);
		
		ypass->bind();
		ypass->mod1f("pixelSizeX", dx);
		ypass->mod1f("pixelSizeY", dy);

		glBindTexture(GL_TEXTURE_2D, colorOutRed);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, svr->width, svr->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorOutGreen);	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, svr->width, svr->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, colorOutBlue);		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, svr->width, svr->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glUseProgram(0);

	}

	void DofComplex::ChangeNumberOfComponents()
	{
		vector2D ve1;
		vector2D ve2;
		vector2D v1[17];
		vector2D v2[17];
		if (comp == 1) //NOTE: No need for the zw components as they can be calculated with the weights of the real and imaginary components
		{
			//Component 1
			ve1 = vector2D(5.268909, -0.886528);
			v1[0] = vector2D(0.028949, -0.017701);
			v1[1] = vector2D(0.028133, -0.012569);
			v1[2] = vector2D(0.027004, -0.008611);
			v1[3] = vector2D(0.025805, -0.005618);
			v1[4] = vector2D(0.024691, -0.003409);
			v1[5] = vector2D(0.023758, -0.001837);
			v1[6] = vector2D(0.023062, -0.000791);
			v1[7] = vector2D(0.022634, -0.000194);
			v1[8] = vector2D(0.022490, -0.000000);
			v1[9] = vector2D(0.022634, -0.000194);
			v1[10] = vector2D(0.023062, -0.000791);
			v1[11] = vector2D(0.023758, -0.001837);
			v1[12] = vector2D(0.024691, -0.003409);
			v1[13] = vector2D(0.025805, -0.005618);
			v1[14] = vector2D(0.027004, -0.008611);
			v1[15] = vector2D(0.028133, -0.012569);
			v1[16] = vector2D(0.028949, -0.017701);
		}
		else if (comp == 2)
		{
			//Component 1
			ve1 = vector2D(0.411259, -0.548794); //Weights
			v1[0] = vector2D(0.014096, -0.022658);
			v1[1] = vector2D(-0.020612, -0.025574);
			v1[2] = vector2D(-0.038708, 0.006957);
			v1[3] = vector2D(-0.021449, 0.040468);
			v1[4] = vector2D(0.013015, 0.050223);
			v1[5] = vector2D(0.042178, 0.038585);
			v1[6] = vector2D(0.057972, 0.019812);
			v1[7] = vector2D(0.063647, 0.005252);
			v1[8] = vector2D(0.064754, 0.000000);
			v1[9] = vector2D(0.063647, 0.005252);
			v1[10] = vector2D(0.057972, 0.019812);
			v1[11] = vector2D(0.042178, 0.038585);
			v1[12] = vector2D(0.013015, 0.050223);
			v1[13] = vector2D(-0.021449, 0.040468);
			v1[14] = vector2D(-0.038708, 0.006957);
			v1[15] = vector2D(-0.020612, -0.025574);
			v1[16] = vector2D(0.014096, -0.022658);

			//Component 2 
			ve2 = vector2D(0.513282, 4.561110);; //Weights
			v2[0] = vector2D(0.000115, 0.009116);
			v2[1] = vector2D(0.005324, 0.013416);
			v2[2] = vector2D(0.013753, 0.016519);
			v2[3] = vector2D(0.024700, 0.017215);
			v2[4] = vector2D(0.036693, 0.015064);
			v2[5] = vector2D(0.047976, 0.010684);
			v2[6] = vector2D(0.057015, 0.005570);
			v2[7] = vector2D(0.062782, 0.001529);
			v2[8] = vector2D(0.064754, 0.000000);
			v2[9] = vector2D(0.062782, 0.001529);
			v2[10] = vector2D(0.057015, 0.005570);
			v2[11] = vector2D(0.047976, 0.010684);
			v2[12] = vector2D(0.036693, 0.015064);
			v2[13] = vector2D(0.024700, 0.017215);
			v2[14] = vector2D(0.013753, 0.016519);
			v2[15] = vector2D(0.005324, 0.013416);
			v2[16] = vector2D(0.000115, 0.009116);
		}


		xpass->bind();
		xpass->mod1i("nrComp", comp);
		xpass->modVector2fArray("kernelArray0", 17, v1);
		xpass->modVector2fArray("kernelArray1", 17, v2);

		ypass->bind();
		ypass->mod1i("nrComp", comp);
		ypass->modVector2fArray("kernelArray0", 17, v1);
		ypass->modVector2fArray("kernelArray1", 17, v2);
		ypass->modVector2f("kernellWeights0", ve1);
		ypass->modVector2f("kernellWeights1", ve2);

		glUseProgram(0);
	}
}
