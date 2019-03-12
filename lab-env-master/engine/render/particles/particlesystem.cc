#include "config.h"
#include "particlesystem.h"
#include <algorithm>


namespace Particles
{

    Particlesystem::Particlesystem()
    {
        colors = vector4D(0.055f, 0.055f, 0.055f, 1.0f);
        direction = vector3D(0, 10, 0);
        position = vector3D(0, 0, -10);
        spread = 2.0f;
        min_age = 0.0f;
        shaderRs = std::make_shared<Resources::ShaderObject>("content/Shader/particles.vs", "content/Shader/particles.fs");
        textureRs = std::make_shared<Resources::TextureResource>("content/smoke.png");
        textureRs->Load();

        g_particule_position_size_data = new GLfloat[MAX_PARTICLES * 4];
        g_particule_color_data = new GLubyte[MAX_PARTICLES * 4];

        for (size_t i = 0; i < MAX_PARTICLES; i++)
        {
            ParticlesContainer[i].cameradistance = -1;
            ParticlesContainer[i].life = -1.0f;
        }

        //Billboard
        static const GLfloat g_vertex_buffer_data[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
        };
        glGenBuffers(1, &billboard_vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        // The VBO containing the positions and sizes of the particles
        glGenBuffers(1, &particles_position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

        // The VBO containing the colors of the particles
        glGenBuffers(1, &particles_color_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
    }

    void Particlesystem::Update(float t, float dt, vector3D campos)
    {
        // Simulate all particles
        ParticlesCount = 0;
        for (int i = 0; i<MAX_PARTICLES; i++)
        {
            Particle& p = ParticlesContainer[i];
            UpdateParticle(&p, t, dt, campos);
        }
        //SortParticles();

        min_age += dt;

        // Should we create any new particle(s)?
        while (min_age >= BIRTH_INTERVAL)
        {
            min_age -= BIRTH_INTERVAL;

            // Find a dead particle to replace with a new one
            InitParticle(&ParticlesContainer[0], t);
        }

    }

    void Particlesystem::Render(matrix4D view, matrix4D projection)
    {
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
        glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);

        //Bind our shader
        shaderRs->bind();
        //Bind our texture
        textureRs->bind(GL_TEXTURE0, glGetUniformLocation(shaderRs->getProgram(), "particleTexture"), 0);

        shaderRs->modVector3f("CameraRight_worldspace", vector3D(view[0][0], view[1][0], view[2][0]));
        shaderRs->modVector3f("CameraUp_worldspace", vector3D(view[0][1], view[1][1], view[2][1]));
        shaderRs->modMatrix4fv("VP", view * projection);

        //vertices
        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // positions of particles
        glEnableVertexAttribArray(5);
        glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        //particles' colors
        glEnableVertexAttribArray(6);
        glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
        glVertexAttribPointer(6, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

        glVertexAttribDivisor(4, 0); // particles vertices
        glVertexAttribDivisor(5, 1); // positions 
        glVertexAttribDivisor(6, 1); // color
        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

        glDisableVertexAttribArray(4);
        glDisableVertexAttribArray(5);
        glDisableVertexAttribArray(6);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }

    int Particlesystem::FindUnusedParticle()
    {
        for (int i = LastUsedParticle; i<MAX_PARTICLES; i++)
        {
            if (ParticlesContainer[i].life < 0)
            {
                LastUsedParticle = i;
                return i;
            }
        }

        for (int i = 0; i<LastUsedParticle; i++)
        {
            if (ParticlesContainer[i].life < 0)
            {
                LastUsedParticle = i;
                return i;
            }
        }
        return 0; // All particles are taken, override the first one
    }

    void Particlesystem::SortParticles()
    {
        std::sort(&ParticlesContainer[0], &ParticlesContainer[MAX_PARTICLES]);
    }

    void Particlesystem::InitParticle(Particle *p, float dt)
    {
        int particleIndex = FindUnusedParticle();
        ParticlesContainer[particleIndex].life = LIFE_SPAN;
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float r3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        //Random startpos for a wider base
        ParticlesContainer[particleIndex].pos = vector3D(r, r2, r3) + position;

        //Random dir
        vector3D randomdir = vector3D(
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f,
            (rand() % 2000 - 1000.0f) / 1000.0f
        );

        //Add random dir with general dir and spread
        ParticlesContainer[particleIndex].vel = direction + randomdir * spread;


        ParticlesContainer[particleIndex].r = unsigned char(colors.x() * 255);
        ParticlesContainer[particleIndex].g = unsigned char(colors.y() * 255);
        ParticlesContainer[particleIndex].b = unsigned char(colors.z() * 255);
        ParticlesContainer[particleIndex].a = unsigned char(colors.w() * 255);

        ParticlesContainer[particleIndex].size = (rand() % 1000) * 0.0005f + 0.1f;
    }

    void Particlesystem::UpdateParticle(Particle *p, double t, float dt, vector3D campos)
    {
        if (p->life > 0.0f)
        {
            // Decrease life
            p->life -= dt;
            if (p->life > 0.0f)
            {
                float x_acc = 10.0f * ((rand() % 3) - 1);
                float y_acc = 1.0f * ((rand() % 3) - 1);
                float z_acc = 1.0f;//0.0001*random();
                vector3D acc = vector3D(x_acc, y_acc, z_acc);
                p->vel = p->vel + acc * dt;
                p->pos += p->vel * dt * 0.3f;
                p->size += 0.001f;

                //Make more transparent the older the particle
                p->a = unsigned char(155 / LIFE_SPAN * p->life);


                // Fill the GPU buffer
                g_particule_position_size_data[4 * ParticlesCount + 0] = p->pos.x();
                g_particule_position_size_data[4 * ParticlesCount + 1] = p->pos.y();
                g_particule_position_size_data[4 * ParticlesCount + 2] = p->pos.z();

                g_particule_position_size_data[4 * ParticlesCount + 3] = p->size;

                g_particule_color_data[4 * ParticlesCount + 0] = p->r;
                g_particule_color_data[4 * ParticlesCount + 1] = p->g;
                g_particule_color_data[4 * ParticlesCount + 2] = p->b;
                g_particule_color_data[4 * ParticlesCount + 3] = p->a;

            }
            else
            {
                // Particles that just died will be put at the end of the buffer in SortParticles();
                p->cameradistance = -1.0f;
            }
            ParticlesCount++;
        }
    }

}
