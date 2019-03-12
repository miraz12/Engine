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
        void renderQuad();
        void UpdateResolution();
        unsigned int gPosition, gNormal, gAlbedoSpec, gDepth;
    private:

        std::shared_ptr<Resources::ShaderObject> shader;

    };

}
