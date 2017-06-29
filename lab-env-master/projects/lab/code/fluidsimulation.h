//
// Created by antchr-4 on 2/2/17.
//

#ifndef GSCEPT_LAB_ENV_FLUIDSIMULATION_H
#define GSCEPT_LAB_ENV_FLUIDSIMULATION_H


#include <vector>
//#include "macgrid.h"
#include "particlesystem.h"
#include "pressuresolver.h"
#include "mactable.h"
#define kcfl 0.5f    //Increase time step size *
#define maxVelocity 5.f //Max velocity

class FluidSimulation
{
private:

    const float maxPos = 0.8f;
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

    vector3D gravity = vector3D(0.f, -25.0f, 0.f);
    float viscosity = 1.f;


public:
    FluidSimulation(float width, float height, float depth, vector3D pos);
    float fRandom(float low, float high);
    void initParticles();
    void CalculateTimeStep();
    void AdvanceVelocityField();
    void ApplyExternal();
    void ApplyViscosity();
    void TraceParticle();
    vector3D GetVelocityAtPos(float x, float y, float z);
    void CalculatePressure();
    void ApplyPressureToVelocityField();
    void ApplyPressureToFaceU(int i, int j, int k);
    void ApplyPressureToFaceV(int i, int j, int k);
    void ApplyPressureToFaceW(int i, int j, int k);
    void ExtrapolateFluidVel();
    void SetSolidVelocity();
    void UpdateGrid();
    void MoveParticles();
    double getMaxSpeed();

    std::vector<vector3D> fluidIndex;
    //MacGrid grid;
    MacTable grid;
    std::vector<particle> markerPart;
    vector3D pos = vector3D(0.f,0.f,0.f);
    const float width;
    const float height;
    const float depth;
    float dt = 1.f/33.f; //Timestep
    double density = 20.0;

    std::vector<float> pressureVector;
    std::map<int, int> fluidVIndex;
    std::vector<Cell* > fluidCells;
    int nP = 1;
    bool firstTimeStep = true;

};


#endif //GSCEPT_LAB_ENV_FLUIDSIMULATION_H
