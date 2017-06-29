//
// Created by antchr-4 on 12/1/16.
//

#ifndef GSCEPT_LAB_ENV_COLLISIONHANDLER_H
#define GSCEPT_LAB_ENV_COLLISIONHANDLER_H
#include <list>
#include <vector>
#include <map>
#include "GraphicsNode.h"
#include "simplex.h"

#define getAABB getMesh()->box
#define MAX_OBJECTS 10

static int sortAxis = 0;

struct Triangle
{
    SupportPoint points[3];
    vector3D n;
    Triangle(SupportPoint a, SupportPoint b, SupportPoint c)
    {
        points[0] = a;
        points[1] = b;
        points[2] = c;
        n = (((b.v-a.v).cross(c.v-a.v)).normalizeRe());
    }
};

struct Edge
{
    SupportPoint points[2];
    Edge(SupportPoint a, SupportPoint b)
    {
        points[0] = a;
        points[1] = b;
    }
};

enum
{
    MIN_ELEM = 0,
    MAX_ELEM = 1
};



class CollisionHandler
{

public:
    CollisionHandler(std::vector<GraphicsNode*> nodes);
    void SortAndSweep();
    bool Overlap(AABB* a, AABB* b);
    bool GJK(GraphicsNode* A, GraphicsNode* B);
    void EPA(GraphicsNode *A, GraphicsNode *B);
    void ExtrapolateContactInfo(GraphicsNode *A, GraphicsNode *B, Triangle* tri);
    vector3D Support(GraphicsNode* A, GraphicsNode* B, vector3D dir);
    vector3D FindSupportVertex(GraphicsNode* a, vector3D dir);
    void ResolveCollision(std::vector<GraphicsNode*> gNodesPhysics);


    Simplex simp;

    int numEle;
    GraphicsNode* objects[MAX_OBJECTS];

    bool collisionBool = false;

    vector3D point;
    vector3D norm;
    float penDepth;

private:
    void Barycentric(vector3D p, vector3D a, vector3D b, vector3D c, float* u, float* v, float* w);
};



#endif //GSCEPT_LAB_ENV_COLLISIONHANDLER_H
