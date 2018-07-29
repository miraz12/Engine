#pragma once
#include "render/base/framepass.h"
#include <vector>
#include "render/passes/geometrypass.h"
#include "render/passes/lightpass.h"

namespace Servers
{
    class RenderServer
    {
    public:
        RenderServer();
        void Render();
        void Setup();

        static RenderServer* GetInstance();
        RenderServer(const RenderServer&) = delete;
        void operator=(const RenderServer) = delete;

        GLuint gBuffer;
        Passes::LightPass* lPass;

    private:

        std::vector<Base::FramePass*> passes;
        Passes::GeometryPass* gPass;
        static RenderServer* instance;
    };
}
