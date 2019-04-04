#pragma once
#include "render/base/framepass.h"



namespace Passes
{
	///Depth of field with gaussian blur
    class DofGaussPass : public Base::FramePass
    {
    public:
		DofGaussPass();
        ~DofGaussPass();

        void Setup();
        void Execute();
        void UpdateResolution();

		//Downscaled render to texture
		GLuint downTex;

    private:
        std::shared_ptr<Resources::ShaderObject> downsampleShader;
        std::shared_ptr<Resources::ShaderObject> gaussX;
        std::shared_ptr<Resources::ShaderObject> gaussY;
        std::shared_ptr<Resources::ShaderObject> composit;

		//FBO
		GLuint downFBO;

    };

    
}
