//
// Created by antchr-4 on 2/1/17.
//

#ifndef GSCEPT_LAB_ENV_CELL_H
#define GSCEPT_LAB_ENV_CELL_H


#include "vector3D.h"

class Cell
{
public:

    enum cellType
    {
        AIR,
        FLUID,
        SOLID
    };
    Cell(cellType t, int lay);
    Cell(){};

    float pressure = 0.0f;
    vector3D velocity = vector3D();
    vector3D stagedVel = vector3D();
    vector3D position = vector3D();
    Cell* rightN = nullptr;
    Cell* leftN = nullptr;
    Cell* upN = nullptr;
    Cell* downN = nullptr;
    Cell* inN = nullptr;
    Cell* outN = nullptr;
    int i,j,k;
    int fluidIndex = -1;

    cellType type = SOLID;
    int layer = -1;
};


#endif //GSCEPT_LAB_ENV_CELL_H
