#pragma once
#include "render/base/framepass.h"
#include "render/resources/shaderobject.h"
#include <memory>

namespace Passes
{
    class LightPass : public Base::FramePass
    {
    public:
        LightPass();
        ~LightPass();

        void Setup();
        void Execute();
        void UpdateResolution();

        unsigned int gPosition, gNormal, gAlbedoSpec, gDepth, rboDepth;
        unsigned int gColor;

    private:

        std::shared_ptr<Resources::ShaderObject> shader;

    };

}
