#include "config.h"
#include "lightpass.h"
#include "render/camera.h"
#include "render/managers/lightmanager.h"
#include "render/resources/meshresource.h"
#include "render/servers/renderserver.h"

namespace Passes
{

    LightPass::LightPass() 
    {
        shader = std::make_shared<Resources::ShaderObject>("content/Shader/lightingpass.vs", "content/Shader/lightingpass.fs");

		Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
		int width = svr->width;
		int height = svr->height;

		shader->bind();
		shader->mod1i("gPosition", 0);
		shader->mod1i("gNormal", 1);
		shader->mod1i("gAlbedoSpec", 2);
		shader->mod1i("gDepth", 3);
		shader->mod1i("gColor", 4);
    }

    LightPass::~LightPass()
    {

    }

    void LightPass::Execute()
    {
		Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
        //Bind lighting shader
        this->shader->bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gAlbedoSpec);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, svr->gBuffer->gDepth);



        Managers::LightManager::GetInstance()->Setup(this->shader);

        Display::Camera* cam = Display::Camera::GetInstance();

        shader->mod1f("focalPoint", cam->depth);
        shader->mod1f("focalScale", cam->depthScale);

        shader->mod1f("zNear", cam->zNear);
        shader->mod1f("zFar", cam->zFar);


        //Render quad that cover the whole screen
		RenderQuad();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
