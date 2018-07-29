#include "config.h"
#include "renderserver.h"
#include "render/passes/geometrypass.h"

namespace Servers
{
    RenderServer* RenderServer::instance = 0;

    RenderServer::RenderServer()
    {

    }

    void RenderServer::Render()
    {
        for (int i = 0; i < passes.size(); ++i)
        {
            passes[i]->Execute();
        }
    }

    void RenderServer::Setup()
    {
        gPass = new Passes::GeometryPass();
        gPass->Setup();
        passes.push_back(gPass);
    }


    RenderServer* RenderServer::GetInstance()
    {
        if (instance == 0)
        {
            instance = new RenderServer();
        }
        return instance;
    }

}
