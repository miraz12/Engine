#pragma once
#include "render/base/framepass.h"



namespace Passes
{
    class DofPass : public Base::FramePass
    {
    public:
        DofPass();
        ~DofPass();

        void Execute();
        void UpdateResolution();

    private:
        unsigned int colorBuffer;
        std::shared_ptr<Resources::ShaderObject> circleShader;
        std::shared_ptr<Resources::ShaderObject> shader;
        std::shared_ptr<Resources::ShaderObject> composit;


    };

    
}
