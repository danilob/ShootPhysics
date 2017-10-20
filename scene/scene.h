#ifndef SCENE_H
#define SCENE_H
#include "physics/physics.h"
#include "math/vec4.h"

class Object;
class GLWidget;
class Camera;


using namespace std;

class Scene
{
    //physics
    WorldID        world;
    SpaceID        space;
    JointGroupID   contactGroup;

    int            sim_step;       //passo da simulação
    bool           sim_status;

    int            width,height;
    Vec4           projection;

    Camera*        cam;
    Vec4           ori_cannon;
    vector<Object*> all_objects;

    vector<Vec4> trajetory;

    float          velocity_ball;


public:
    Scene();
    ~Scene();

    //scene
    void           setWindow(int width,int height);
    int            getWidth();
    int            getHeight();
    void           setProjection(Vec4 p);
    void           applyProjection();
    void           secundaryView();

    //Physics
    SpaceID        getSpace();                               //retorna o espaço veinculado ao cenário
    void           setSpace(SpaceID space);                  //seta um espaço ao cenário
    WorldID        getWorld();                               //retorna o mundo veinculado ao cenário
    void           setWorld(WorldID world);                  //seta um mundo ao cenário
    JointGroupID   getJointGroup();                          //retorna o grupo de juntas veinculado ao cenário
    void           setJointGroup(JointGroupID contactGroup); //seta um grupo de juntas ao cenário
    void           simulationStep();                         //executa um passo da simulação
    void           initPhysics();
    void           stopPhysics();
    void           startPhysics();

    void           createObject(int type, float mass, Vec4 scale=Vec4(1,1,1), Vec4 position=Vec4(), QuaternionQ orientation=QuaternionQ(), Vec4 color=Vec4(0.9,0.8,1), bool physics=true); //cria um objeto qquer
    void           defaultScene();
    void           draw();
    void           drawShadow();
    Camera*        getCamera();
    void           lookAt();

    void           drawCannon();
    void           getTrajetory();



    //manipulation
    void           addForce(Vec4 value, int id_object);
    void           addTorque(Vec4 value, int id_object);
    void           updateOriCannon(Vec4 ori);
    void           shotBall();
    void           increaseVelocity();
    void           decreaseVelocity();

};

#endif // SCENE_H
