//
// Created by antchr-4 on 11/21/16.
//

#ifndef GSCEPT_LAB_ENV_PHYSICSNODE_H
#define GSCEPT_LAB_ENV_PHYSICSNODE_H


#include "vector3D.h"
#include "vector4D.h"
#include "matrix4D.h"

class PhysicsNode
{
    struct state
    {
        vector3D angularAcc;
        vector3D lastFrameAcceleration;
    };

public:
    PhysicsNode();

    float mass = 4.0f;
    float inverseMass = 1.f/mass;

    vector3D forceAccum;
    vector3D torqueAccum;
    vector3D angularAcc;

    vector3D position;

    vector3D velocity;

    vector3D acceleration= vector3D(0.f, -.8f, 0.f);
    vector3D lastFrameAcceleration;

    matrix4D inverseInertiaTensorWorld;
    matrix4D invInertiaTensorShape;

    vector4D orientation;
    vector3D angularMomentum;

    vector3D rotation;

    matrix4D transformMatrix;
    matrix4D rotationMatrix;

    state lastFrame;
    state thisFrame;

    float angularDamping = 0.999f;
    float linearDamping = 0.999f;

    bool posLocked = false;
    bool gravity = false;

    void calculateDerivedData();
    void addForceAtPoint(float force, vector3D dir, vector3D point);
    void _transformInertiaTensor(matrix4D &iitWorld, vector4D quat, matrix4D &iitBody, matrix4D rotmat);
    void addForce(float force, vector3D dir);
    void integrate(double duration);
    void calculateInertaInv();
    void createInerta(float w, float h, float d);
    void applyImpulse(vector3D, vector3D);
    void euler(double duration);
    void midpoint(double duration);
    void RK(double duration);
    state evaluate(state de, double time, state n);

    vector3D getSpinVec();

};


#endif //GSCEPT_LAB_ENV_PHYSICSNODE_H
