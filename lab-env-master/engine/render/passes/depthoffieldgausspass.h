#pragma once
#include "render/base/framepass.h"



namespace Passes
{
    class DofGaussPass : public Base::FramePass
    {
    public:
		DofGaussPass();
        ~DofGaussPass();

        void Setup();
        void Execute();
        void UpdateResolution();

		//Texture
		GLuint downTex;

    private:
        std::shared_ptr<Resources::ShaderObject> downsampleShader;
        std::shared_ptr<Resources::ShaderObject> shader;

		//FBO
		GLuint downFBO;

    };

    
}
