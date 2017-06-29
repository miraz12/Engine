//
// Created by antchr-4 on 12/6/16.
//

#include "simplex.h"

void Simplex::addSupport(SupportPoint vert)
{
    this->simplexVec.insert(simplexVec.begin(), vert);
    return;
}

vector3D Simplex::LineCheck()
{
    vector3D AB = getB().v - getA().v;
    vector3D org(0.0f, 0.0f, 0.0f);
    vector3D AO = org - getA().v;
    if (AB.dot(AO) > 0)
    {
        NewSimplex(getA(), getB());
        return AB.cross(AO).cross(AB);
    } else
    {
        NewSimplex(getA());
        return AO;
    }
}

vector3D Simplex::TriangleCheck()
{
    vector3D AB = getB().v - getA().v;
    vector3D AC = getC().v - getA().v;
    vector3D ABC = AB.cross(AC);
    vector3D org(0.0f, 0.0f, 0.0f);
    vector3D AO = org - getA().v;

    vector3D ABCxAC = ABC.cross(AC);
    if(ABCxAC.dot(org - ABCxAC) > 0)
    {
        if (AC.dot(AO) > 0)
        {
            //1
            NewSimplex(getA(), getC());
            return AC.cross(AO).cross(AC);
        }
        else
        {
            //StarCASE
            if (AB.dot(AO) > 0)
            {
                //4
                NewSimplex(getA(), getB());
                return AB.cross(AO).cross(AB);
            }
            else
            {
                //5
                NewSimplex(getA());
                return AO;
            }
        }
    }
    else
    {
        if (AB.cross(ABC).dot(AO) > 0)
        {
            //StarCASE
            if (AB.dot(AO) > 0)
            {
                //4
                NewSimplex(getA(), getB());
                return AB.cross(AO).cross(AB);
            }
            else
            {
                //5
                NewSimplex(getA());
                return AO;
            }
        }
        else
        {
            if (ABC.dot(AO) > 0)
            {
                //2
                NewSimplex(getA(), getB(), getC());
                return ABC;
            }
            else
            {
                //3
                NewSimplex(getA(), getC(), getB());
                return ABC * -1;
            }
        }
    }
}

bool Simplex::TetrahedronCheck(vector3D& dir)
{
    vector3D normal1 = (getA().v-getB().v).cross(getA().v-getC().v);
    vector3D normal2 = (getA().v-getD().v).cross(getA().v-getB().v);
    vector3D normal3 = (getA().v-getC().v).cross(getA().v-getD().v);
    vector3D AO = vector3D(0,0,0) - getA().v;

    if(normal1.dot(AO) > 0)
    {
        NewSimplex(getA(), getB(), getC());
        dir = TriangleCheck();
        return false;
    }
    else if(normal2.dot(AO) > 0)
    {
        NewSimplex(getA(), getB(), getD());
        dir = TriangleCheck();
        return false;
    }
    else if(normal3.dot(AO) > 0)
    {
        NewSimplex(getA(), getD(), getC());
        dir = TriangleCheck();
        return false;
    }
    else
    {
        return true;
    }
}

bool Simplex::DoSimplex(vector3D& dir)
{
    if (simplexVec.size() == 4)
    {
        return TetrahedronCheck(dir);
    }
    else if(simplexVec.size() == 3)
    {
        dir = TriangleCheck();
    }
    else
    {
        dir = LineCheck();
    }
    return false;
}

vector3D Simplex::Support(GraphicsNode *A, GraphicsNode *B, vector3D dir)
{
    vector3D pA = FindSupportVertex(A, dir);
    vector3D pB = FindSupportVertex(B, (dir * -1));
    //Minkowski
    vector3D kow = pA - pB;
    return kow;
}

vector3D Simplex::FindSupportVertex(GraphicsNode *a, vector3D dir)
{
    std::vector<vector3D> verts =  a->getMesh()->vertColl;
    vector3D center = a->getMesh()->box->center;
    vector3D outVert = verts[0];
    float lOut = (outVert - center).dot(dir);
    for (int i = 1; i < verts.size(); ++i)
    {
        vector3D centerToVert = verts[i] - center;
        float l = centerToVert.dot(dir);
        if (l > lOut)
        {
            outVert = verts[i];
            lOut = l;
        }
    }
    return outVert;
}

void Simplex::NewSimplex(SupportPoint A)
{
    simplexVec.clear();
    simplexVec.push_back(A);
}

void Simplex::NewSimplex(SupportPoint A, SupportPoint B)
{
    simplexVec.clear();
    simplexVec.push_back(A);
    simplexVec.push_back(B);
}

void Simplex::NewSimplex(SupportPoint A, SupportPoint B, SupportPoint C)
{
    simplexVec.clear();
    simplexVec.push_back(A);
    simplexVec.push_back(B);
    simplexVec.push_back(C);
}

void Simplex::Clear()
{
    this->simplexVec.clear();
    this->VecA.clear();
}


