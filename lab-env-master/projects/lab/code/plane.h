//
// Created by antchr-4 on 11/7/16.
//

#ifndef GSCEPT_LAB_ENV_PLANE_H
#define GSCEPT_LAB_ENV_PLANE_H

#include "vector3D.h"
//a*x + b*y + c*z + d = 0

class Plane
{
public:
    Plane(vector3D a, vector3D b, vector3D c, vector3D d);
    Plane(vector3D a, vector3D b, vector3D c);

    bool pointOnPlane(vector3D p);
    bool pointOnQuad(vector3D p);
    bool pointOnQuadZ(vector3D p);
    void reconstructPlane(vector3D a, vector3D b, vector3D c, vector3D d);
    void reconstructPlane(vector3D a, vector3D b, vector3D c);
    bool sameSide(vector3D p1, vector3D p2, vector3D a, vector3D b);

    ///corners
    vector3D A;
    vector3D B;
    vector3D C;
    vector3D D;

    //Origin
    vector3D oA;
    vector3D oB;
    vector3D oC;
    vector3D oD;

    vector3D n;
    float d;

};


#endif //GSCEPT_LAB_ENV_PLANE_H
