#pragma once
#include "render/base/framepass.h"

namespace Passes
{
    class DofPass : public Base::FramePass
    {
    public:
        DofPass();
        ~DofPass();

        void Setup();
        void Execute();
        void renderQuad();

    private:
        unsigned int colorBuffer;
        std::shared_ptr<Resources::ShaderObject> circleShader;
        std::shared_ptr<Resources::ShaderObject> shader;


    };

    
}
