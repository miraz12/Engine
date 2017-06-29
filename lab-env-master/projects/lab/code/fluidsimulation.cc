//
// Created by antchr-4 on 2/2/17.
//

#include "fluidsimulation.h"
#include <algorithm>

FluidSimulation::FluidSimulation(float width, float height, float depth, vector3D pos) : width(width), height(height), depth(depth), grid(width, height, depth, pos)
{
    this->pos = pos;

}

void FluidSimulation::CalculateTimeStep()
{
    if (firstTimeStep)
    {
        firstTimeStep = false;
    } else
    {
        double mS = getMaxSpeed();
        dt = kcfl * grid.hCellWidth / mS;
        std::cout << "TIME STEP: " << dt << std::endl;
    }

    return;
}

void FluidSimulation::initParticles()
{
    this->markerPart.clear();
    int n = NUM_PARTICLES;
    this->markerPart.resize(n);
    for( int i = 0; i < n; i++ )
    {
        this->markerPart[ i ].pos[0] = fRandom( XMIN, XMAX );
        this->markerPart[ i ].pos[1] = fRandom( YMIN, YMAX );
        this->markerPart[ i ].pos[2] = fRandom( ZMIN, ZMAX );
        this->markerPart[ i ].pos[3] = 1.f;
    }
}

float FluidSimulation::fRandom(float low, float high)
{
    long random();

    float r = (float)rand();
    return (low + r * (high-low) / (float)RAND_MAX);
}

void FluidSimulation::UpdateGrid()
{

    this->grid.SetLayers();     //Set all cells to layer -1
    this->fluidCells.clear();

    for (int i = 0; i < this->markerPart.size(); ++i)   //Set all cells with marker particles to fluid.
    {
        vector3D pos = grid.FindClosestCell(vector3D(markerPart[i].pos.x(), markerPart[i].pos.y(), markerPart[i].pos.z()));
        bool inGrid;
        int pi = std::floor((pos.x()+grid.hWidth)*grid.invCellWidht);
        int pj = std::floor((pos.y()+grid.hHeight)*grid.invCellHeight);
        int pk = std::floor((pos.z()+grid.hDepth)*grid.invCellDepth);
        Cell* c = grid.findGridCell(pi, pj, pk, &inGrid);
        if(inGrid)
        {
            if (c->type != Cell::SOLID)
            {
                c->type = Cell::FLUID;
                c ->layer = 0;
            }
        }
        else
        {
            c = new Cell(Cell::FLUID, 0);
            c->position = pos;
            c->i = pi;
            c->j = pj;
            c->k = pk;
            grid.insertCell(c);
        }
    }
    //Add buffer layers.
    for (int j = 1; j <= std::max(2, (int)kcfl); ++j)
    {
        for (auto it : grid.cellMap)
        {
            Cell *itc;
            std::vector<Cell *> chain = it.second;
            for (int idx = 0; idx < (int) chain.size(); idx++)
            {
                itc = chain[idx];
                if ((itc->type == Cell::FLUID || itc->type == Cell::AIR) &&
                    (itc->layer == j - 1)) //For each fluid or air cell with layer == j - 1;
                {
                    std::vector<vector3D> neigh = grid.getNeighbourPos(itc->position);
                    for (int i = 0; i < neigh.size(); ++i)  //For cells neighbors
                    {
                        int indexX = (int)floor((neigh[i].x()+grid.hWidth) * grid.invCellWidht);
                        int indexY = (int)floor((neigh[i].y()+grid.hWidth) * grid.invCellHeight);
                        int indexZ = (int)floor((neigh[i].z()+grid.hWidth) * grid.invCellDepth);
                        bool inGrid = false;
                        Cell *c = grid.findGridCell(indexX, indexY, indexZ, &inGrid);
                        if (inGrid)   //If in grid
                        {
                            if ((c->layer == -1) && c->type != Cell::SOLID)
                            {
                                c->type = Cell::AIR;
                                c->layer = j;
                            } else if ((c->layer == -1) && c->type == Cell::SOLID)
                            {
                                c->type = Cell::SOLID;
                                c->layer = j;
                            }
                        } else    //Else add to grid
                        {
                            c = new Cell;
                            if (grid.cellInBounds(neigh[i]))
                            {
                                c->type = Cell::AIR;
                            } else
                            {
                                c->type = Cell::SOLID;
                            }
                            c->position = neigh[i];
                            c->layer = j;
                            c->i = indexX;
                            c->j = indexY;
                            c->k = indexZ;
                            grid.insertCell(c);
                        }
                    }
                }
            }
        }
    }

    for (int z = 0; z < grid.depthCount; ++z)
    {
        for (int y = 0; y < grid.colCount; ++y)
        {
            for (int x = 0; x < grid.rowCount; ++x)
            {
                bool inGrid = false;
                Cell* c = grid.findGridCell(x,y,z, &inGrid);

                if (inGrid)
                {
                    if (c->type == Cell::FLUID)
                    {
                        c->fluidIndex = fluidCells.size();
                        this->fluidCells.push_back(c);
                    }
                }
            }
        }
    }

    for (auto it : grid.cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            if (itc->layer == -1)
            {
                grid.deleteCell(itc);
            }
        }
    }

    grid.setNeighbours();

}

