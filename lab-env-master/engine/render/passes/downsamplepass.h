#pragma once
#include "render/base/framepass.h"



namespace Passes
{
	///Depth of field with gaussian blur
    class DownsamplePass : public Base::FramePass
    {
    public:
		DownsamplePass();
        ~DownsamplePass();

        void Setup();
        void Execute();
        void UpdateResolution();

		//Downscaled render to texture
		GLuint downTex, downTexX, downTexY;
		//FBO
		GLuint downFBO;

    private:
        std::shared_ptr<Resources::ShaderObject> downsampleShader;



    };

    
}
