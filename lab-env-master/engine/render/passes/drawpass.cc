#include "config.h"
#include "drawpass.h"
#include "render/servers/renderserver.h"

namespace Passes
{
    DrawPass::DrawPass()
    {
        shader = std::make_shared<Resources::ShaderObject>("content/Shader/drawpass.vs", "content/Shader/drawpass.fs");
    }

    DrawPass::~DrawPass()
    {
    }

    void DrawPass::Setup()
    {

        shader->bind();
        shader->mod1i("gColor", 4); //0:position, 1:normal, 2:albedoSpec, 3:depth, 4:Fragcolor 
        glUseProgram(0);
    }

    void DrawPass::Execute()
    {
        Servers::RenderServer* svr = Servers::RenderServer::GetInstance();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        svr->ReadGBuffer();
               
        //Bind lighting shader
        this->shader->bind();
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, svr->getlPass()->gColor);

        //Render quad that covers the whole screen
        renderQuad();

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DrawPass::renderQuad()
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
