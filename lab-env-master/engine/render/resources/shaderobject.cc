#include "config.h"
#include "shaderobject.h"
#include <fstream>

namespace Resources
{
    ShaderObject::ShaderObject(const std::string& vertexFile, const std::string& fragmentFile)
    {
        program = glCreateProgram();
        m_fileNameVs = vertexFile;
        m_fileNameFs = fragmentFile;


        shaders[0] = CreateShader(LoadFile(vertexFile), GL_VERTEX_SHADER);
        shaders[1] = CreateShader(LoadFile(fragmentFile), GL_FRAGMENT_SHADER);

        for (int i = 0; i < NUM_SHADERS; i++)
        {
            glAttachShader(program, shaders[i]);
        }

        GLint shaderLogSize;
        glLinkProgram(program);
        glValidateProgram(program);

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


    ShaderObject::ShaderObject()
    {
    }



    ShaderObject::~ShaderObject()
    {
        glDeleteProgram(program);

    }



    std::string ShaderObject::LoadFile(const std::string& fileName)
    {
        std::ifstream file;
        file.open((fileName).c_str());

        std::string output;
        std::string line;

        if (file.is_open())
        {
            while (file.good())
            {
                getline(file, line);
                output.append(line + "\n");
            }
        }
        else
        {
            std::cerr << "Unable to load file: " << fileName << std::endl;
        }

        return output;
    }

    void ShaderObject::ReloadShader()
    {
        // check if shader already exists
        if (this->program)
        {
            glDeleteProgram(this->program);
            this->program = 0;
        }
        if (this->shaders[0])
        {
            glDeleteShader(this->shaders[0]);
            this->shaders[0] = 0;
        }
        if (this->shaders[1])
        {
            glDeleteShader(this->shaders[1]);
            this->shaders[1] = 0;
        }

        // create a program object
        this->program = glCreateProgram();

        shaders[0] = CreateShader(LoadFile(m_fileNameVs), GL_VERTEX_SHADER);
        shaders[1] = CreateShader(LoadFile(m_fileNameFs), GL_FRAGMENT_SHADER);

        glAttachShader(this->program, this->shaders[0]);
        glAttachShader(this->program, this->shaders[1]);
        glLinkProgram(this->program);

        //bind();
    }


    GLuint ShaderObject::CreateShader(const std::string& fileName, unsigned int type)
    {
        GLuint shader = glCreateShader(type);

        if (shader == 0)
            std::cerr << "Error compiling shader type " << type << std::endl;

        const GLchar* p[1];
        p[0] = fileName.c_str();
        GLint lengths[1];
        lengths[0] = (GLint)fileName.length();

        glShaderSource(shader, 1, p, lengths);
        glCompileShader(shader);

        GLint shaderLogSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shaderLogSize);
        if (shaderLogSize > 0)
        {
            GLchar* buf = new GLchar[shaderLogSize];
            glGetShaderInfoLog(shader, shaderLogSize, NULL, buf);
            printf("[SHADER COMPILE ERROR]: %s", buf);
            delete[] buf;
        }

        return shader;
    }

    void ShaderObject::bind()
    {
        glUseProgram(program);
    }

    void ShaderObject::mod1f(const char* name, float mat)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniform1f(loc, mat);

    }

    void ShaderObject::mod1i(const char* name, int i)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniform1i(loc, i);

    }

    void ShaderObject::modMatrix4fv(const char* name, matrix4D mat)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat.get());

    }

    void ShaderObject::modMatrix3fv(const char* name, matrix3D mat)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniformMatrix3fv(loc, 1, GL_FALSE, *mat.values2D);

    }

    void ShaderObject::modVector4f(const char* name, vector4D vec)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniform4f(loc, vec[0], vec[1], vec[2], vec[3]);
    }

    void ShaderObject::modVector3f(const char* name, vector3D vec)
    {
        GLint loc = glGetUniformLocation(this->program, name);
        glUniform3f(loc, vec[0], vec[1], vec[2]);
    }

    ShaderObject::ShaderObject(const std::string &computeShader)
    {
        program = glCreateProgram();
        shaders[0] = CreateShader(LoadFile(computeShader), GL_COMPUTE_SHADER);


        for (int i = 0; i < 1; i++)
        {
            glAttachShader(program, shaders[i]);
        }

        GLint shaderLogSize;
        glLinkProgram(program);
        glValidateProgram(program);

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
}
