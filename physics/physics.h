#ifndef PHYSICS_H
#define PHYSICS_H
#include <ode/ode.h>
#include <ode/odecpp_collision.h>

#define BodyID dBodyID
#define GeomID dGeomID
#define WorldID dWorldID
#define SpaceID dSpaceID
#define Mass dMass
#define JointID dJointID
#define JointGroupID dJointGroupID
//#define JointFeedback dJointFeedback
#include "math/vec4.h"
#include "math/quaternion.h"

class Matrix4x4;
class Scene;
class Object;
class Joint;


namespace Physics{

    //inicializadores do ode
    void nearCallback(void *data, GeomID o1, GeomID o2);
    void simSingleStep(Scene *scene);
    void worldStep(WorldID world, float stepSize);
    void setGravity(Scene *scene, Vec4 g);

    void                  createObject(Object *obj);                          //associa ao objeto elementos físicos
    void                  createObject(Object *obj, Vec4 velocity);           //associa ao objeto elementos físicos com velocidade inicial
    QuaternionQ           getOrientationBody(Object *obj);                              //extrai o quaternion de rotação do objeto
    Vec4                  getPositionBody(Object* obj);                              //extrai a posição do objeto
    void                  enableObject(Object* obj);
    void                  disableObject(Object *obj);              //desabilita o objeto para ser manipulado no mundo
    void                  closeObject(Object *obj);

    //manipulation
    void                  bodySetTorque(Object *obj, Vec4 value);
    void                  bodyAddTorque(Object *obj, Vec4 value);
    void                  bodySetForce(Object *obj, Vec4 value);
    void                  bodyAddForce(Object *obj, Vec4 value);

    void                  initScene(Scene *scene);
    void                  closeScene(Scene *scene);
}

#endif // PHYSICS_H