void FluidSimulation::AdvanceVelocityField()
{
    //int size = grid.macCells.size();
    CalculateTimeStep();
    UpdateGrid();
    TraceParticle();                      //Apply convection using backwards particles (Advection)
    ApplyExternal();                      //Apply external forces. (Like gravity)
    //ApplyViscosity();                   //Viscosity
    CalculatePressure();                  //Solve pressure              Pressure calc is probably correct now
    //ApplyPressureToVelocityField();       //Apply pressure to grid
    ExtrapolateFluidVel();              //Extrapolate fluid velocity to neigbours
    //SetSolidVelocity();                 //Set velocities of solid cells
    MoveParticles();
}

void FluidSimulation::TraceParticle()
{
    float t = -dt;
    grid.ResetStagedVelocity();

    for (int l = 0; l < this->fluidCells.size(); ++l)
    {
        Cell * itc = fluidCells[l];

        float x = itc->position.x();
        float y = itc->position.y();
        float z = itc->position.z();

        //RK2 Integration
        vector3D v = GetVelocityAtPos(x, y, z);
        v = GetVelocityAtPos(x + 0.5f * t * v.x(), y + 0.5f * t * v.y(), z + 0.5f * t * v.z());
        vector3D point = vector3D(x, y, z) + t * v;

        int i = std::floor((point.x()+grid.hWidth) * grid.invCellWidht);
        int j = std::floor((point.y()+grid.hWidth) * grid.invCellHeight);
        int k = std::floor((point.z()+grid.hWidth) * grid.invCellDepth);

        bool inGrid=false;
        Cell* c = grid.findGridCell(i,j,k, &inGrid);
        if(inGrid)
        {
            itc->stagedVel = c->velocity * dt;      //TODO: This might not be correct. Fix the cell face offset and maybe the interpolation
        }
    }
    for (auto it : grid.cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            itc->velocity = itc->stagedVel;
        }
    }
}

vector3D FluidSimulation::GetVelocityAtPos(float x, float y, float z)
{
    vector3D v;
    v[0] = grid.getInterpolatedValue((x+grid.hWidth)/grid.cellWidth,       (y+grid.hHeight)/grid.cellWidth-0.5f ,(z+grid.hDepth)/grid.cellWidth-0.5f, 0);
    v[1] = grid.getInterpolatedValue((x+grid.hWidth)/grid.cellWidth-0.5f,  (y+grid.hHeight)/grid.cellWidth ,     (z+grid.hDepth)/grid.cellWidth-0.5f, 1);
    v[2] = grid.getInterpolatedValue((x+grid.hWidth)/grid.cellWidth-0.5f,  (y+grid.hHeight)/grid.cellWidth-0.5f ,(z+grid.hDepth)/grid.cellWidth, 2);
    return v;
}

void FluidSimulation::ApplyExternal()
{
    for (auto it : grid.cellMap)
    {
        Cell *itc;
        std::vector<Cell*> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            if (grid.IsFaceBorderingTypeV(itc->i, itc->j, itc->k, Cell::FLUID))  //Add body force to faces bordering fluid
            {
                itc->velocity += gravity * dt;
            }
        }
    }
}

