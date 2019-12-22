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
		composit = std::make_shared<Resources::ShaderObject>("content/Shader/composit.vs", "content/Shader/composit.fs");

		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		float scale = 1.0f;
		if (svr->downssample)
		{
			scale = 2.0f;
		}

		shader->bind();
		shader->mod1i("gColor", 0); 
		shader->mod1i("gDepth", 1);
		shader->mod1f("resDownX", 1.f / ((svr->width + scale - 1) / scale));
		shader->mod1f("resDownY", 1.f / ((svr->height + scale - 1) / scale));

		samples = 64;

		//Setup sample offsets
		float dx = 1.f / ((svr->width + scale - 1) / scale);
		float dy = 1.f / ((svr->height + scale - 1) / scale);
		/*	
		vector2D v[27];
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
		*/

		vector2D poissonDisk[64];
		poissonDisk[0] = vector2D(-0.613392f, 0.617481f);
		poissonDisk[1] = vector2D(0.170019f, -0.040254f);
		poissonDisk[2] = vector2D(-0.299417f, 0.791925f);
		poissonDisk[3] = vector2D(0.645680f, 0.493210f);
		poissonDisk[4] = vector2D(-0.651784f, 0.717887f);
		poissonDisk[5] = vector2D(0.421003f, 0.027070f);
		poissonDisk[6] = vector2D(-0.817194f, -0.271096f);
		poissonDisk[7] = vector2D(-0.705374f, -0.668203f);
		poissonDisk[8] = vector2D(0.977050f, -0.108615f);
		poissonDisk[9] = vector2D(0.063326f, 0.142369f);
		poissonDisk[10] = vector2D(0.203528f, 0.214331f);
		poissonDisk[11] = vector2D(-0.667531f, 0.326090f);
		poissonDisk[12] = vector2D(-0.098422f, -0.295755f);
		poissonDisk[13] = vector2D(-0.885922f, 0.215369f);
		poissonDisk[14] = vector2D(0.566637f, 0.605213f);
		poissonDisk[15] = vector2D(0.039766f, -0.396100f);
		poissonDisk[16] = vector2D(0.751946f, 0.453352f);
		poissonDisk[17] = vector2D(0.078707f, -0.715323f);
		poissonDisk[18] = vector2D(-0.075838f, -0.529344f);
		poissonDisk[19] = vector2D(0.724479f, -0.580798f);
		poissonDisk[20] = vector2D(0.222999f, -0.215125f);
		poissonDisk[21] = vector2D(-0.467574f, -0.405438f);
		poissonDisk[22] = vector2D(-0.248268f, -0.814753f);
		poissonDisk[23] = vector2D(0.354411f, -0.887570f);
		poissonDisk[24] = vector2D(0.175817f, 0.382366f);
		poissonDisk[25] = vector2D(0.487472f, -0.063082f);
		poissonDisk[26] = vector2D(-0.084078f, 0.898312f);
		poissonDisk[27] = vector2D(0.488876f, -0.783441f);
		poissonDisk[28] = vector2D(0.470016f, 0.217933f);
		poissonDisk[29] = vector2D(-0.696890f, -0.549791f);
		poissonDisk[30] = vector2D(-0.149693f, 0.605762f);
		poissonDisk[31] = vector2D(0.034211f, 0.979980f);
		poissonDisk[32] = vector2D(0.503098f, -0.308878f);
		poissonDisk[33] = vector2D(-0.016205f, -0.872921f);
		poissonDisk[34] = vector2D(0.385784f, -0.393902f);
		poissonDisk[35] = vector2D(-0.146886f, -0.859249f);
		poissonDisk[36] = vector2D(0.643361f, 0.164098f);
		poissonDisk[37] = vector2D(0.634388f, -0.049471f);
		poissonDisk[38] = vector2D(-0.688894f, 0.007843f);
		poissonDisk[39] = vector2D(0.464034f, -0.188818f);
		poissonDisk[40] = vector2D(-0.440840f, 0.137486f);
		poissonDisk[41] = vector2D(0.364483f, 0.511704f);
		poissonDisk[42] = vector2D(0.034028f, 0.325968f);
		poissonDisk[43] = vector2D(0.099094f, -0.308023f);
		poissonDisk[44] = vector2D(0.693960f, -0.366253f);
		poissonDisk[45] = vector2D(0.678884f, -0.204688f);
		poissonDisk[46] = vector2D(0.001801f, 0.780328f);
		poissonDisk[47] = vector2D(0.145177f, -0.898984f);
		poissonDisk[48] = vector2D(0.062655f, -0.611866f);
		poissonDisk[49] = vector2D(0.315226f, -0.604297f);
		poissonDisk[50] = vector2D(-0.780145f, 0.486251f);
		poissonDisk[51] = vector2D(-0.371868f, 0.882138f);
		poissonDisk[52] = vector2D(0.200476f, 0.494430f);
		poissonDisk[53] = vector2D(-0.494552f, -0.711051f);
		poissonDisk[54] = vector2D(0.612476f, 0.705252f);
		poissonDisk[55] = vector2D(-0.578845f, -0.768792f);
		poissonDisk[56] = vector2D(-0.772454f, -0.090976f);
		poissonDisk[57] = vector2D(0.504440f, 0.372295f);
		poissonDisk[58] = vector2D(0.155736f, 0.065157f);
		poissonDisk[59] = vector2D(0.391522f, 0.849605f);
		poissonDisk[60] = vector2D(-0.620106f, -0.328104f);
		poissonDisk[61] = vector2D(0.789239f, -0.419965f);
		poissonDisk[62] = vector2D(-0.545396f, 0.538133f);
		poissonDisk[63] = vector2D(-0.178564f, -0.596057f);

        for (int i = 0; i < 64; ++i)
        {
			poissonDisk[i][0] *= dx;
			poissonDisk[i][1] *= dy;
        }



		shader->modVector2fArray("sampleArray", 64, poissonDisk);
		shader->mod1i("samples", samples);

		composit->bind();
		composit->mod1i("inFullRes", 0);
		composit->mod1i("inDownSampled", 1);

		glUseProgram(0);
    }

    DofPass::~DofPass()
    {
    }

    void DofPass::Execute()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		if (svr->dof_type != 1)
			return;

		this->shader->bind();

		//Bind lighting shader
		glActiveTexture(GL_TEXTURE0);
		if (svr->downssample)
			glBindTexture(GL_TEXTURE_2D, svr->downPass->downTex);
		else
			glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor);
		glActiveTexture(GL_TEXTURE1);
		if (svr->downssample)
			glBindTexture(GL_TEXTURE_2D, svr->downPass->downDepth);
		else
			glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gDepth);
		//Render quad that covers the whole screen
		RenderQuad();

		//Dof---------------------------
		if (svr->downssample)
		{
			svr->pBuffer->DrawBuffer();
		}
		composit->bind();
		Display::Camera* cam = Display::Camera::GetInstance();


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, svr->pBuffer->fragColor); //Fullscale color
		glActiveTexture(GL_TEXTURE1);
		if (svr->downssample)
			glBindTexture(GL_TEXTURE_2D, svr->downPass->downTexX); //Downscaled blurred 
		else
			glBindTexture(GL_TEXTURE_2D, svr->pBuffer->postColorX); 
		RenderQuad();

		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
    }

	void DofPass::UpdateResolution()
    {
		Servers::RenderServer* svr;
		svr = Servers::RenderServer::GetInstance();

		shader->bind();

		float scale = 1.0f;
		if (svr->downssample)
		{
			scale = 2.0f;
		}

		//Setup sample offsets
		float dx = 1.f / ((svr->width + scale - 1) / scale);
		float dy = 1.f / ((svr->height + scale - 1) / scale);

		/*vector2D v[12];
		//Setup sample offsets
		float dx = 1.0f / svr->width / scale;
		float dy = 1.0f / svr->height / scale;
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
		shader->modVector2fArray("sampleArray", 12, v);*/

		vector2D poissonDisk[64];
		poissonDisk[0] = vector2D(-0.613392f, 0.617481f);
		poissonDisk[1] = vector2D(0.170019f, -0.040254f);
		poissonDisk[2] = vector2D(-0.299417f, 0.791925f);
		poissonDisk[3] = vector2D(0.645680f, 0.493210f);
		poissonDisk[4] = vector2D(-0.651784f, 0.717887f);
		poissonDisk[5] = vector2D(0.421003f, 0.027070f);
		poissonDisk[6] = vector2D(-0.817194f, -0.271096f);
		poissonDisk[7] = vector2D(-0.705374f, -0.668203f);
		poissonDisk[8] = vector2D(0.977050f, -0.108615f);
		poissonDisk[9] = vector2D(0.063326f, 0.142369f);
		poissonDisk[10] = vector2D(0.203528f, 0.214331f);
		poissonDisk[11] = vector2D(-0.667531f, 0.326090f);
		poissonDisk[12] = vector2D(-0.098422f, -0.295755f);
		poissonDisk[13] = vector2D(-0.885922f, 0.215369f);
		poissonDisk[14] = vector2D(0.566637f, 0.605213f);
		poissonDisk[15] = vector2D(0.039766f, -0.396100f);
		poissonDisk[16] = vector2D(0.751946f, 0.453352f);
		poissonDisk[17] = vector2D(0.078707f, -0.715323f);
		poissonDisk[18] = vector2D(-0.075838f, -0.529344f);
		poissonDisk[19] = vector2D(0.724479f, -0.580798f);
		poissonDisk[20] = vector2D(0.222999f, -0.215125f);
		poissonDisk[21] = vector2D(-0.467574f, -0.405438f);
		poissonDisk[22] = vector2D(-0.248268f, -0.814753f);
		poissonDisk[23] = vector2D(0.354411f, -0.887570f);
		poissonDisk[24] = vector2D(0.175817f, 0.382366f);
		poissonDisk[25] = vector2D(0.487472f, -0.063082f);
		poissonDisk[26] = vector2D(-0.084078f, 0.898312f);
		poissonDisk[27] = vector2D(0.488876f, -0.783441f);
		poissonDisk[28] = vector2D(0.470016f, 0.217933f);
		poissonDisk[29] = vector2D(-0.696890f, -0.549791f);
		poissonDisk[30] = vector2D(-0.149693f, 0.605762f);
		poissonDisk[31] = vector2D(0.034211f, 0.979980f);
		poissonDisk[32] = vector2D(0.503098f, -0.308878f);
		poissonDisk[33] = vector2D(-0.016205f, -0.872921f);
		poissonDisk[34] = vector2D(0.385784f, -0.393902f);
		poissonDisk[35] = vector2D(-0.146886f, -0.859249f);
		poissonDisk[36] = vector2D(0.643361f, 0.164098f);
		poissonDisk[37] = vector2D(0.634388f, -0.049471f);
		poissonDisk[38] = vector2D(-0.688894f, 0.007843f);
		poissonDisk[39] = vector2D(0.464034f, -0.188818f);
		poissonDisk[40] = vector2D(-0.440840f, 0.137486f);
		poissonDisk[41] = vector2D(0.364483f, 0.511704f);
		poissonDisk[42] = vector2D(0.034028f, 0.325968f);
		poissonDisk[43] = vector2D(0.099094f, -0.308023f);
		poissonDisk[44] = vector2D(0.693960f, -0.366253f);
		poissonDisk[45] = vector2D(0.678884f, -0.204688f);
		poissonDisk[46] = vector2D(0.001801f, 0.780328f);
		poissonDisk[47] = vector2D(0.145177f, -0.898984f);
		poissonDisk[48] = vector2D(0.062655f, -0.611866f);
		poissonDisk[49] = vector2D(0.315226f, -0.604297f);
		poissonDisk[50] = vector2D(-0.780145f, 0.486251f);
		poissonDisk[51] = vector2D(-0.371868f, 0.882138f);
		poissonDisk[52] = vector2D(0.200476f, 0.494430f);
		poissonDisk[53] = vector2D(-0.494552f, -0.711051f);
		poissonDisk[54] = vector2D(0.612476f, 0.705252f);
		poissonDisk[55] = vector2D(-0.578845f, -0.768792f);
		poissonDisk[56] = vector2D(-0.772454f, -0.090976f);
		poissonDisk[57] = vector2D(0.504440f, 0.372295f);
		poissonDisk[58] = vector2D(0.155736f, 0.065157f);
		poissonDisk[59] = vector2D(0.391522f, 0.849605f);
		poissonDisk[60] = vector2D(-0.620106f, -0.328104f);
		poissonDisk[61] = vector2D(0.789239f, -0.419965f);
		poissonDisk[62] = vector2D(-0.545396f, 0.538133f);
		poissonDisk[63] = vector2D(-0.178564f, -0.596057f);

		for (int i = 0; i < 64; ++i)
		{
			poissonDisk[i][0] *= dx;
			poissonDisk[i][1] *= dy;
		}

		shader->modVector2fArray("sampleArray", 64, poissonDisk);

		shader->mod1f("resDownX", 1.f / ((svr->width + scale - 1) / scale));
		shader->mod1f("resDownY", 1.f / ((svr->height + scale - 1) / scale));
		glUseProgram(0);
    }

}
