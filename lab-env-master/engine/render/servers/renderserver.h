#pragma once
#include "render/base/framepass.h"
#include <vector>
#include "render/passes/geometrypass.h"
#include "render/passes/lightpass.h"
#include "render/skybox/skybox.h"

namespace Display {
    class Window;
}

namespace Servers
{
    class RenderServer
    {
    public:
        RenderServer();
        void Render();
        void Init(Display::Window* window);

        static RenderServer* GetInstance();
        RenderServer(const RenderServer&) = delete;
        void operator=(const RenderServer) = delete;

        void UpdateResolution(int w, int h);
        void BindGBuffer();

        GLuint gBuffer;

        Passes::GeometryPass* getgPass() const
        {
            return gPass;
        }

        Passes::LightPass* getlPass() const
        {
            return lPass;
        }

        int width, height;

    private:
        std::vector<Base::FramePass*> passes;
        Passes::GeometryPass* gPass;
        Passes::LightPass* lPass;
        Skybox::Skybox* skybox;

        Display::Window* window;
        static RenderServer* instance;
    };
}
