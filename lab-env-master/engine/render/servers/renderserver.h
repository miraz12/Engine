#pragma once
#include "render/base/framepass.h"
#include <vector>
#include "render/passes/geometrypass.h"

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


    private:
        std::vector<Base::FramePass*> passes;
        Passes::GeometryPass* gPass;
        //Passes::LightPass* lPass;
        static RenderServer* instance;
    };
}