void FluidSimulation::ApplyViscosity()
{
/*
    for(auto it : grid.macCells)
    {
        if (it.second->type == Cell::FLUID)
        {
            float x = it.second->position.x();
            float y = it.second->position.y();
            float z = it.second->position.z();

            vector3D v;
            v[0] = grid.getLaplacianTimes(x/grid.cellWidth, y/grid.cellWidth - 0.5f ,z/grid.cellWidth-0.5f, 0);
            v[1] = grid.getLaplacianTimes(x/grid.cellWidth - 0.5f, y/grid.cellWidth ,z/grid.cellWidth- 0.5f, 1);
            v[2] = grid.getLaplacianTimes(x/grid.cellWidth- 0.5f, y/grid.cellWidth - 0.5f ,z/grid.cellWidth, 2);
            it.second->stagedVel = it.second->stagedVel +  v;
        }
    }*/
}

void FluidSimulation::CalculatePressure()
{
    PressureSolverParameters param;
    param.cellwidth = grid.cellWidth;
    param.deltaTime = this->dt;
    param.velocityField = &grid;
    param.fCells = this->fluidCells;
    param.density = this->density;

    int numFluid = fluidCells.size();
    VectorXd p(numFluid);
    PressureSolver pSolver = PressureSolver();
    pSolver.solve(param, p);

    this->pressureVector.resize(numFluid);
    for (int i = 0; i < p.size(); ++i)
    {
        this->pressureVector[i] = (float)p[i];
    }


    std::map<int, int> temp(pSolver.fluidVIndex);
    fluidVIndex = temp;


}
///-----------------------Apply pressure------------------------
void FluidSimulation::ApplyPressureToVelocityField()
{
    for (int k = 0; k < grid.depthCount; k++)
    {
        for (int j = 0; j < grid.colCount; j++)
        {
            for (int i = 0; i < grid.rowCount + 1; i++)
            {
                bool inGrid = false;
                Cell* c = grid.findGridCell(i, j, k, &inGrid);
                if (inGrid)
                {
                    int i = c->i;
                    int j = c->j;
                    int k = c->k;
                    if (grid.IsFaceBorderingTypeU(i, j, k, Cell::SOLID))
                    {
                        c->velocity[0] = 0.0f;
                    }

                    else if (grid.IsFaceBorderingTypeU(i, j, k, Cell::FLUID))
                    {
                        ApplyPressureToFaceU(i,j,k);
                    }
                }
            }
        }
    }
    for (int k = 0; k < grid.depthCount; k++)
    {
        for (int j = 0; j < grid.colCount+1; j++)
        {
            for (int i = 0; i < grid.rowCount; i++)
            {
                bool inGrid = false;
                Cell* c = grid.findGridCell(i, j, k, &inGrid);
                if (inGrid)
                {
                    if (grid.IsFaceBorderingTypeV(i, j, k, Cell::SOLID))
                    {
                        c->velocity[1] = 0.0f;
                    }
                    if (grid.IsFaceBorderingTypeV(i, j, k, Cell::FLUID) && !grid.IsFaceBorderingTypeV(i, j, k, Cell::SOLID))
                    {
                        ApplyPressureToFaceV(i, j, k);
                    }
                }
            }
        }
    }
    for (int k = 0; k < grid.depthCount+1; k++)
    {
        for (int j = 0; j < grid.colCount; j++)
        {
            for (int i = 0; i < grid.rowCount; i++)
            {
                bool inGrid = false;
                Cell* c = grid.findGridCell(i, j, k, &inGrid);
                if (inGrid)
                {
                    if (grid.IsFaceBorderingTypeW(i, j, k, Cell::SOLID))
                    {
                        c->velocity[2] = 0.0f;
                    }
                    if (grid.IsFaceBorderingTypeW(i, j, k, Cell::FLUID) && !grid.IsFaceBorderingTypeW(i, j, k, Cell::SOLID))
                    {
                        ApplyPressureToFaceW(i, j, k);
                    }
                }
            }
        }
    }
    for (auto it : grid.cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            itc->velocity = itc->stagedVel;         //Set velocity from staged
        }
    }
}

