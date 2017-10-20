#include "physics.h"
#include "scene/object.h"
#include "scene/scene.h"
#include <iostream>
using namespace std;
//#endif

WorldID world_scene;
JointGroupID jg_scene;

static Scene* scn = NULL;
int count_collide = 0;
std::vector<dJointFeedback*> apagar;
void Physics::nearCallback(void *data, dGeomID o1, dGeomID o2){
    //dois corpos ligados por alguma junta nao colidem
    //if (b1 && b2 && dAreConnectedExcluding (b1,b2,dJointTypeContact)) return;
    //so ha colisao se uma das geometrias for tipo plano - no caso, soh ha colisao com o chao (nao ha nenhuma colisao entre os corpos)
    //      if (dGeomGetClass(o1)!=dPlaneClass && dGeomGetClass(o2)!=dPlaneClass) return;
    //caso a colisao seja entre as geomFixas (chao, paredes e teto), nao tratar a colisao na simulacao
    if (dGeomGetClass(o1)==dPlaneClass && dGeomGetClass(o2)==dPlaneClass) return;
    //      //duas geometrias pertencentes ao mesmo modelo nao colidem (nem dois objetos - data==Objeto::objetos(FF)) (nem duas geomFixas - data==NULL)
    //if (dGeomGetData(o1)==dGeomGetData(o2)) return;
    //      //duas geometrias pertencentes ao mesmo modelo nao colidem (nem duas geomFixas - data==NULL)
    //if (dGeomGetData(o1)==dGeomGetData(o2))
    //permite colisao entre box e cilindro, mesmo pertencentes ao mesmo modelo
    //if ( (dGeomGetClass(o1)==dCylinderClass && dGeomGetClass(o2)==dCylinderClass) ) dSpaceCollide2 (o1, o2, data,&nearCallback);
    //return;

    if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) {
        // Collide a space with everything else
        dSpaceCollide2 (o1, o2, data,&nearCallback);
    }else{
        int numcont = 4;
        dContact contact[numcont];
        int i;
        if (int numc = dCollide(o1,o2,numcont,&contact[0].geom,sizeof(dContact))) {

            for(i=0; i<numcont; i++){
                contact[i].surface.mode = dContactApprox1|dContactSoftERP|dContactSoftCFM;
                // friction parameter
                contact[i].surface.mu = 0.8; //dInfinity;
                contact[i].surface.soft_erp = 0.02;//1.0; //1.0;
                contact[i].surface.soft_cfm = 1e-009;//1e-10;
            }

            dMatrix3 RI;
            dRSetIdentity (RI);
            //const dReal ss[3] = {0.02,0.02,0.02};
            for (i=0; i<numc; i++) {
                dJointID c = dJointCreateContact (world_scene,jg_scene,&contact[i]);
                //criar as juntas de contato
                dJointAttach (c, dGeomGetBody (contact[i].geom.g1), dGeomGetBody (contact[i].geom.g2));
            }
        }
    }
}

void Physics::worldStep(WorldID world, float stepSize){
    dWorldQuickStep(world, stepSize);
}

void Physics::simSingleStep (Scene *scene)
{
    dSpaceCollide (scene->getSpace(),0,&nearCallback);
    // step the simulation
    dWorldStep (scene->getWorld(),0.0005);
    // remove all contact joints
    dJointGroupEmpty(scene->getJointGroup());

}

void Physics::createObject(Object *object, Vec4 velocity)
{
    switch(object->getType()){
    case TYPE_SPHERE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateSphere(object->getScene()->getSpace(),object->getScale().x()));
        dMassSetSphereTotal(object->getMass(),object->getFmass(),object->getScale().x());
        break;
    }
    case TYPE_CUBE:{
        object->setBody(dBodyCreate(object->getScene()->getWorld()));
        object->setGeometry(dCreateBox(object->getScene()->getSpace(),object->getScale().x(),object->getScale().y(),object->getScale().z()));
        dMassSetBoxTotal(object->getMass(),object->getFmass(),object->getScale().x(),object->getScale().y(),object->getScale().z());
        break;
    }
    case TYPE_CYLINDER:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateCylinder (object->getScene()->getSpace(),object->getScale().x(),object->getScale().z()));
        dMassSetCylinder(object->getMass(),object->getFmass(),1,object->getScale().x(),object->getScale().z());
        break;
    }
    default:{
        return;
        break;
    }
    }

    dGeomSetData(object->getGeometry(), (void*)(object));
    dBodySetMass (object->getBody(),object->getMass());
    dGeomSetBody (object->getGeometry(),object->getBody());
    Vec4 position = object->getPosition();
    dBodySetPosition (object->getBody(),position.x(),position.y(),position.z());
    dBodySetLinearVel(object->getBody(), velocity.x(), velocity.y(), velocity.z());

    dReal rot[] = {object->getOrientation().getScalar(),object->getOrientation().getPosX(),object->getOrientation().getPosY(),object->getOrientation().getPosZ()};
    dBodySetQuaternion (object->getBody(),rot);
}



