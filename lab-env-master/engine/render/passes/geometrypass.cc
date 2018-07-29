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
        //Could possably be moved out into renderserver..
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Servers::RenderServer::GetInstance()->gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        eManager->OnBeginFrame();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    }
}
