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
		shader->mod1f("resDownX", 1.f / ((svr->width + 1) / scale));
		shader->mod1f("resDownY", 1.f / ((svr->height + 1) / scale));

		samples = 64;

		//Setup sample offsets
		float dx = 1.0f / svr->width / scale;
		float dy = 1.0f / svr->height / scale;
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
		poissonDisk[0] = vector2D(-0.613392, 0.617481);
		poissonDisk[1] = vector2D(0.170019, -0.040254);
		poissonDisk[2] = vector2D(-0.299417, 0.791925);
		poissonDisk[3] = vector2D(0.645680, 0.493210);
		poissonDisk[4] = vector2D(-0.651784, 0.717887);
		poissonDisk[5] = vector2D(0.421003, 0.027070);
		poissonDisk[6] = vector2D(-0.817194, -0.271096);
		poissonDisk[7] = vector2D(-0.705374, -0.668203);
		poissonDisk[8] = vector2D(0.977050, -0.108615);
		poissonDisk[9] = vector2D(0.063326, 0.142369);
		poissonDisk[10] = vector2D(0.203528, 0.214331);
		poissonDisk[11] = vector2D(-0.667531, 0.326090);
		poissonDisk[12] = vector2D(-0.098422, -0.295755);
		poissonDisk[13] = vector2D(-0.885922, 0.215369);
		poissonDisk[14] = vector2D(0.566637, 0.605213);
		poissonDisk[15] = vector2D(0.039766, -0.396100);
		poissonDisk[16] = vector2D(0.751946, 0.453352);
		poissonDisk[17] = vector2D(0.078707, -0.715323);
		poissonDisk[18] = vector2D(-0.075838, -0.529344);
		poissonDisk[19] = vector2D(0.724479, -0.580798);
		poissonDisk[20] = vector2D(0.222999, -0.215125);
		poissonDisk[21] = vector2D(-0.467574, -0.405438);
		poissonDisk[22] = vector2D(-0.248268, -0.814753);
		poissonDisk[23] = vector2D(0.354411, -0.887570);
		poissonDisk[24] = vector2D(0.175817, 0.382366);
		poissonDisk[25] = vector2D(0.487472, -0.063082);
		poissonDisk[26] = vector2D(-0.084078, 0.898312);
		poissonDisk[27] = vector2D(0.488876, -0.783441);
		poissonDisk[28] = vector2D(0.470016, 0.217933);
		poissonDisk[29] = vector2D(-0.696890, -0.549791);
		poissonDisk[30] = vector2D(-0.149693, 0.605762);
		poissonDisk[31] = vector2D(0.034211, 0.979980);
		poissonDisk[32] = vector2D(0.503098, -0.308878);
		poissonDisk[33] = vector2D(-0.016205, -0.872921);
		poissonDisk[34] = vector2D(0.385784, -0.393902);
		poissonDisk[35] = vector2D(-0.146886, -0.859249);
		poissonDisk[36] = vector2D(0.643361, 0.164098);
		poissonDisk[37] = vector2D(0.634388, -0.049471);
		poissonDisk[38] = vector2D(-0.688894, 0.007843);
		poissonDisk[39] = vector2D(0.464034, -0.188818);
		poissonDisk[40] = vector2D(-0.440840, 0.137486);
		poissonDisk[41] = vector2D(0.364483, 0.511704);
		poissonDisk[42] = vector2D(0.034028, 0.325968);
		poissonDisk[43] = vector2D(0.099094, -0.308023);
		poissonDisk[44] = vector2D(0.693960, -0.366253);
		poissonDisk[45] = vector2D(0.678884, -0.204688);
		poissonDisk[46] = vector2D(0.001801, 0.780328);
		poissonDisk[47] = vector2D(0.145177, -0.898984);
		poissonDisk[48] = vector2D(0.062655, -0.611866);
		poissonDisk[49] = vector2D(0.315226, -0.604297);
		poissonDisk[50] = vector2D(-0.780145, 0.486251);
		poissonDisk[51] = vector2D(-0.371868, 0.882138);
		poissonDisk[52] = vector2D(0.200476, 0.494430);
		poissonDisk[53] = vector2D(-0.494552, -0.711051);
		poissonDisk[54] = vector2D(0.612476, 0.705252);
		poissonDisk[55] = vector2D(-0.578845, -0.768792);
		poissonDisk[56] = vector2D(-0.772454, -0.090976);
		poissonDisk[57] = vector2D(0.504440, 0.372295);
		poissonDisk[58] = vector2D(0.155736, 0.065157);
		poissonDisk[59] = vector2D(0.391522, 0.849605);
		poissonDisk[60] = vector2D(-0.620106, -0.328104);
		poissonDisk[61] = vector2D(0.789239, -0.419965);
		poissonDisk[62] = vector2D(-0.545396, 0.538133);
		poissonDisk[63] = vector2D(-0.178564, -0.596057);

        for (int i = 0; i < samples; ++i)
        {
			poissonDisk[i][0] *= dx;
			poissonDisk[i][1] *= dy;
        }



		shader->modVector2fArray("sampleArray", samples, poissonDisk);
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
		float dx = 1.0f / svr->width / scale;
		float dy = 1.0f / svr->height / scale;

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
		poissonDisk[0] = vector2D(-0.613392, 0.617481);
		poissonDisk[1] = vector2D(0.170019, -0.040254);
		poissonDisk[2] = vector2D(-0.299417, 0.791925);
		poissonDisk[3] = vector2D(0.645680, 0.493210);
		poissonDisk[4] = vector2D(-0.651784, 0.717887);
		poissonDisk[5] = vector2D(0.421003, 0.027070);
		poissonDisk[6] = vector2D(-0.817194, -0.271096);
		poissonDisk[7] = vector2D(-0.705374, -0.668203);
		poissonDisk[8] = vector2D(0.977050, -0.108615);
		poissonDisk[9] = vector2D(0.063326, 0.142369);
		poissonDisk[10] = vector2D(0.203528, 0.214331);
		poissonDisk[11] = vector2D(-0.667531, 0.326090);
		poissonDisk[12] = vector2D(-0.098422, -0.295755);
		poissonDisk[13] = vector2D(-0.885922, 0.215369);
		poissonDisk[14] = vector2D(0.566637, 0.605213);
		poissonDisk[15] = vector2D(0.039766, -0.396100);
		poissonDisk[16] = vector2D(0.751946, 0.453352);
		poissonDisk[17] = vector2D(0.078707, -0.715323);
		poissonDisk[18] = vector2D(-0.075838, -0.529344);
		poissonDisk[19] = vector2D(0.724479, -0.580798);
		poissonDisk[20] = vector2D(0.222999, -0.215125);
		poissonDisk[21] = vector2D(-0.467574, -0.405438);
		poissonDisk[22] = vector2D(-0.248268, -0.814753);
		poissonDisk[23] = vector2D(0.354411, -0.887570);
		poissonDisk[24] = vector2D(0.175817, 0.382366);
		poissonDisk[25] = vector2D(0.487472, -0.063082);
		poissonDisk[26] = vector2D(-0.084078, 0.898312);
		poissonDisk[27] = vector2D(0.488876, -0.783441);
		poissonDisk[28] = vector2D(0.470016, 0.217933);
		poissonDisk[29] = vector2D(-0.696890, -0.549791);
		poissonDisk[30] = vector2D(-0.149693, 0.605762);
		poissonDisk[31] = vector2D(0.034211, 0.979980);
		poissonDisk[32] = vector2D(0.503098, -0.308878);
		poissonDisk[33] = vector2D(-0.016205, -0.872921);
		poissonDisk[34] = vector2D(0.385784, -0.393902);
		poissonDisk[35] = vector2D(-0.146886, -0.859249);
		poissonDisk[36] = vector2D(0.643361, 0.164098);
		poissonDisk[37] = vector2D(0.634388, -0.049471);
		poissonDisk[38] = vector2D(-0.688894, 0.007843);
		poissonDisk[39] = vector2D(0.464034, -0.188818);
		poissonDisk[40] = vector2D(-0.440840, 0.137486);
		poissonDisk[41] = vector2D(0.364483, 0.511704);
		poissonDisk[42] = vector2D(0.034028, 0.325968);
		poissonDisk[43] = vector2D(0.099094, -0.308023);
		poissonDisk[44] = vector2D(0.693960, -0.366253);
		poissonDisk[45] = vector2D(0.678884, -0.204688);
		poissonDisk[46] = vector2D(0.001801, 0.780328);
		poissonDisk[47] = vector2D(0.145177, -0.898984);
		poissonDisk[48] = vector2D(0.062655, -0.611866);
		poissonDisk[49] = vector2D(0.315226, -0.604297);
		poissonDisk[50] = vector2D(-0.780145, 0.486251);
		poissonDisk[51] = vector2D(-0.371868, 0.882138);
		poissonDisk[52] = vector2D(0.200476, 0.494430);
		poissonDisk[53] = vector2D(-0.494552, -0.711051);
		poissonDisk[54] = vector2D(0.612476, 0.705252);
		poissonDisk[55] = vector2D(-0.578845, -0.768792);
		poissonDisk[56] = vector2D(-0.772454, -0.090976);
		poissonDisk[57] = vector2D(0.504440, 0.372295);
		poissonDisk[58] = vector2D(0.155736, 0.065157);
		poissonDisk[59] = vector2D(0.391522, 0.849605);
		poissonDisk[60] = vector2D(-0.620106, -0.328104);
		poissonDisk[61] = vector2D(0.789239, -0.419965);
		poissonDisk[62] = vector2D(-0.545396, 0.538133);
		poissonDisk[63] = vector2D(-0.178564, -0.596057);

		for (int i = 0; i < samples; ++i)
		{
			poissonDisk[i][0] *= dx;
			poissonDisk[i][1] *= dy;
		}

		shader->modVector2fArray("sampleArray", samples, poissonDisk);

		shader->mod1f("resDownX", 1.f / ((svr->width + 1) / scale));
		shader->mod1f("resDownY", 1.f / ((svr->height + 1) / scale));
		glUseProgram(0);
    }

}
