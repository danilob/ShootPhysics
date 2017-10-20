#include "object.h"
#include "scene.h"
#include "physics/physics.h"
#include "draw.h"

Object::Object()
{
}

Object::~Object()
{
    delete mass;
}

Object::Object(Scene *scn, Vec4 position, QuaternionQ orientation, Vec4 scale, int type, float mass, Vec4 color, bool withPhysics)
{
    this->scene = scn;
    this->position = position;
    this->orientation = orientation;
    this->scale = scale;
    this->color = color;
    this->type = type;
    this->fmass = mass;
    this->mass  = new Mass();
    this->hasPhysics = withPhysics;

    if(withPhysics){
        Physics::createObject(this);
    }

}

Object::Object(Scene *scn, Vec4 position, QuaternionQ orientation, Vec4 scale, int type, float mass, Vec4 color, Vec4 velocity)
{
    this->scene = scn;
    this->position = position;
    this->orientation = orientation;
    this->scale = scale;
    this->color = color;
    this->type = type;
    this->fmass = mass;
    this->mass  = new Mass();
    this->hasPhysics = true;

    Physics::createObject(this,velocity);

}



Scene *Object::getScene() const
{
    return scene;
}

void Object::setScene(Scene *value)
{
    scene = value;
}

QString Object::getName() const
{
    return name;
}

void Object::setName(const QString &value)
{
    name = value;
}

Vec4 Object::getPosition() const
{
    return position;
}

void Object::setPosition(const Vec4 &value)
{
    position = value;
}

QuaternionQ Object::getOrientation() const
{
    return orientation;
}

void Object::setOrientation(const QuaternionQ &value)
{
    orientation = value;
}

Vec4 Object::getScale() const
{
    return scale;
}

void Object::setScale(const Vec4 &value)
{
    scale = value;
}

Vec4 Object::getColor() const
{
    return color;
}

void Object::setColor(const Vec4 &value)
{
    color = value;
}

int Object::getType() const
{
    return type;
}

void Object::setType(int value)
{
    type = value;
}

float Object::getFmass()
{
    return fmass;
}

void Object::setFmass(float value)
{
    fmass = value;
}

bool Object::getHasPhysics() const
{
    return hasPhysics;
}

void Object::setHasPhysics(bool value)
{
    hasPhysics = value;
}

//propriedades físicas
dBodyID Object::getBody()
{
    return this->body;
}

void Object::setBody(dBodyID body)
{
    this->body = body;
}

GeomID Object::getGeometry()
{
    return this->geometry;
}

void Object::setGeometry(GeomID geom)
{
    this->geometry = geom;
}

Mass *Object::getMass()
{
    return this->mass;
}

void Object::setMass(Mass *mass)
{
    this->mass = mass;
}

QuaternionQ Object::getOrientationCurrent()
{
    return Physics::getOrientationBody(this);
}

Vec4 Object::getPositionCurrent()
{
    return Physics::getPositionBody(this);
}

void Object::draw()
{
    switch(type){
        case TYPE_SPHERE:{
            if(hasPhysics){
                Draw::Sphere(this->getPositionCurrent(),this->getOrientationCurrent(),this->color,this->scale.x());
            }
            else{
                Draw::Sphere(this->getPosition(),this->getOrientation(),this->color,this->scale.x());
            }
            break;
        }
    case TYPE_CUBE:{
        if(hasPhysics){
            Draw::Box(this->getPositionCurrent(),this->getOrientationCurrent(),this->scale,this->color);
        }
        else{
            Draw::Box(this->getPosition(),this->getOrientation(),this->scale,this->color);
        }
        break;
    }
    case TYPE_CYLINDER:{
        if(hasPhysics){
            Draw::Cylinder(this->getPositionCurrent(),this->getOrientationCurrent(),this->scale,this->color);
        }
        else{
            Draw::Cylinder(this->getPosition(),this->getOrientation(),this->scale,this->color);
        }
        break;
    }

    }
}

void Object::shadow()
{
    switch(type){
    case TYPE_SPHERE:{
        if(hasPhysics){
            Draw::Sphere2Shadow(this->getPositionCurrent(),this->getOrientationCurrent(),this->scale.x());
        }
        else{
            Draw::Sphere2Shadow(this->getPosition(),this->getOrientation(),this->scale.x());
        }
        break;
    }
    case TYPE_CUBE:{
        if(hasPhysics){
            Draw::Box2Shadow(this->getPositionCurrent(),this->getOrientationCurrent(),this->scale);
        }
        else{
            Draw::Box2Shadow(this->getPosition(),this->getOrientation(),this->scale);
        }
        break;
    }
    case TYPE_CYLINDER:{
        if(hasPhysics){
            Draw::Cylinder2Shadow(this->getPositionCurrent(),this->getOrientationCurrent(),this->scale);
        }
        else{
            Draw::Cylinder2Shadow(this->getPosition(),this->getOrientation(),this->scale);
        }
        break;
    }
    }
}
