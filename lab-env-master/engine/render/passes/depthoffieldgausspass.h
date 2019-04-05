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

        void Execute();
        void UpdateResolution();


    private:
        std::shared_ptr<Resources::ShaderObject> gaussX;
        std::shared_ptr<Resources::ShaderObject> gaussY;
        std::shared_ptr<Resources::ShaderObject> composit;


    };

    
}
