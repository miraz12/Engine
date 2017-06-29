//
// Created by antchr-4 on 11/7/16.
//

#include "plane.h"

Plane::Plane(vector3D a, vector3D b, vector3D c, vector3D d)
{
    this->A = a;
    this->B = b;
    this->C = c;
    this->D = d;
    this->n = (((b-a).cross(c-a)).normalizeRe());
    this->d = -this->n.dot(a);

    this->oA = a;
    this->oB = b;
    this->oC = c;
    this->oD = d;

}

Plane::Plane(vector3D a, vector3D b, vector3D c)
{
    this->A = a;
    this->B = b;
    this->C = c;
    this->n = (((b-a).cross(c-a)).normalizeRe());
    this->d = -this->n.dot(a);

    this->oA = a;
    this->oB = b;
    this->oC = c;
}

bool Plane::pointOnPlane(vector3D p)
{
    float x = (n[0]*p[0]);
    float y = (n[1]*p[1]);
    float z = (n[2]*p[2]);
    double b = ( x+ y + z) + d;
    if (b <= 0.00001 )
    {
        return  true;
    }
    return false;
}

bool Plane::pointOnQuad(vector3D p)
{
    if (sameSide(p, A, B, C) && sameSide(p, B, D, A) && sameSide(p, D, C, B) && sameSide(p, C, A, D) && pointOnQuadZ(p))
        return true;

    return false;
}

bool Plane::sameSide(vector3D p, vector3D a, vector3D b, vector3D c)
{
    vector3D cp1 = (b-a).cross(p-a);
    vector3D cp2 = (b-a).cross(c-a);
    float test = cp1.dot(cp2);
    if(cp1.dot(cp2) >= 0.0f)
        return true;
    else
        return false;
}

bool Plane::pointOnQuadZ(vector3D p)
{
    vector3D v = A - p;

    if(!(v.dot(n) >= 0.000001) && !(v.dot(n) <= -0.000001))
    {
        return true;
    }
    return false;
}

void Plane::reconstructPlane(vector3D a, vector3D b, vector3D c, vector3D d)
{
    this->A = a;
    this->B = b;
    this->C = c;
    this->D = d;
    this->n = (((b-a).cross(c-a)).normalizeRe());
    this->d = -this->n.dot(a);
}

void Plane::reconstructPlane(vector3D a, vector3D b, vector3D c)
{
    this->A = a;
    this->B = b;
    this->C = c;
    this->n = (((b-a).cross(c-a)).normalizeRe());
    this->d = -this->n.dot(a);
}




