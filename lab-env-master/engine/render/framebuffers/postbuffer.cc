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
		// position buffer
		glGenTextures(1, &fragColor);
		glBindTexture(GL_TEXTURE_2D, fragColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, srv->width, srv->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fragColor, 0);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

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


