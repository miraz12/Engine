//
// Created by antchr-4 on 1/30/17.
//

#include "particlesystem.h"


GLuint partSSbo;

void ParticleSystem::initGraphics()
{
    glGenBuffers(1, &partSSbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, partSSbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_PARTICLES* sizeof(particle), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

float ParticleSystem::fRandom(float low, float high)
{
    long random();

    float r = (float)rand();
    return (low + r * (high-low) / (float)RAND_MAX);
}

void ParticleSystem::animate()
{
    shaderCompute->bind();
    glDispatchCompute(NUM_PARTICLES / WORK_GROUP_SIZE, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

ParticleSystem::ParticleSystem(const std::string& cShader)
{
    shaderCompute = std::make_shared<ShaderObject>(cShader);
    shaderRender = std::make_shared<ShaderObject>("VertexShaderCompute.vs", "FragmentShaderCompute.fs");
}

void ParticleSystem::display()
{

    shaderCompute->bind();

    glColor3f(1,0,0);

    glBindBuffer( GL_ARRAY_BUFFER, partSSbo  );
    glVertexPointer( 4, GL_FLOAT, 24, (void *)0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    glUseProgram(0);

}

void ParticleSystem::updateParticles(std::vector<particle> p)
{

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, partSSbo );
    particle *sParticle = (particle *) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 0, NUM_PARTICLES * sizeof(particle), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT );
    for( int i = 0; i < NUM_PARTICLES; i++ )
    {
        sParticle[ i ].pos[0] = p[i].pos[0];//fRandom( XMIN, XMAX );
        sParticle[ i ].pos[1] = p[i].pos[1];//fRandom( YMIN, YMAX );
        sParticle[ i ].pos[2] = p[i].pos[2];//fRandom( ZMIN, ZMAX );
        sParticle[ i ].pos[3] = 1.f;

        sParticle[ i ].vel[0] = 0;//;fRandom( VXMIN, VXMAX );
        sParticle[ i ].vel[1] = 0;//fRandom( VYMIN, VYMAX );
        sParticle[ i ].vel[2] = 0;//fRandom( VZMIN, VZMAX );
        sParticle[ i ].vel[3] = 0.f;

        sParticle[ i ].col[0] = 0.f;//fRandom( .3f, 1.f );
        sParticle[ i ].col[1] = 0.f;//( .3f, 1.f );
        sParticle[ i ].col[2] = 1.f; //fRandom( .3f, 1.f );
        sParticle[ i ].col[3] = 1.f;
    }
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );

    glBindBufferBase( GL_SHADER_STORAGE_BUFFER,  7,  partSSbo  );

    glBindBuffer( GL_SHADER_STORAGE_BUFFER, 0 );
}

void ParticleSystem::render(matrix4D mvp)
{
    shaderRender->bind();
    shaderRender->modMatrix4fv("projectionView", mvp);

    glPointSize(10.f);
    //glEnable( GL_PROGRAM_POINT_SIZE );
    glBindBuffer( GL_ARRAY_BUFFER, partSSbo );
    glVertexPointer( 4, GL_FLOAT, 24, (void *)0 );

    glDrawArraysInstanced(GL_POINTS, 0, 1, NUM_PARTICLES);

}

