#pragma once
#include "GL/glew.h"
#include <string>
#include "core/math/matrix4D.h"
#include "core/math/vector2D.h"


namespace Resources
{
    class ShaderObject
    {
    public:
        /// constructor
        ShaderObject();
        ShaderObject(const std::string&, const std::string&);
        ShaderObject(const std::string& computeShader);
        GLuint getProgram() { return program; };

        /// destructor
        ~ShaderObject();

        std::string LoadFile(const std::string&);
        void ReloadShader();
        GLuint CreateShader(const std::string&, unsigned int);
        void bind();

        void mod1f(const char*, float);
        void mod1i(const char*, int);
        void modMatrix4fv(const char*, matrix4D);
        void modMatrix3fv(const char* name, matrix3D mat);
        void modVector4f(const char*, vector4D);
        void modVector3f(const char* name, vector3D vec);
        void modVector1fArray(const char* name, int size, float* vec);
        void modVector2fArray(const char* name, int size, vector2D* vec);
        void modVector3fArray(const char* name, int size, vector3D* vec);
        void modVector4fArray(const char* name, int size, vector4D* vec);


        void checkGlErrors(std::string location)
        {
            GLint shaderLogSize;
            //Get error log
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &shaderLogSize);
            if (shaderLogSize > 0)
            {
                GLchar* buf = new GLchar[shaderLogSize];
                glGetProgramInfoLog(program, shaderLogSize, NULL, buf);
                printf("[PROGRAM LINK ERROR]: %s", buf);
                delete[] buf;
            }
        }

    private:
        static const unsigned int NUM_SHADERS = 2;
        //static const unsigned int NUM_UNIFORMS = 3;

        GLuint program;
        GLuint vertexShader;
        GLuint pixelShader;

        GLuint shaders[NUM_SHADERS];
        std::string m_fileNameVs;
        std::string m_fileNameFs;
        //GLuint uniforms[NUM_UNIFORMS];

    };
}