void FluidSimulation::ApplyPressureToFaceU(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (density * grid.cellWidth);
    double invscale = 1.0 / scale;

    int ci = i - 1; int cj = j; int ck = k;
    Cell* c0, *c1;
    c0 = grid.getGridCell(i, j, k);
    c1 = grid.getGridCell(i -1 , j, k);


    double p0, p1;
    if (c1->type != Cell::SOLID && c0->type != Cell::SOLID) {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c0->fluidIndex];
    } else if (c1->type == Cell::SOLID) {
        p0 = pressureVector[c0->fluidIndex] - invscale*(c0->velocity.x() - usolid);
        p1 = pressureVector[c0->fluidIndex];
    } else {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c1->fluidIndex] + invscale*(c0->velocity.x() - usolid);
    }

    double unext = c0->velocity.x() - scale*(p1 - p0);
    c0->stagedVel[0] = unext;
}

void FluidSimulation::ApplyPressureToFaceV(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (density * grid.cellWidth);
    double invscale = 1.0 / scale;

    Cell* c0, *c1;
    c0 = grid.getGridCell(i, j, k);
    c1 = grid.getGridCell(i, j-1, k);


    double p0, p1;
    if (c1->type != Cell::SOLID && c0->type != Cell::SOLID) {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c0->fluidIndex];
    } else if (c1->type == Cell::SOLID) {
        p0 = pressureVector[c0->fluidIndex] - invscale*(c0->velocity.y() - usolid);
        p1 = pressureVector[c0->fluidIndex];
    } else {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c1->fluidIndex] + invscale*(c0->velocity.y() - usolid);
    }

    double vnext = c0->velocity.y() - scale*(p1 - p0);
    c0->stagedVel[1] = vnext;
}

void FluidSimulation::ApplyPressureToFaceW(int i, int j, int k)
{
    double usolid = 0.0;   // solids are stationary
    double scale = dt / (density * grid.cellWidth);
    double invscale = 1.0 / scale;

    Cell* c0, *c1;
    c0 = grid.getGridCell(i, j, k);
    c1 = grid.getGridCell(i , j, k-1);


    double p0, p1;
    if (c1->type != Cell::SOLID && c0->type != Cell::SOLID) {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c0->fluidIndex];
    } else if (c1->type == Cell::SOLID) {
        p0 = pressureVector[c0->fluidIndex] - invscale*(c0->velocity.z() - usolid);
        p1 = pressureVector[c0->fluidIndex];
    } else {
        p0 = pressureVector[c1->fluidIndex];
        p1 = pressureVector[c1->fluidIndex] + invscale*(c0->velocity.z() - usolid);
    }

    double wnext = c0->velocity.z() - scale*(p1 - p0);
    c0->stagedVel[2] = wnext;
}
///--------------------Extrapolate fluid----------------------------------------------
void FluidSimulation::ExtrapolateFluidVel()
{

    int maxLayers = std::max(2, (int)kcfl);
    grid.SetLayers();       //Set all layers to -1
    grid.UpdateLayers(maxLayers);   //Update grid

    for (int idx = 1; idx <= maxLayers; ++idx)
    {
        //-----------------U-vel----------------------

        for (int k = 0; k < grid.depthCount; k++)
        {
            for (int j = 0; j < grid.colCount; j++)
            {
                for (int i = 0; i < grid.rowCount+1; i++)
                {
                    bool inGrid = false;
                    Cell* c = grid.findGridCell(i, j, k, &inGrid);
                    if (inGrid)
                    {
                        bool isExtrapolated = (grid.IsBorderingIndexU(i, j , k, idx))  && (!grid.IsBorderingIndexU(i, j , k, idx-1))  && (!grid.IsFaceBorderingTypeU(i, j, k, Cell::SOLID));
                        if (isExtrapolated)
                        {
                            double v0 = grid.getExtrapolatedVelocity(i, j, k, idx, 0);
                            c->velocity[0] = (float)v0;
                        }
                    }
                }
            }
        }
        //-----------------V-vel----------------------

        for (int k = 0; k < grid.depthCount; k++)
        {
            for (int j = 0; j < grid.colCount+1; j++)
            {
                for (int i = 0; i < grid.rowCount; i++)
                {
                    bool inGrid = false;
                    Cell *c = grid.findGridCell(i, j, k, &inGrid);
                    if (inGrid)
                    {
                        bool isExtrapolated = (grid.IsBorderingIndexV(i, j , k, idx))  && (!grid.IsBorderingIndexV(i, j , k, idx-1)) && (!grid.IsFaceBorderingTypeV(i, j, k, Cell::SOLID));
                        if (isExtrapolated)
                        {
                            double v1 = grid.getExtrapolatedVelocity(i, j, k, idx, 1);
                            c->velocity[1] = (float)v1;
                        }
                    }
                }
            }
        }
        //-----------------W-vel----------------------
        for (int k = 0; k < grid.depthCount+1; k++)
        {
            for (int j = 0; j < grid.colCount; j++)
            {
                for (int i = 0; i < grid.rowCount; i++)
                {
                    bool inGrid = false;
                    Cell *c = grid.findGridCell(i, j, k, &inGrid);
                    if (inGrid)
                    {
                        bool isExtrapolated = (grid.IsBorderingIndexW(i, j , k, idx))  && (!grid.IsBorderingIndexW(i, j , k, idx-1))  && (!grid.IsFaceBorderingTypeW(i, j, k, Cell::SOLID));
                        if (isExtrapolated)
                        {
                            double v2 = grid.getExtrapolatedVelocity(i, j, k, idx, 2);
                            c->velocity[2] = (float)v2;
                        }
                    }
                }
            }
        }
    }

}

