#include "scene.h"
#include "draw.h"
#include "camera.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "object.h"

Scene::Scene()
{
    Physics::initScene(this);
    cam = new Camera(Vec4(0,5,-32),Vec4(0,5,0),Vec4(0,1,0));
    this->sim_step = 67;
    this->sim_status = true;
    all_objects.clear();
    velocity_ball = 60.;
    show_second_viewport = true;
    getTrajetory();
}

void Scene::setWindow(int width, int height)
{
    this->width = width;
    this->height = height;
}

int Scene::getWidth()
{
    return width;
}

int Scene::getHeight()
{
    return height;
}

void Scene::setProjection(Vec4 p)
{
    projection = p;
}

void Scene::applyProjection()
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(projection.x(),projection.y(),projection.z(),projection.w());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Scene::secundaryView()
{
    glScissor(width-310, height-160, 300, 150);
            glEnable(GL_SCISSOR_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glLoadIdentity();
            glViewport(width-310, height-160, 300, 150);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
    glOrtho(-35,20,-5,20,-30,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //gluLookAt(10,5,0,0,0,0,0,0,1);

    glPushMatrix();
    glRotatef(90,0,1,0);
    glPushMatrix();

    Draw::SphereColor(Vec4(),QuaternionQ(),Vec4(0,0.5,0.5),0.2);
    Draw::GroundInfinity();
    Draw::TrajetoryBall(trajetory,Vec4(0.4,0.1,0.5));
    drawCannon();
    for(int i=0;i<all_objects.size();i++){
        all_objects.at(i)->draw();
    }

    glPopMatrix();
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glLineWidth(3.0);
    glColor3f(.5,.7,1);
    glBegin(GL_LINES);
        glVertex2f(-34.7,-5);
        glVertex2f(-34.7,30);

        glVertex2f(19.7,-5);
        glVertex2f(19.7,30);

        glVertex2f(19.7,-4.7);
        glVertex2f(-34.7,-4.7);

        glVertex2f(19.7,19.7);
        glVertex2f(-34.7,19.7);
    glEnd();


    Draw::Text(QString().sprintf("%d m/s",(int)velocity_ball),0,0,300,150,-0.97,-0.92,Vec4(0.5,0.2,0),1);

    glEnable(GL_LIGHTING);
    glDisable(GL_SCISSOR_TEST);
}

SpaceID Scene::getSpace()
{
    return this->space;
}

void Scene::setSpace(dSpaceID space)
{
    this->space = space;
}

WorldID Scene::getWorld()
{
    return this->world;
}

void Scene::setWorld(dWorldID world)
{
    this->world = world;

}

dJointGroupID Scene::getJointGroup()
{
    return this->contactGroup;
}

void Scene::setJointGroup(dJointGroupID contactGroup)
{
    this->contactGroup = contactGroup;
}

void Scene::simulationStep()
{
    if(!sim_status) return; //se a simulação físicanão estiver habilitada

    for(int i=0;i<this->sim_step;i++){
        Physics::simSingleStep(this);
        for(unsigned int i=0;i<all_objects.size();i++){
            Physics::enableObject(all_objects.at(i));
        }

    }

}

void Scene::initPhysics()
{
    Physics::initScene(this);

}

void Scene::stopPhysics()
{
    sim_status = false;
}

void Scene::startPhysics()
{
    sim_status = true;
}

void Scene::createObject(int type,float mass,Vec4 scale, Vec4 position, QuaternionQ orientation, Vec4 color, bool physics)
{
    Object *obj = new Object(this,position,orientation,scale,type,mass,color,physics);
    all_objects.push_back(obj);
}

void Scene::defaultScene()
{
    float dist = 12;
    //desenho da bancada
    this->createObject(TYPE_CUBE,1000,Vec4(10,3,4),Vec4(0,1.5,dist),QuaternionQ());
    //desenho dos cilindros da base da torre
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(0,4.725,dist),QuaternionQ(Vec4(90,0,0)),Vec4(0.9,0,0.8));
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(2.5,4.725,dist),QuaternionQ(Vec4(90,0,0)),Vec4(0.9,0,0.8));
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(-2.5,4.725,dist),QuaternionQ(Vec4(90,0,0)),Vec4(0.9,0,0.8));
    //desenho dos cilindros do meio
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(1.25,6.325,dist),QuaternionQ(Vec4(90,0,0)),Vec4(0.6,1,0.7));
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(-1.25,6.325,dist),QuaternionQ(Vec4(90,0,0)),Vec4(0.6,1,0.7));
    //desenho do cilindro do topo
    this->createObject(TYPE_CYLINDER,5,Vec4(0.7,1.5,0.7),Vec4(0,7.83,dist),QuaternionQ(Vec4(90,0,0)),Vec4(1,0.6,0));

}

