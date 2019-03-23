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

        void Execute();

    private:

        std::shared_ptr<Resources::ShaderObject> shader;

    };

}
