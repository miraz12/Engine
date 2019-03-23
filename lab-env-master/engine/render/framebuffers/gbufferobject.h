#pragma once
#include "GL/glew.h"

namespace Servers {
	class RenderServer;
}

namespace FrameBuffers
{
	class GBufferObject
	{

	public:
		GBufferObject(Servers::RenderServer* svr);
		void UpdateResolution();

		GLuint gBuffer;
		unsigned int gPosition, gNormal, gAlbedoSpec, gDepth, rboDepth;
		unsigned int gColor;
		Servers::RenderServer* srv;


		void BindGBuffer();
		void ReadGBuffer();
		void DrawGBuffer();

	private:
		
	};

}