void Scene::draw(){
    applyProjection();
    lookAt();
    glPushMatrix();

    Draw::SphereColor(Vec4(),QuaternionQ(),Vec4(0,0.5,0.5),0.2);
    Draw::GroundInfinity();
    Draw::TrajetoryBall(trajetory,Vec4(0.4,0.1,0.5));
    drawCannon();
    for(int i=0;i<all_objects.size();i++){
        all_objects.at(i)->draw();
    }

    glPopMatrix();
    Draw::Shadow(this);

    if(show_second_viewport)
        secundaryView();
}

void Scene::drawShadow()
{
    for(int i=0;i<all_objects.size();i++)
        all_objects.at(i)->shadow();
}


Camera *Scene::getCamera()
{
    return this->cam;
}

void Scene::lookAt()
{
    gluLookAt(cam->eye.x(),cam->eye.y(),cam->eye.z(),cam->at.x(),cam->at.y(),cam->at.z(),cam->up.x(),cam->up.y(),cam->up.z());
}

void Scene::drawCannon()
{
    Draw::Cylinder(Vec4(0,3,-27),QuaternionQ(ori_cannon),Vec4(1,4,1),Vec4(0.7,0.1,0));
}

void Scene::getTrajetory()
{
    trajetory.clear();
    Vec4 position = Vec4(0,3,-27)+Vec4(0,0,2.0);
    position = QuaternionQ::getVecRotation(QuaternionQ(ori_cannon),position-Vec4(0,3,-27));
    Vec4 velocity = QuaternionQ::getVecRotation(QuaternionQ(ori_cannon),Vec4(0,0,1))*velocity_ball;
    Object *obj = new Object(this,position+Vec4(0,3,-27),QuaternionQ(),Vec4(1,1,1),TYPE_SPHERE,2.0,Vec4(0.8,1,0),velocity);
    for(int i=0;i<this->sim_step*60;i++){
        Physics::simSingleStep(this);
        Physics::enableObject(obj);
        if(((position+Vec4(0,3,-27))-obj->getPositionCurrent()).module()>10) break;
        for(unsigned int i=0;i<all_objects.size();i++){
            Physics::disableObject(all_objects.at(i));
        }
        Vec4 traj = obj->getPositionCurrent();
        trajetory.push_back(traj);
    }
    Physics::closeObject(obj);
    delete obj;
}

void Scene::enableSecondViewport(bool b)
{
    show_second_viewport = b;
}

bool Scene::isEnabledSecondViewport()
{
    return show_second_viewport;
}

void Scene::addForce(Vec4 value, int id_object)
{
    Physics::bodyAddForce(all_objects.at(id_object),value);
}

void Scene::addTorque(Vec4 value, int id_object)
{
    Physics::bodyAddTorque(all_objects.at(id_object),value);
}

void Scene::updateOriCannon(Vec4 ori)
{
    ori_cannon += ori;
    getTrajetory();
}

void Scene::shotBall()
{
    Vec4 position = Vec4(0,3,-27)+Vec4(0,0,2.0);
    position = QuaternionQ::getVecRotation(QuaternionQ(ori_cannon),position-Vec4(0,3,-27));
    Vec4 velocity = QuaternionQ::getVecRotation(QuaternionQ(ori_cannon),Vec4(0,0,1))*velocity_ball;
    Object *obj = new Object(this,position+Vec4(0,3,-27),QuaternionQ(),Vec4(1,1,1),TYPE_SPHERE,2.0,Vec4(0.8,1,0),velocity);
    all_objects.push_back(obj);
}

void Scene::increaseVelocity()
{
    velocity_ball++;
    getTrajetory();
}

void Scene::decreaseVelocity()
{
    velocity_ball--;
    getTrajetory();
}
