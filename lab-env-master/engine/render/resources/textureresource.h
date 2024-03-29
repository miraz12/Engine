#pragma once
#include "GL/glew.h"

namespace Resources
{
    class TextureResource
    {
    public:
        /// constructor
        TextureResource();
        TextureResource(const char* filename);
        TextureResource(GLenum TextureTarget, const char* filename);

        bool Load();

        /// destructor
        ~TextureResource();


        void bind();
        void bind(GLenum textureUnit, GLuint textureID, int unit);
        void Unbind();

    private:
        const char* m_fileName;
        GLenum m_textureTarget = GL_TEXTURE_2D;
        GLuint m_texture;

    };
}

