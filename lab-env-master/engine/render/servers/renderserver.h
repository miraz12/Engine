#pragma once
#include "render/base/framepass.h"
#include <vector>
#include "render/passes/geometrypass.h"
#include "render/passes/lightpass.h"
#include "render/passes/drawpass.h"
#include "render/passes/depthoffieldpass.h"
#include "render/passes/skyboxpass.h"
#include "render/framebuffers/gbufferobject.h"
#include "render/framebuffers/postbuffer.h"
#include "render/passes/depthoffieldgausspass.h"
#include "render/passes/dofcomplex.h"
#include "render/passes/downsamplepass.h"
#include "render/passes/ParticlePass.h"

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
	    void ResolutionUpdated(int w, int h);
	    RenderServer(const RenderServer&) = delete;
        void operator=(const RenderServer) = delete;

        void UpdateResolution(int w, int h);

		Passes::GeometryPass* gPass;


	    FrameBuffers::GBufferObject* gBuffer;
		FrameBuffers::PostBuffer* pBuffer;
		std::vector<Base::FramePass*> passes;

		Passes::LightPass* lPass;
		Passes::DofPass* dofPass;
		Passes::SkyboxPass* skyPass;
		Passes::DrawPass* dPass;
		Passes::DofGaussPass* dGPass;
		Passes::DofComplex* cdPass;
		Passes::DownsamplePass* downPass;
		Passes::ParticlePass* particlePass;


		bool downssample = true;
		int dof_type = 0;

        int width, height;
	    bool resUpdated=false;

		float t, prevt, dt; // time

    private:




        Display::Window* window;
        static RenderServer* instance;

    };
}
