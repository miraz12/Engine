#include "config.h"
#include "renderserver.h"

namespace Servers
{
    RenderServer* RenderServer::instance = 0;

    RenderServer::RenderServer()
    {

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