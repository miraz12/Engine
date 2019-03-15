#pragma once
#include "render/base/framepass.h"

namespace Passes
{
    class DrawPass : public Base::FramePass
    {
    public:
        DrawPass();
        ~DrawPass();

        void Setup();
        void Execute();
        void renderQuad();

    private:
        std::shared_ptr<Resources::ShaderObject> shader;


    };

    
}
