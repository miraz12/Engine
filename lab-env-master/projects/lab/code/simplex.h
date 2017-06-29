//
// Created by antchr-4 on 12/6/16.
//

#ifndef GSCEPT_LAB_ENV_SIMPLEX_H
#define GSCEPT_LAB_ENV_SIMPLEX_H


#include <vector>
#include "vector3D.h"
#include "GraphicsNode.h"

struct SupportPoint
{
    vector3D v;
    vector3D supA;
    inline bool operator==(const SupportPoint &r) const { return v == r.v; }
};

class Simplex
{
public:
    void addSupport(SupportPoint vert);
    bool DoSimplex(vector3D& dir);
    vector3D Support(GraphicsNode* A, GraphicsNode* B, vector3D dir);
    vector3D FindSupportVertex(GraphicsNode* a, vector3D dir);
    void Clear();
    int Size(){ return simplexVec.size(); };
    SupportPoint getA(){ return simplexVec[0];};
    SupportPoint getB(){ return simplexVec[1];};
    SupportPoint getC(){ return simplexVec[2];};
    SupportPoint getD(){ return simplexVec[3];};

private:
    std::vector<SupportPoint> simplexVec;
    std::vector<vector3D> VecA;
    vector3D LineCheck();
    vector3D TriangleCheck();
    bool TetrahedronCheck(vector3D& dir);
    void NewSimplex(SupportPoint A);
    void NewSimplex(SupportPoint A, SupportPoint B);
    void NewSimplex(SupportPoint A, SupportPoint B, SupportPoint C);

};


#endif //GSCEPT_LAB_ENV_SIMPLEX_H
