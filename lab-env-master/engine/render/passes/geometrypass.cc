#include "config.h"
#include "geometrypass.h"
#include "render/managers/entitymanager.h"
#include "render/servers/renderserver.h"

namespace Passes
{

    GeometryPass::GeometryPass()
    {
    }

    GeometryPass::~GeometryPass()
    {
    }

    void GeometryPass::Setup()
    {
        eManager = Managers::EntityManager::GetInstance();
    }

    void GeometryPass::Execute()
    {
        glClearColor(0.f, 0.f, 0.f, 0.0f);
        //Could possably be moved out into renderserver..
        glBindFramebuffer(GL_FRAMEBUFFER, Servers::RenderServer::GetInstance()->gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        eManager->OnBeginFrame();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
