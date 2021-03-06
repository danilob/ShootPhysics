#ifndef VEC4_H
#define VEC4_H
#include <stdio.h>
#include <math.h>
#include <QtOpenGL>
class Vec4
{
public:
    float x1,x2,x3,x4;
public:
    //construtor
    Vec4(float x=0.0,float y=0.0,float z=0.0,float w=1.0);
    //operadores
    bool                   operator == (Vec4 v);
    bool                   operator != (Vec4 v);
    Vec4                   operator + (Vec4 v);                    //soma de dois vetores
    void                   operator += (Vec4 v);                    //soma de dois vetores
    void                   operator -= (Vec4 v);                    //soma de dois vetores
    void                   operator /= (float k);                    //soma de dois vetores
    Vec4                   operator ^ (Vec4 v);                    //produto vetorial entre dois vetores
    Vec4                   operator - (Vec4 v);                    //subtração de dois vetores
    Vec4                   operator / (float k);                   //divisão das componentes do vetor por um numero real
    Vec4                   operator * (float k);                   //produto das componentes do vetor por um numero real
    float                  operator * (Vec4 v);                    //produto escalar entre dois vetores (dot)
    //funções
    Vec4                   mult(Vec4 v); //multiplica ordenada por ordenada
    void                   setVec4(Vec4 p);                        //adiciona um vetor
    void                   setVec4(float x1=0.0,float x2=0.0,float x3=0.0,float x4=1.0);
    float                  x();                                    //retorna parametro x1
    void                   setX(float x);                          //seta parametro x1
    float                  y();                                    //retorna parametro x2
    void                   setY(float y);                          //seta parametro x2
    float                  z();                                    //retorna parametro x3
    void                   setZ(float z);                          //seta parametro x3
    float                  w();                                    //retorna parametro x4
    void                   setW(float w);                          //seta parametro x4
    void                   normalize();                            //normaliza o vetor

    Vec4                   onlyX();
    Vec4                   onlyY();
    Vec4                   onlyZ();

    float                  module();                              //retorna o modulo do vetor
    Vec4                   unitary();                             //retorna o vetor unitário
    void                   showVec4();                            //imprime os valores do vetor
    Vec4                   projXZ();                              //retorna o vetor projetado no plano xz, y=0
    //metódos estáticos
    static Vec4            crossProduct(Vec4 a, Vec4 b);          //retorna o resultado do produto vetorial entre dois vetores

    static Vec4            projectionPlane(Vec4 point, Vec4 normal, Vec4 point_plane);
    static Vec4            orthogonal(Vec4 v);
    static float           dot(Vec4 a,Vec4 b);


};


#endif // VEC4_H
