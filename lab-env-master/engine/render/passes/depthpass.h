#pragma once
#include "render/base/framepass.h"
#include "core/toolkit/userinterface.h"

namespace Passes
{
    class DepthPass : public Base::FramePass
    {
    public:
        DepthPass();

        ~DepthPass();
        void Setup();
        void Execute();

    private:
        GLuint depthBuffer;
    };

}
