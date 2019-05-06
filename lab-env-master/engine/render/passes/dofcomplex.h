#pragma once
#include "render/base/framepass.h"

namespace Servers
{
	class RenderServer;
}


namespace Passes
{
    class DofComplex : public Base::FramePass
    {
    public:
		DofComplex();
        ~DofComplex();
        void Execute();
		void UpdateResolution();

		GLuint colorOutRed, colorOutBlue, colorOutGreen, fragColor;


    private:
		std::shared_ptr<Resources::ShaderObject> xpass;
		std::shared_ptr<Resources::ShaderObject> ypass;
		std::shared_ptr<Resources::ShaderObject> composit;

		//FBO
		GLuint fbo;

		Servers::RenderServer* svr;

    };

}
