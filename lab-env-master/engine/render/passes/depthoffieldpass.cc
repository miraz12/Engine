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
        renderQuad();

		this->shader->bind();

		Display::Camera* cam =  Display::Camera::GetInstance();
		shader->mod1f("inFocusPoint", cam->depth);
		shader->mod1f("inFocusScale", cam->depthScale);

		Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
		shader->mod1f("resX", svr->width);
		shader->mod1f("resY", svr->height);

		//Bind lighting shader
		this->shader->bind();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gDepth);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, Servers::RenderServer::GetInstance()->getlPass()->gColor);

		//Render quad that covers the whole screen
		renderQuad();

        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    void DofPass::renderQuad()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        unsigned int quadVAO = 0;
        unsigned int quadVBO;
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

}
