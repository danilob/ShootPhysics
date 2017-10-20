#ifndef OBJECT_H
#define OBJECT_H
#include <QString>
#include <physics/physics.h>
#include <math/vec4.h>
#include <math/quaternion.h>

class Scene;
class Ray;
class Material;
class Matrix4x4;
class MatrixF;
class Mesh;
class ObjectDebug;


//tipos de geometrias definidas no ODE
#define TYPE_CUBE      dBoxClass
#define TYPE_CYLINDER  dCylinderClass
#define TYPE_SPHERE    dSphereClass

class Object
{
    Scene        *scene;                                                  //cenário do objeto
    QString      name;                                                    //nome do objeto

    // --------Physics atributos físicos
    BodyID       body;                                                    //body ODE
    GeomID       geometry;                                                //geom ODE
    Mass         *mass;                                                   //massa do objeto ODE
   

    // --------Geometry
    Vec4         position;                                                //posição inicial do centro do objeto
    QuaternionQ  orientation;                                             //rotação inicial do objeto
    Vec4         scale;                                                   //propriedades de dimensão do objeto
    Vec4         color;                                                   //cor do objeto
    int          type;                                                    //tipo do objeto em vinculação com o ODE
    float        fmass;                                                   //massa em float na inicialização

    bool         hasPhysics;
    

public:
    Object();
    ~Object();
    Object(Scene* scn,Vec4 position,QuaternionQ orientation, Vec4 scale, int type, float mass, Vec4 color, bool withPhysics=true);
    Object(Scene* scn,Vec4 position,QuaternionQ orientation, Vec4 scale, int type, float mass, Vec4 color, Vec4 velocity);

    Scene *getScene() const;
    void setScene(Scene *value);
    QString getName() const;
    void setName(const QString &value);
    Vec4 getPosition() const;
    void setPosition(const Vec4 &value);
    QuaternionQ getOrientation() const;
    void setOrientation(const QuaternionQ &value);
    Vec4 getScale() const;
    void setScale(const Vec4 &value);
    Vec4 getColor() const;
    void setColor(const Vec4 &value);
    int getType() const;
    void setType(int value);
    float getFmass();
    void setFmass(float value);
    bool getHasPhysics() const;
    void setHasPhysics(bool value);

    //propriedades físicas
    BodyID          getBody();                                                 //esta parametro de corpo do ODE
    void            setBody(BodyID body);                                      //extraí parâmetro de corpo do ODE
    GeomID          getGeometry();                                             //extraí geom do ODE
    void            setGeometry(GeomID geom);                                  //seta geom do ODE
    Mass*           getMass();                                                 //extraí atributo de massa do objeto
    void            setMass(Mass *mass);                                       //adiciona a massa ao objeto

    QuaternionQ     getOrientationCurrent();                                   //extraí a orientação corrente da simulação
    Vec4            getPositionCurrent();                                      //extraí a posição corrente da simulação

    //desenho
    void        draw();
    void        shadow();
};

#endif // OBJECT_H
