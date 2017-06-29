//
// Created by antchr-4 on 11/21/16.
//

#include "physicsnode.h"
#include "GL/glew.h"

PhysicsNode::PhysicsNode()
{
}
//Add force in dir
void PhysicsNode::addForce(float force, vector3D dir)
{
    forceAccum = dir* force;
}
//Integrator. Choose what integrator type you want to use.
void PhysicsNode::integrate(double duration)
{
    //Before integrate
    // Calculate linear acceleration from force inputs.
    thisFrame.lastFrameAcceleration = acceleration; //If gravity is wanted
    thisFrame.lastFrameAcceleration.addScaledVector(forceAccum, inverseMass);
    //Add to angular momentum
    thisFrame.angularAcc = inverseInertiaTensorWorld.transform(torqueAccum);
    //Integrate:
    //euler(duration);
    //midpoint(duration);
    RK(duration);

    calculateDerivedData();
    //clear accum
    forceAccum.setValues(0);
    torqueAccum.setValues(0);
    lastFrame = thisFrame;
}
//Calculate new position and rotation from velocity and orientation
void PhysicsNode::calculateDerivedData()
{
    //Calculate model matrix
    orientation.normalize();
    this->rotationMatrix = this->rotationMatrix.quatRotation(orientation);
    //transformMatrix.transform(position.x(), position.y(), position.z());
    matrix4D posmat;
    posmat.setPos(position.x(), position.y(), position.z());
    ~rotationMatrix;
    this->transformMatrix =  rotationMatrix * posmat;
    calculateInertaInv();
    //_transformInertiaTensor(inverseInertiaTensorWorld, orientation,invInertiaTensorShape, transformMatrix );
}
//Add a force at point
void PhysicsNode::addForceAtPoint(float force, vector3D dir, vector3D point)
{
    vector3D pt = point;
    pt = position - pt;
    dir.normalize();

    //Calculate force
    forceAccum += forceAccum + ( dir * force);
    //Calculate torque Ti(t) = (ri(t)-Cm) x Fi(t)
    torqueAccum += (dir.cross(pt * force));
}
//Create shape inerta tensor. Dependent on weight and volume.
void PhysicsNode::createInerta(float w, float h, float d)
{
    float m = mass/12.0f;
    this->invInertiaTensorShape[0][0] = m * (powf(h, 2) + powf(d, 2));
    this->invInertiaTensorShape[1][1] = m * (powf(w, 2) + powf(d, 2));
    this->invInertiaTensorShape[2][2] = m * (powf(w, 2) + powf(h, 2));
    this->invInertiaTensorShape = invInertiaTensorShape.inv();
    invInertiaTensorShape[3][3] = 1;
}
//Recreate invWorld inertatensor
void PhysicsNode::calculateInertaInv()
{
    matrix4D inv;
    matrix4D rotTrans = rotationMatrix;
    ~rotTrans;
    inv = rotationMatrix*invInertiaTensorShape*rotTrans;
    inverseInertiaTensorWorld = inv;
    inverseInertiaTensorWorld[3][0] = 0;
    inverseInertiaTensorWorld[3][1] = 0;
    inverseInertiaTensorWorld[3][2] = 0;
    inverseInertiaTensorWorld[3][3] = 1;

}
void PhysicsNode::_transformInertiaTensor(matrix4D &iitWorld, vector4D quat, matrix4D &iitBody, matrix4D rotmat)
{
    float t4 = rotmat[0][0]*iitBody[0][0]+
              rotmat[0][1]*iitBody[1][0]+
              rotmat[0][2]*iitBody[2][0];
    float t9 = rotmat[0][0]*iitBody[0][1]+
              rotmat[0][1]*iitBody[1][1]+
              rotmat[0][2]*iitBody[2][1];
    float t14 = rotmat[0][0]*iitBody[0][2]+
               rotmat[0][1]*iitBody[1][2]+
               rotmat[0][2]*iitBody[2][2];
    float t28 = rotmat[1][0]*iitBody[0][0]+
               rotmat[1][1]*iitBody[1][0]+
               rotmat[1][2]*iitBody[2][0];
    float t33 = rotmat[1][0]*iitBody[0][1]+
               rotmat[1][1]*iitBody[1][1]+
               rotmat[1][2]*iitBody[2][1];
    float t38 = rotmat[1][0]*iitBody[0][2]+
               rotmat[1][1]*iitBody[1][2]+
               rotmat[1][2]*iitBody[2][2];
    float t52 = rotmat[2][0]*iitBody[0][0]+
               rotmat[2][1]*iitBody[1][0]+
               rotmat[2][2]*iitBody[2][0];
    float t57 = rotmat[2][0]*iitBody[0][1]+
               rotmat[2][1]*iitBody[1][1]+
               rotmat[2][2]*iitBody[2][1];
    float t62 = rotmat[2][0]*iitBody[0][2]+
               rotmat[2][1]*iitBody[1][2]+
               rotmat[2][2]*iitBody[2][2];

    iitWorld[0][0] = t4*rotmat[0][0]+
                       t9*rotmat[0][1]+
                       t14*rotmat[0][2];
    iitWorld[0][1] = t4*rotmat[1][0]+
                       t9*rotmat[1][1]+
                       t14*rotmat[1][2];
    iitWorld[0][2] = t4*rotmat[2][0]+
                       t9*rotmat[2][1]+
                       t14*rotmat[2][2];
    iitWorld[1][0] = t28*rotmat[0][0]+
                       t33*rotmat[0][1]+
                       t38*rotmat[0][2];
    iitWorld[1][1] = t28*rotmat[1][0]+
                       t33*rotmat[1][1]+
                       t38*rotmat[1][2];
    iitWorld[1][2] = t28*rotmat[2][0]+
                       t33*rotmat[2][1]+
                       t38*rotmat[2][2];
    iitWorld[2][0] = t52*rotmat[0][0]+
                       t57*rotmat[0][1]+
                       t62*rotmat[0][2];
    iitWorld[2][1] = t52*rotmat[1][0]+
                       t57*rotmat[1][1]+
                       t62*rotmat[1][2];
    iitWorld[2][2] = t52*rotmat[2][0]+
                       t57*rotmat[2][1]+
                       t62*rotmat[2][2];

    
}
//Apply impulse
void PhysicsNode::applyImpulse(vector3D J, vector3D P )
{
    velocity += J * (inverseMass);
    vector3D tImp = (P- position).cross(J);
    rotation = rotation + inverseInertiaTensorWorld.transform(tImp);
    return;
}
//Return "spinVector"
vector3D PhysicsNode::getSpinVec()
{
    return rotation;
}

