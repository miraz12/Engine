#include "config.h"
#include "depthoffieldpass.h"
#include "render/servers/renderserver.h"
#include "render/camera.h"

namespace Passes
{
    DofPass::DofPass()
    {
        shader = std::make_shared<Resources::ShaderObject>("content/Shader/dofpass.vs", "content/Shader/dofpass.fs");
		circleShader = std::make_shared<Resources::ShaderObject>("content/Shader/dofpass2.vs", "content/Shader/dofpass2.fs");
    }

    DofPass::~DofPass()
    {
    }

    void DofPass::Setup()
    {
		circleShader->bind();
		circleShader->mod1i("gColor", 4); //0:position, 1:normal, 2:albedoSpec, 3:depth, 4:Fragcolor 
		circleShader->mod1i("gDepth", 3);

        shader->bind();
        shader->mod1i("gColor", 4); //0:position, 1:normal, 2:albedoSpec, 3:depth, 4:Fragcolor 
        shader->mod1i("gDepth", 3); 
        glUseProgram(0);
    }

    void DofPass::Execute()
    {
		Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
        //Bind dof shader that calculates coc size and saves it in alpha of each pixel.
        this->circleShader->bind();

		circleShader->mod1f("inFocusPoint", Display::Camera::GetInstance()->depth);
		circleShader->mod1f("inFocusScale", Display::Camera::GetInstance()->depthScale);

        //Bind lighting shader
        this->circleShader->bind();
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gDepth);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gColor);

        //Render quad that covers the whole screen
		RenderQuad();

		this->shader->bind();

		Display::Camera* cam =  Display::Camera::GetInstance();
		shader->mod1f("inFocusPoint", cam->depth);
		shader->mod1f("inFocusScale", cam->depthScale);

		shader->mod1f("resX", svr->width);
		shader->mod1f("resY", svr->height);

		//Bind lighting shader
		this->shader->bind();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gDepth);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gColor);

		//Render quad that covers the whole screen
		RenderQuad();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}
