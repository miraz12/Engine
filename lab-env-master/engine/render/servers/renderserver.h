#pragma once
#include "render/base/framepass.h"
#include <vector>
#include "render/passes/geometrypass.h"
#include "render/passes/lightpass.h"
#include "render/skybox/skybox.h"

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

        void UpdateResolution();

        GLuint gBuffer;

    private:

        std::vector<Base::FramePass*> passes;
        Passes::GeometryPass* gPass;
        Passes::LightPass* lPass;
        Skybox::Skybox* skybox;

        static RenderServer* instance;
    };
}
