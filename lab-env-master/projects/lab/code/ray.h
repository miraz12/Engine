//
// Created by antchr-4 on 11/7/16.
//

#ifndef GSCEPT_LAB_ENV_RAY_H
#define GSCEPT_LAB_ENV_RAY_H

#include <vector>
#include "vector3D.h"
#include "plane.h"
#include "GraphicsNode.h"

class Ray
{
public:
    Ray(vector3D P0, vector3D P1);
    vector3D PointOnRay(float t);
    vector3D Intersect(Plane p);
    void Intersect(std::vector<GraphicsNode*> planes);
    bool Intersect(std::vector<Plane*> planes);
    void ClosestHit(vector3D newHit);
    void IntersectTriangle(Plane* tri);
    bool PointOnMesh(Plane* pla);

    vector3D P0;
    vector3D P1;
    vector3D v;

    vector3D pHit;

    std::vector<vector3D> hits;

    bool hit;

};


#endif //GSCEPT_LAB_ENV_RAY_H
