//
// Created by antchr-4 on 11/17/16.
//

#ifndef GSCEPT_LAB_ENV_AABB_H
#define GSCEPT_LAB_ENV_AABB_H

#include <vector>
#include "vector3D.h"
#include "matrix4D.h"

class Plane;

class AABB
{
public:
    AABB(vector3D center, vector3D halfSize);

    AABB();

    virtual ~AABB();

    AABB(float maxX, float maxY, float maxZ, float minX, float minY, float minZ);
    void reconstructAABB(matrix4D mod);

    vector3D p1, p2, p3, p4, p5, p6, p7, p8;
    vector3D op1, op2, op3, op4, op5, op6, op7, op8;

    std::vector<Plane*> planes;
    Plane* pl1, *pl2, *pl3, *pl4, *pl5, *pl6;
    vector3D center;
    vector3D centerOr;
    vector3D halfSize;
    vector3D min;
    vector3D max;


};


#endif //GSCEPT_LAB_ENV_AABB_H