PhysicsNode::state PhysicsNode::evaluate(PhysicsNode::state lastStep, double time, PhysicsNode::state d)
{
    d.lastFrameAcceleration += lastStep.lastFrameAcceleration * time;
    d.angularAcc += lastStep.angularAcc * time;
    return d;
}

void PhysicsNode::euler(double duration)
{
    state der = evaluate(lastFrame, 0.0f, thisFrame);
    velocity.addScaledVector(der.lastFrameAcceleration, duration);
    rotation.addScaledVector(der.angularAcc, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
    //Add to quat rot and out pos
    position.addScaledVector(velocity, duration);
    orientation.addScaledVectorQuat(rotation, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
}

void PhysicsNode::midpoint(double duration)
{
    state der = evaluate(lastFrame, 0.0f, thisFrame);
    state der2 = evaluate(lastFrame, duration * 0.5f, der);

    velocity.addScaledVector(der2.lastFrameAcceleration, duration);
    rotation.addScaledVector(der2.angularAcc, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
    //Add to quat rot and out pos
    position.addScaledVector(velocity, duration);
    orientation.addScaledVectorQuat(rotation, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
}

void PhysicsNode::RK(double duration)
{
    state der = evaluate(lastFrame, 0.0f, thisFrame);
    state der2 = evaluate(lastFrame, duration * 0.5f, der);
    state der3 = evaluate(lastFrame, duration * 0.5f, der2);
    state der4 = evaluate(lastFrame, duration, der3);

    vector3D dangAcc = (der.angularAcc + ((der2.angularAcc + der3.angularAcc)*(2.0f)) + der4.angularAcc) * (1.0f/6.0f);
    vector3D dacc = (der.lastFrameAcceleration + ((der2.lastFrameAcceleration + der3.lastFrameAcceleration)*(2.0f)) + der4.lastFrameAcceleration) * (1.0f/6.0f);

    velocity.addScaledVector(dacc, duration);
    rotation.addScaledVector(dangAcc, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
    //Add to quat rot and out pos
    position.addScaledVector(velocity, duration);
    orientation.addScaledVectorQuat(rotation, duration);
    //Impose drag
    velocity = velocity * pow(linearDamping, duration);
    rotation = rotation * pow(angularDamping, duration);
}

