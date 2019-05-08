#include "config.h"
#include <ostream>
#include <iostream>
#include "render/servers/renderserver.h"
#include "postbuffer.h"

namespace FrameBuffers
{

	PostBuffer::PostBuffer(Servers::RenderServer* svr)
	{
		glGenFramebuffers(1, &fbo);
		srv = svr;
		BindBuffer();
		// light color buffer
		glGenTextures(1, &fragColor);
		glBindTexture(GL_TEXTURE_2D, fragColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fragColor, 0);
		// post color buffer
		glGenTextures(1, &postColorX);
		glBindTexture(GL_TEXTURE_2D, postColorX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, postColorX, 0);
		// post color buffer
		glGenTextures(1, &postColorY);
		glBindTexture(GL_TEXTURE_2D, postColorY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, postColorY, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(3, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}


	void PostBuffer::UpdateResolution()
	{
		glBindTexture(GL_TEXTURE_2D, fragColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, postColorX);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, postColorY);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void PostBuffer::BindBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	}

	void PostBuffer::ReadBuffer()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

	}

	void PostBuffer::DrawBuffer()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	}
}


