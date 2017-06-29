//
// Created by antchr-4 on 11/7/16.
//

#include "ray.h"

Ray::Ray(vector3D P0, vector3D P1) : P0(P0), P1(P1)
{
    //v = (P1 - P0)
    this->v = this->P1 - this->P0;
    pHit[0] = -13;
    pHit[1] = -3;
    pHit[2] = -7;
}

vector3D Ray::PointOnRay(float t)
{
    //L(t) = P0 + tv (t >= 0)
    vector3D L = this->P0 + this->v*t;
    return L;
}

vector3D Ray::Intersect(Plane p)
{
    //t = -(P0 * N + d)/(V * N)
    float t = -(P0.dot(p.n)+ p.d) / (this->v.dot(p.n));
    return PointOnRay(t);
}



void Ray::ClosestHit(vector3D newHit)
{
    vector3D l1 = pHit - P0;
    vector3D l2 = newHit - P0;

    if (pHit[0] == -13, pHit[1] == -3, pHit[2] == -7)
    {
        pHit = newHit;
    }
    if (newHit[0] == -13, newHit[1] == -3, newHit[2] == -7)
    {
        return;
    }

    if (l1.sqrlength() > l2.sqrlength())
    {
        pHit = newHit;
    }
}

void Ray::Intersect(std::vector<GraphicsNode*> planes)
{
    for (int i = 0; i < planes.size(); ++i)
    {
        planes[i]->getMesh()->hitByRay = false;
        vector3D inter = Intersect(*planes[i]->getMesh()->pla);
        if (planes[i]->getMesh()->pla->pointOnPlane(inter))
        {
            if (planes[i]->getMesh()->pla->pointOnQuad(inter))
            {
                hits.push_back(inter);
            }
        }
    }

    for (int j = 0; j < hits.size(); ++j)
    {
        ClosestHit(hits[j]);
    }
    hits.clear();

    //Draw plane and color closest plane hit by ray.
    for (int i = 0; i < planes.size(); ++i)
    {
        bool validPoint = true;

        vector3D inter = pHit;
        if (inter[0] == -13 && inter[1] == -3 && inter[2] == -7)
            validPoint = false;

        if (planes[i]->getMesh()->pla->pointOnQuad(inter) && validPoint)
        {
            planes[i]->getMesh()->hitByRay = true;
        }
    }

}

bool Ray::Intersect(std::vector<Plane*> planes)
{
    for (int i = 0; i < planes.size(); ++i)
    {
        vector3D inter = Intersect(*planes[i]);
        if (planes[i]->pointOnPlane(inter))
        {
            if (planes[i]->pointOnQuad(inter))
            {
                hits.push_back(inter);
            }
        }
    }

    for (int j = 0; j < hits.size(); ++j)
    {
        ClosestHit(hits[j]);
    }
    hits.clear();

    //Draw plane and color closest plane hit by ray.
    for (int i = 0; i < planes.size(); ++i)
    {
        bool validPoint = true;

        vector3D inter = pHit;
        if (inter[0] == -13 && inter[1] == -3 && inter[2] == -7)
            validPoint = false;

        if (planes[i]->pointOnQuad(inter) && validPoint)
        {
            return true;
        }
    }

    return false;
}

void Ray::IntersectTriangle(Plane* pla)
{
    vector3D hitPoint;
    vector3D u, v, n;
    vector3D dir, w0, w;
    float r, a, b;

    // get triangle edge vectors and plane normal
    u = pla->B - pla->A;
    v = pla->C - pla->A;
    n = u.cross(v);              // cross product
    if (n[0] == 0 && n[1] == 0 && n[2] == 0)             // triangle is degenerate
        return;                  // do not deal with this case

    dir = P1 - P0;              // ray direction vector
    w0 = P0 - pla->A;
    a = -n.dot(w0);
    b = n.dot(dir);
    if (fabs(b) < 0.00000001)
    {     // ray is  parallel to triangle plane
        if (a == 0)                 // ray lies in triangle plane
            return;
        else return;              // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
        return;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    hitPoint = P0 + dir * r;            // intersect point of ray and plane

    // is I inside T?
    float    uu, uv, vv, wu, wv, D;
    uu = u.dot(u);
    uv = u.dot(v);
    vv = v.dot(v);
    w = hitPoint - pla->A;
    wu = w.dot(u);
    wv = w.dot(v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)         // I is outside T
        return;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)  // I is outside T
        return;

    ClosestHit(hitPoint); // I is in T
}

bool Ray::PointOnMesh(Plane* pla)
{
    vector3D hitPoint;
    vector3D u, v, n;
    vector3D dir, w0, w;
    float r, a, b;

    // get triangle edge vectors and plane normal
    u = pla->B - pla->A;
    v = pla->C - pla->A;
    n = u.cross(v);              // cross product
    if (n[0] == 0 && n[1] == 0 && n[2] == 0)             // triangle is degenerate
        return false;                  // do not deal with this case

    dir = P1 - P0;              // ray direction vector
    w0 = P0 - pla->A;
    a = -n.dot(w0);
    b = n.dot(dir);
    if (fabs(b) < 0.00000001)
    {     // ray is  parallel to triangle plane
        if (a == 0)                 // ray lies in triangle plane
            return false;
        else return false;              // ray disjoint from plane
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)                    // ray goes away from triangle
        return false;                   // => no intersect
    // for a segment, also test if (r > 1.0) => no intersect

    hitPoint = P0 + dir * r;            // intersect point of ray and plane

    if (hitPoint[0] == pHit[0] && hitPoint[1] == pHit[1] && hitPoint[2] == pHit[2])
        return true;
    else
        return false;
}
