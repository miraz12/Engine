#pragma once
#include "GL/glew.h"

namespace Servers {
	class RenderServer;
}

namespace FrameBuffers
{
	class PostBuffer
	{

	public:
		PostBuffer(Servers::RenderServer* svr);
		void UpdateResolution();


		void BindBuffer();
		void ReadBuffer();
		void DrawBuffer();

		//Texture attachements
		GLuint fragColor, depth;

		//Framebuffer object
		GLuint fbo;

	private:

		Servers::RenderServer* srv; 
		
	};

}