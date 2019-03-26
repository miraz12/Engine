#include "config.h"
#include "gbufferobject.h"
#include <ostream>
#include <iostream>
#include "render/servers/renderserver.h"

namespace FrameBuffers
{

	GBufferObject::GBufferObject(Servers::RenderServer* svr)
	{
		glGenFramebuffers(1, &gBuffer);
		srv = svr;
		BindGBuffer();
		// Out color
		glGenTextures(1, &gColor);
		glBindTexture(GL_TEXTURE_2D, gColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gColor, 0);
		// normal buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, srv->width, srv->height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		// color + specular buffer
		glGenTextures(1, &gAlbedoSpec);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, srv->width, srv->height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
		// depth buffer
		glGenTextures(1, &gDepth);
		glBindTexture(GL_TEXTURE_2D, gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, srv->width, srv->height, 0, GL_RG, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gDepth, 0);
		// position buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, srv->width, srv->height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gPosition, 0);

		// create and attach depth buffer (renderbufferobject)
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, srv->width, srv->height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}


	void GBufferObject::UpdateResolution()
	{
		Servers::RenderServer* srv = Servers::RenderServer::GetInstance();
		int width = srv->width;
		int height = srv->height;
		glBindTexture(GL_TEXTURE_2D, srv->gBuffer->gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, srv->gBuffer->gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, srv->gBuffer->gAlbedoSpec);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, srv->gBuffer->gDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, NULL);

		glBindTexture(GL_TEXTURE_2D, srv->gBuffer->gColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindRenderbuffer(GL_RENDERBUFFER, srv->gBuffer->rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GBufferObject::BindGBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	}

	void GBufferObject::ReadGBuffer()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);

	}

	void GBufferObject::DrawGBuffer()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, gBuffer);

	}
}


