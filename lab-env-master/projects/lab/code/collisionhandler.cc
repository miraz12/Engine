//
// Created by antchr-4 on 12/1/16.
//

#include <algorithm>
#include <cfloat>
#include "collisionhandler.h"

bool compr(GraphicsNode const * a, GraphicsNode const * b)
{
    return a->getMeshConst()->box->min[sortAxis] < b->getMeshConst()->box->min[sortAxis];
};
void CollisionHandler::SortAndSweep()       //Sort objects positions and sweep for collision
{
    collisionBool = false;
    std::sort(objects, objects + numEle, compr);
    vector3D s, s2, v;

    for (int i = 0; i < numEle; ++i)
    {
        vector3D p = (objects[i]->getAABB->min + objects[i]->getAABB->max) * 0.5f;
        for (int c = 0; c < 3; ++c)
        {
            s[c] += p[c];
            s2[c] += p[c] * p[c];
        }
        for (int j = i + 1; j < numEle; ++j)
        {
            if (objects[j]->getAABB->min[sortAxis] > objects[i]->getAABB->max[sortAxis])
            {
                break;
            }
            if (Overlap(objects[i]->getAABB, objects[j]->getAABB))
            {
                    bool col = GJK(objects[i], objects[j]);
                    objects[i]->getMesh()->collision = col;
                    objects[j]->getMesh()->collision = col;
                    collisionBool = true;
                if (col)
                    {
                        EPA(objects[i], objects[j]);
                        CollisionInfo cI;
                        cI.depth = this->penDepth;
                        cI.normal = this->norm;
                        cI.point = this->point;
                        cI.B = objects[j];
                        objects[i]->collInfVec.push_back(cI);
                        //cI.normal = this->norm * -1;
                        //cI.B = objects[i];
                        //objects[j]->collInfVec.push_back(cI);
                    }
            }
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        v[i] = s2[i] - s[i] * s[i] / numEle;
    }
    sortAxis = 0;
    if (v[1] > v[0]) sortAxis = 1;
    if (v[2] > v[sortAxis]) sortAxis = 2;
}
CollisionHandler::CollisionHandler(std::vector<GraphicsNode *> nodes)
{
    int i;
    for (i = 0; i < nodes.size(); ++i)
        if (i < 10)
            objects[i] = nodes[i];
    numEle = i;
}
bool CollisionHandler::Overlap(AABB* a, AABB* b)        //AABB intersection
{
    bool x = std::abs(a->center[0] - b->center[0]) <= (a->halfSize[0] + b->halfSize[0]);
    bool y = std::abs(a->center[1] - b->center[1]) <= (a->halfSize[1] + b->halfSize[1]);
    bool z = std::abs(a->center[2] - b->center[2]) <= (a->halfSize[2] + b->halfSize[2]);
    return x && y && z;
}
bool CollisionHandler::GJK(GraphicsNode *A, GraphicsNode *B)        //Preform GJK
{
    simp.Clear();
    vector3D dir;
    std::vector<vector3D> asdf = A->getMesh()->vertColl;
    vector3D startVert = A->getMesh()->vertColl[0] - B->getMesh()->vertColl[0];
    SupportPoint start;
    start.v = startVert;
    start.supA = A->getMesh()->vertColl[0];
    simp.addSupport(start);
    dir = vector3D(0,0,0) - startVert;
    dir = dir * -1;
    while (true)
    {
        vector3D supportVert = simp.Support(A, B, dir);
        SupportPoint newSupp;
        newSupp.v = supportVert;
        newSupp.supA = simp.FindSupportVertex(A, dir);
        if (supportVert.dot(dir) <= 0)
        {
            return false; //origin not in shape
        }
        else
        {
            //Add support to simplex
            simp.addSupport(newSupp);
            if (simp.DoSimplex(dir)) //Inside simplex?
            {
                return true;
            }
        }
    }
}
void CollisionHandler::EPA(GraphicsNode *A, GraphicsNode *B)        //Expanding Polytype Algorithm
{
    const float _EXIT_THRESHOLD = 0.001f;
    const unsigned _EXIT_ITERATION_LIMIT = 50;
    unsigned _EXIT_ITERATION_CUR = 0;

    std::list<Triangle> faces;
    std::list<Edge> edges;

    auto lam_support = [&](vector3D d)->SupportPoint {
        d.normalize(); // to quell numerical instability
        SupportPoint ret;
        ret.supA = FindSupportVertex(A, d);
        ret.v = Support(A, B, d);
        return ret;
    };

    // process the specified edge, if another edge with the same points in the
    // opposite order exists then it is removed and the new point is also not added
    // this ensures only the outermost ring edges of a cluster of triangles remain
    // in the list
    auto lam_addEdge = [&](SupportPoint a, SupportPoint b)->void {
        for(auto it = edges.begin(); it != edges.end(); it++) {
            if(it->points[0]==b && it->points[1]==a) {
                //opposite edge found, remove it and do not add new one
                edges.erase(it);
                return;
            }
        }
        edges.emplace_back(a,b);
    };

    // add the GJK simplex triangles to the list
    faces.emplace_back(simp.getA(), simp.getB(), simp.getC());
    faces.emplace_back(simp.getA(), simp.getC(), simp.getD());
    faces.emplace_back(simp.getA(), simp.getD(), simp.getB());
    faces.emplace_back(simp.getB(), simp.getD(), simp.getC());

    while (true)
    {
        if (_EXIT_ITERATION_CUR++ >= _EXIT_ITERATION_LIMIT)
            return;

        // find closest triangle to origin
        std::list<Triangle>::iterator entry_cur_triangle_it = faces.begin();
        float entry_cur_dst = FLT_MAX;
        for (auto it = faces.begin(); it != faces.end(); ++it)
        {
            float dst = fabs(it->n.dot(it->points[0].v));
            if(dst < entry_cur_dst)
            {
                entry_cur_dst = dst;
                entry_cur_triangle_it = it;
            }
        }

        // get the next support point in front of the current triangle, away from the origin
        SupportPoint entry_cur_support = lam_support(entry_cur_triangle_it->n);

        // checks how much further this new point will takes us from the origin
        // if it is not far enough then we assume we have found the closest triangle
        // on the hull from the origin
        if(((entry_cur_triangle_it->n.dot(entry_cur_support.v)) - entry_cur_dst < _EXIT_THRESHOLD))
        {
            // Extrapolate info and get coll normal, depth and point.
            ExtrapolateContactInfo(A, B, &(*entry_cur_triangle_it));
            break;
        }

        for(auto it = faces.begin(); it != faces.end();)
        {
            // can this face be 'seen' by entry_cur_support?
            if((it->n.dot(entry_cur_support.v - it->points[0].v)) > 0)
            {
                lam_addEdge(it->points[0],it->points[1]);
                lam_addEdge(it->points[1],it->points[2]);
                lam_addEdge(it->points[2],it->points[0]);
                it = faces.erase(it);
                continue;
            }
            it++;
        }

        // create new triangles from the edges in the edge list
        for(auto it = edges.begin(); it != edges.end(); it++)
        {
            faces.emplace_back(entry_cur_support,it->points[0],it->points[1]);
        }
        edges.clear();
    }
    return;

}
void CollisionHandler::ExtrapolateContactInfo(GraphicsNode *A, GraphicsNode *B, Triangle *tri)      //Calculate contact information
{
    //Collision depth
    float dis = tri->n.dot(tri->points[0].v);
    //Normal of coll is in the opposite dir of the face normal
    vector3D wNorm = tri->n * -1;

    float bary_u,bary_v,bary_w;
    //Transform to barycentric coord
    Barycentric(tri->n * dis, tri->points[0].v, tri->points[1].v, tri->points[2].v, &bary_u, &bary_v, &bary_w);

    if(fabs(bary_u)>1.0f || fabs(bary_v)>1.0f || fabs(bary_w)>1.0f) return;
    //Calculate collision point in world space
    point = vector3D((bary_u * (tri->points[0].supA)) + (bary_v * (tri->points[1].supA)) + (bary_w * (tri->points[2].supA)));
    norm = wNorm;
    penDepth = dis;
}
void CollisionHandler::Barycentric(vector3D p, vector3D a, vector3D b, vector3D c, float *u, float *v, float *w)    //Calculate berycentric coordinates.
{
    vector3D v0 = b - a,v1 = c - a,v2 = p - a;
    float d00 = v0.dot(v0);
    float d01 = v0.dot(v1);
    float d11 = v1.dot(v1);
    float d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    float denom = d00 * d11 - d01 * d01;
    *v = (d11 * d20 - d01 * d21) / denom;
    *w = (d00 * d21 - d01 * d20) / denom;
    *u = 1.0f - *v - *w;
}
vector3D CollisionHandler::Support(GraphicsNode *A, GraphicsNode *B, vector3D dir)      //Find support vertex from both objects.
{
    vector3D pA = FindSupportVertex(A, dir);
    vector3D pB = FindSupportVertex(B, (dir * -1));
    //Minkowski
    vector3D kow = pA - pB;
    return kow;
}
vector3D CollisionHandler::FindSupportVertex(GraphicsNode *a, vector3D dir)     //Find vertex in direction
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

void CollisionHandler::ResolveCollision(std::vector<GraphicsNode *> gNodesPhysics)
{
    for (int i = 0; i < gNodesPhysics.size(); ++i)
    {
        if (gNodesPhysics[i]->collInfVec.size() > 0)
        {
            for (int k = 0; k < gNodesPhysics[i]->collInfVec.size(); ++k)
            {
                //Fetch collision data
                float epsilon = 0.0f;   //Restitution
                GraphicsNode *A = gNodesPhysics[i];
                GraphicsNode *B = (GraphicsNode *) A->collInfVec[k].B;
                vector3D P = A->collInfVec[k].point;
                vector3D N = A->collInfVec[k].normal;
                double D = A->collInfVec[k].depth;

                //Calculate relative velocity
                PhysicsNode Ap = *A->getPhysicsNode();
                PhysicsNode Bp = *B->getPhysicsNode();
                vector3D ra = Ap.position;
                vector3D rb = Bp.position;
                vector3D Pa = ((Ap.velocity) + (Ap.getSpinVec().cross(P - ra)));
                vector3D Pb = ((Bp.velocity) + (Bp.getSpinVec().cross(P - rb)));
                float Vrel = N.dot(Pa - Pb);


                if (Vrel < -0.f)   //Colliding
                {
                    float numerator = -(1.0f + epsilon) * Vrel;
                    float denominator1 = Ap.inverseMass + Bp.inverseMass;   // InvMassA - InvMassB
                    float denominator2 = N.dot((Ap.inverseInertiaTensorWorld * ra.cross(N)).cross(ra)); // N((InvInerta.dot*(ra.cross(n)).cross(ra))))
                    float denominator3 = N.dot((Bp.inverseInertiaTensorWorld * rb.cross(N)).cross(rb)); // N((InvInerta.dot*(rb.cross(n)).cross(rb))))
                    float denominatorFinal = denominator1 + denominator2 + denominator3;
                    float j = numerator / denominatorFinal;

                    //Impulse for bodies A and B
                    vector3D Ja = j * N;
                    vector3D Jb = Ja * -1.0f;

                    //Move bodies to collision point
                    if (A->getPhysicsNode()->inverseMass != 0.f)
                    {
                        vector3D norm = ra - P;
                        norm.normalize();
                        A->getPhysicsNode()->position += norm * D;
                    }

                    if (B->getPhysicsNode()->inverseMass != 0.f)
                    {
                        vector3D norm = rb - P;
                        norm.normalize();
                        B->getPhysicsNode()->position += norm * D;
                    }

                    //Apply impulse
                    A->getPhysicsNode()->applyImpulse(Ja, P);
                    B->getPhysicsNode()->applyImpulse(Jb, P);

                }
                std::vector<CollisionInfo>::iterator c = gNodesPhysics[i]->collInfVec.begin();
                gNodesPhysics[i]->collInfVec.erase(c + k);
            }
            }

    }

}
