#pragma once
namespace Servers
{
    class RenderServer
    {
        RenderServer();

        static RenderServer* GetInstance();
        RenderServer(const RenderServer&) = delete;
        void operator=(const RenderServer) = delete;


    private:

        static RenderServer* instance;
    };
}