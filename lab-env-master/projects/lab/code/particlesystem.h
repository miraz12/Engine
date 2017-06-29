//
// Created by antchr-4 on 1/30/17.
//

#ifndef GSCEPT_LAB_ENV_PARTICLESYSTEM_H
#define GSCEPT_LAB_ENV_PARTICLESYSTEM_H

#include <GL/glew.h>
#include <string>
#include "ShaderObject.h"
#include <memory>
#include <vector>

#define NUM_PARTICLES 1024//*1024 // total number of particles to move
#define WORK_GROUP_SIZE 128 // # work-items per work-group

struct particle
{
    vector4D pos;
    vector4D vel;
    vector4D col;
};
class ParticleSystem
{




public:
    ParticleSystem(const std::string&);

    void initGraphics();
    void animate();
    void display();
    void render(matrix4D mvp);
    void updateParticles(std::vector<particle> p);
    float fRandom(float low, float high);

private:
    // random parameters:

    const float maxPos = 0.f;
    const float XMIN = 	{ -maxPos };
    const float XMAX = 	{  maxPos };
    const float YMIN = 	{ -maxPos };
    const float YMAX = 	{  maxPos };
    const float ZMIN = 	{ -maxPos };
    const float ZMAX = 	{  maxPos };

    const float maxVel = 0.01f;
    const float VXMIN =	{   -maxVel };
    const float VXMAX =	{    maxVel };
    const float VYMIN =	{   -maxVel };
    const float VYMAX =	{    maxVel };
    const float VZMIN =	{   -maxVel };
    const float VZMAX =	{    maxVel };

    std::shared_ptr<ShaderObject> shaderCompute;
    std::shared_ptr<ShaderObject> shaderRender;


};


#endif //GSCEPT_LAB_ENV_PARTICLESYSTEM_H