void Physics::createObject(Object *object){

    switch(object->getType()){
    case TYPE_SPHERE:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateSphere(object->getScene()->getSpace(),object->getScale().x()));
        dMassSetSphereTotal(object->getMass(),object->getFmass(),object->getScale().x());
        break;
    }
    case TYPE_CUBE:{
        object->setBody(dBodyCreate(object->getScene()->getWorld()));
        object->setGeometry(dCreateBox(object->getScene()->getSpace(),object->getScale().x(),object->getScale().y(),object->getScale().z()));
        dMassSetBoxTotal(object->getMass(),object->getFmass(),object->getScale().x(),object->getScale().y(),object->getScale().z());
        break;
    }
    case TYPE_CYLINDER:{
        object->setBody(dBodyCreate (object->getScene()->getWorld()));
        object->setGeometry(dCreateCylinder (object->getScene()->getSpace(),object->getScale().x(),object->getScale().y()));
        dMassSetCylinderTotal(object->getMass(),object->getFmass(),1,object->getScale().x(),object->getScale().y());
        break;
    }
    default:{
        return;
        break;
    }
    }

    dGeomSetData(object->getGeometry(), (void*)(object));
    dBodySetMass (object->getBody(),object->getMass());
    dGeomSetBody (object->getGeometry(),object->getBody());
    Vec4 position = object->getPosition();
    dBodySetPosition (object->getBody(),position.x(),position.y(),position.z());
    QuaternionQ rotation = object->getOrientation();
    dReal rot[] = {rotation.getScalar(),rotation.getPosX(),rotation.getPosY(),rotation.getPosZ()};
    dBodySetQuaternion (object->getBody(),rot);
}

Vec4 Physics::getPositionBody(Object *obj){
    const dReal *pos;
    pos = dGeomGetPosition(obj->getGeometry());
    return Vec4(pos[0],pos[1],pos[2]);
}

QuaternionQ Physics::getOrientationBody(Object *obj){
    dQuaternion quat;
    dGeomGetQuaternion(obj->getGeometry(),quat);
    return QuaternionQ(quat[0],quat[1],quat[2],quat[3]);
}

void Physics::initScene(Scene *scene){

    dInitODE ();
    // create world
    scene->setWorld(dWorldCreate());

    dWorldSetGravity (scene->getWorld(),0,-9.8,0);
    //dWorldSetGravity (scene->world,0,0,0);
    dWorldSetERP (scene->getWorld(),0.2);
    //dWorldSetCFM (scene->world,1e-3);
    dWorldSetCFM (scene->getWorld(),10e-10);
    scene->setSpace(dHashSpaceCreate(0));
    //scene->Plane = dCreatePlane (scene->getSpace(),0,1,0,0); //todo remove

    dCreatePlane (scene->getSpace(),0,1,0,0); //todo remove
    scene->setJointGroup(dJointGroupCreate(0));

    dWorldSetAutoDisableFlag (scene->getWorld(),1);
    dWorldSetContactMaxCorrectingVel (scene->getWorld(),0.09);
    dWorldSetContactSurfaceLayer (scene->getWorld(),0.001);

    world_scene = scene->getWorld();
    jg_scene = scene->getJointGroup();

}

void Physics::closeScene(Scene *scene){
    // clean up
    dJointGroupDestroy (scene->getJointGroup());
    dSpaceDestroy (scene->getSpace());
    dWorldDestroy (scene->getWorld());

    //!Same with close
    dCloseODE();
}


void Physics::enableObject(Object *obj)
{
    dBodyEnable(obj->getBody());
}

void Physics::bodySetTorque(Object *obj, Vec4 value){
    dBodySetTorque(obj->getBody(),value.x(),value.y(),value.z());
}

void Physics::bodyAddForce(Object *obj, Vec4 value){
    dBodyAddForce(obj->getBody(),value.x(),value.y(),value.z());
}

void Physics::bodySetForce(Object *obj, Vec4 value){
    dBodySetForce(obj->getBody(),value.x(),value.y(),value.z());
}

void Physics::bodyAddTorque(Object *obj, Vec4 value){
        dBodyAddTorque(obj->getBody(),value.x(),value.y(),value.z());
}


void Physics::disableObject(Object *obj)
{
    dBodyDisable(obj->getBody());
}

void Physics::closeObject(Object *obj){
    dBodyDestroy(obj->getBody());
    dGeomDestroy(obj->getGeometry());
}
