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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        Servers::RenderServer::GetInstance()->DrawGBuffer();
        glDepthMask(GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glDisable(GL_BLEND);

        //Render all graphicsproperties
        eManager->OnBeginFrame();
        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);
    }
}