void FluidSimulation::SetSolidVelocity()
{
    for (auto it : grid.cellMap)
    {
        Cell *itc;
        std::vector<Cell *> chain = it.second;
        for (int idx = 0; idx < (int) chain.size(); idx++)
        {
            itc = chain[idx];
            if (itc->type == Cell::FLUID)
            {

                int i = itc->i;
                int j = itc->j;
                int k = itc->k;

                bool inGrid = false;
                Cell* c = grid.findGridCell(i-1,j,k,&inGrid);

                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[0] > 0.f)
                        {
                            c->stagedVel[0] = 0.0f;
                        }
                    }
                }
                c = grid.findGridCell(i+1,j,k,&inGrid);
                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[0] < 0.f)
                        {
                            c->stagedVel[0] = 0.0f;
                        }
                    }
                }
                c = grid.findGridCell(i,j+1,k,&inGrid);
                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[1] > 0.f)
                        {
                            c->stagedVel[1] = 0.0f;
                        }
                    }
                }
                c = grid.findGridCell(i,j-1,k,&inGrid);
                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[1] < 0.f)
                        {
                            c->stagedVel[1] = 0.0f;
                        }
                    }
                }
                c = grid.findGridCell(i,j,k-1,&inGrid);
                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[2] > 0.f)
                        {
                            c->stagedVel[2] = 0.0f;
                        }
                    }
                }
                c = grid.findGridCell(i,j,k+1,&inGrid);
                if(inGrid)
                {
                    if(c->type != Cell::SOLID)
                    {
                        if(c->stagedVel[2] < 0.f)
                        {
                            c->stagedVel[2] = 0.0f;
                        }
                    }
                }
            }
        }
    }
}

void FluidSimulation::MoveParticles()
{
    for (int i = 0; i < this->markerPart.size(); ++i)
    {
        //RK2 Integration
        //vector3D v = GetVelocityAtPos(markerPart[i].pos.x(), markerPart[i].pos.y(), markerPart[i].pos.z());
        //v = GetVelocityAtPos(markerPart[i].pos.x() + 0.5f * dt * v.x(), markerPart[i].pos.y() + 0.5f * dt * v.y(), markerPart[i].pos.z() + 0.5f * dt * v.z());
        int ip = std::floor((markerPart[i].pos.x()+grid.hWidth)*grid.invCellDepth);
        //int j = std::floor(y / cellHeight);
        int jp = std::floor((markerPart[i].pos.y()+grid.hWidth)*grid.invCellDepth);
        //int k = std::floor(z / cellDepth);
        int kp = std::floor((markerPart[i].pos.z()+grid.hWidth)*grid.invCellDepth);
        vector3D v = grid.getGridCell(ip, jp, kp)->velocity;


        markerPart[i].pos = markerPart[i].pos + dt * v;
        markerPart[i].vel = v;

    }
    return;
}

double FluidSimulation::getMaxSpeed()
{
    double maxsq = 0.0;
    for (unsigned int i = 0; i < markerPart.size(); i++) {
        double distsq = markerPart[i].vel.lengthSqr();
        if (distsq > maxsq) {
            maxsq = distsq;
        }
    }

    return sqrt(maxsq);
}





