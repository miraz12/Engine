#pragma once
#include "render/base/framepass.h"
#include "core/toolkit/userinterface.h"

namespace Passes
{
    class GeometryPass : public Base::FramePass
    {
    public:
        GeometryPass();

        ~GeometryPass();
        void Setup();
        void Execute();

    private:
        GLuint normalBuffer;
    };

}
