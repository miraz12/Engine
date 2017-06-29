//
// Created by antchr-4 on 11/17/16.
//

#include <cmath>
#include "AABB.h"
#include "plane.h"

AABB::AABB()
{}

AABB::AABB(vector3D center, vector3D halfSize) : center(center), halfSize(halfSize)
{
    centerOr = center;
    this->center = center;

    float maxX = halfSize.x();
    float maxY = halfSize.y();
    float maxZ = halfSize.z();

    float minX = -halfSize.x();
    float minY = -halfSize.y();
    float minZ = -halfSize.z();

    min.setValues(minX, minY, minZ);
    max.setValues(maxX, maxY, maxZ);

    //centerOr = (min + max) * 0.5f;

    p1.setValues(maxX, maxY, minZ);
    p2.setValues(minX, maxY, minZ);
    p3.setValues(maxX, minY, minZ);
    p4.setValues(minX, minY, minZ);
    p5.setValues(maxX, maxY, maxZ);
    p6.setValues(minX, maxY, maxZ);
    p7.setValues(maxX, minY, maxZ);
    p8.setValues(minX, minY, maxZ);

    op1 = p1;
    op2 = p2;
    op3 = p3;
    op4 = p4;
    op5 = p5;
    op6 = p6;
    op7 = p7;
    op8 = p8;

    pl1 = new Plane(p5, p6, p7, p8);
    planes.push_back(pl1);
    pl2 = new Plane(p2, p1, p4, p3);
    planes.push_back(pl2);
    pl3 = new Plane(p1, p5, p3, p7);
    planes.push_back(pl3);
    pl4 = new Plane(p2, p6, p4, p8);
    planes.push_back(pl4);
    pl5 = new Plane(p1, p2, p5, p6);
    planes.push_back(pl5);
    pl6 = new Plane(p7, p8, p3, p4);
    planes.push_back(pl6);

}

AABB::AABB(float maxX, float maxY, float maxZ, float minX, float minY, float minZ)
{
    min.setValues(minX, minY, minZ);
    max.setValues(maxX, maxY, maxZ);

    centerOr = (min + max) * 0.5f;

    p1.setValues(maxX, maxY, minZ);
    p2.setValues(minX, maxY, minZ);
    p3.setValues(maxX, minY, minZ);
    p4.setValues(minX, minY, minZ);
    p5.setValues(maxX, maxY, maxZ);
    p6.setValues(minX, maxY, maxZ);
    p7.setValues(maxX, minY, maxZ);
    p8.setValues(minX, minY, maxZ);

    op1 = p1;
    op2 = p2;
    op3 = p3;
    op4 = p4;
    op5 = p5;
    op6 = p6;
    op7 = p7;
    op8 = p8;

    pl1 = new Plane(p5, p6, p7, p8);
    planes.push_back(pl1);
    pl2 = new Plane(p2, p1, p4, p3);
    planes.push_back(pl2);
    pl3 = new Plane(p1, p5, p3, p7);
    planes.push_back(pl3);
    pl4 = new Plane(p2, p6, p4, p8);
    planes.push_back(pl4);
    pl5 = new Plane(p1, p2, p5, p6);
    planes.push_back(pl5);
    pl6 = new Plane(p7, p8, p3, p4);
    planes.push_back(pl6);
}

void AABB::reconstructAABB(matrix4D mod)
{

    std::vector<vector3D> newPoints;

    newPoints.push_back(mod.transform(op1));
    newPoints.push_back(mod.transform(op2));
    newPoints.push_back(mod.transform(op3));
    newPoints.push_back(mod.transform(op4));
    newPoints.push_back(mod.transform(op5));
    newPoints.push_back(mod.transform(op6));
    newPoints.push_back(mod.transform(op7));
    newPoints.push_back(mod.transform(op8));

    float tempMaxX = newPoints[0][0];
    float tempMaxY = newPoints[0][1];
    float tempMaxZ = newPoints[0][2];

    float tempMinX = newPoints[0][0];
    float tempMinY = newPoints[0][1];
    float tempMinZ = newPoints[0][2];

    for (int i = 1; i < newPoints.size(); ++i)  //Find min and max vertices
    {
        if (newPoints[i][0] > tempMaxX)
        tempMaxX = newPoints[i][0];
        if (newPoints[i][0] < tempMinX)
            tempMinX = newPoints[i][0];
        if (newPoints[i][1] > tempMaxY)
            tempMaxY = newPoints[i][1];
        if (newPoints[i][1] < tempMinY)
            tempMinY = newPoints[i][1];
        if (newPoints[i][2] > tempMaxZ)
            tempMaxZ = newPoints[i][2];
        if (newPoints[i][2] < tempMinZ)
            tempMinZ = newPoints[i][2];
    }

    min.setValues(tempMinX, tempMinY, tempMinZ);
    max.setValues(tempMaxX, tempMaxY, tempMaxZ);

    p1.setValues(tempMaxX, tempMaxY, tempMinZ);
    p2.setValues(tempMinX, tempMaxY, tempMinZ);
    p3.setValues(tempMaxX, tempMinY, tempMinZ);
    p4.setValues(tempMinX, tempMinY, tempMinZ);
    p5.setValues(tempMaxX, tempMaxY, tempMaxZ);
    p6.setValues(tempMinX, tempMaxY, tempMaxZ);
    p7.setValues(tempMaxX, tempMinY, tempMaxZ);
    p8.setValues(tempMinX, tempMinY, tempMaxZ);

    //Reconstruct plane
    planes[0]->reconstructPlane(p5, p6, p7, p8);
    planes[1]->reconstructPlane(p2, p1, p4, p3);
    planes[2]->reconstructPlane(p1, p5, p3, p7);
    planes[3]->reconstructPlane(p2, p6, p4, p8);
    planes[4]->reconstructPlane(p1, p2, p5, p6);
    planes[5]->reconstructPlane(p7, p8, p3, p4);

    center = (min + max) * 0.5f;        //Calculate new center

    halfSize = center - max;        //Calculate new half size
    halfSize[0] = std::abs(halfSize[0]);
    halfSize[1] = std::abs(halfSize[1]);
    halfSize[2] = std::abs(halfSize[2]);
}

AABB::~AABB()
{
    delete(pl1);
    delete(pl2);
    delete(pl3);
    delete(pl4);
    delete(pl5);
    delete(pl6);
}






