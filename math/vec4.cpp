#include "vec4.h"

Vec4::Vec4(float x1,float x2,float x3,float x4)
{
    this->x1 = x1;
    this->x2 = x2;
    this->x3 = x3;
    this->x4 = x4;
}

void Vec4::setVec4(Vec4 p){
    this->x1 = p.x1;
    this->x2 = p.x2;
    this->x3 = p.x3;
}

bool Vec4::operator == (Vec4 v){
    if ((this->x1==v.x1)&&(this->x2==v.x2)&&(this->x3==v.x3)){
        return true;
    }else{
        return false;
    }
}

bool Vec4::operator != (Vec4 v){
    if ((this->x1==v.x1)&&(this->x2==v.x2)&&(this->x3==v.x3)){
        return false;
    }else{
        return true;
    }
}

Vec4 Vec4::operator + (Vec4 v){
    Vec4 r;
    r.x1 = this->x1+v.x1;
    r.x2 = this->x2+v.x2;
    r.x3 = this->x3+v.x3;
    return r;
}

void Vec4::operator +=(Vec4 v)
{
    this->x1 = this->x1+v.x();
    this->x2 = this->x2+v.y();
    this->x3 = this->x3+v.z();
}

void Vec4::operator -=(Vec4 v)
{
    this->x1 = this->x1-v.x();
    this->x2 = this->x2-v.y();
    this->x3 = this->x3-v.z();
}

void Vec4::operator /=(float k)
{
    this->x1 = this->x1/k;
    this->x2 = this->x2/k;
    this->x3 = this->x3/k;
}

Vec4 Vec4::operator - (Vec4 v){
    Vec4 r;
    r.x1 = this->x1-v.x1;
    r.x2 = this->x2-v.x2;
    r.x3 = this->x3-v.x3;
    return r;
}

Vec4 Vec4::operator / (float k){
    Vec4 r;
    r.x1 = this->x1/k;
    r.x2 = this->x2/k;
    r.x3 = this->x3/k;
    return r;
}


Vec4 Vec4::operator * (float k){
    Vec4 r;
    r.x1 = this->x1*k;
    r.x2 = this->x2*k;
    r.x3 = this->x3*k;
    return r;
}


float Vec4::operator * (Vec4 v){
    return this->x1*v.x1+this->x2*v.x2+this->x3*v.x3;
}

Vec4 Vec4::mult(Vec4 v)
{
    return Vec4(this->x()*v.x(),this->y()*v.y(),this->z()*v.z());
}


void Vec4::setVec4(float x1,float x2, float x3, float x4){
    this->x1 = x1;
    this->x2 = x2;
    this->x3 = x3;
    this->x4 = x4;
}

float Vec4::module(){
    return sqrt(pow(x1,2)+pow(x2,2)+pow(x3,2));
}

Vec4 Vec4::unitary(){
    return *this/this->module();
}

void Vec4::showVec4(){
    printf("(%.4f;%.4f;%.4f;%.4f)\n",x1,x2,x3,x4);
}



Vec4 Vec4::crossProduct(Vec4 a,Vec4 b){
    Vec4 r;
    r.x1 = a.x2*b.x3 - b.x2*a.x3;
    r.x2 = a.x3*b.x1 - b.x3*a.x1;
    r.x3 = a.x1*b.x2 - a.x2*b.x1;
    return r;
}

Vec4 Vec4::projectionPlane(Vec4 point, Vec4 normal, Vec4 point_plane)
{
    Vec4 dir(0,-1,0);
    float d = point_plane.x()*normal.x() + point_plane.y()*normal.y() + point_plane.z()*normal.z();
    float t = (- d - (normal.x()*point.x()+normal.y()*point.y()+normal.z()*point.z()))/(normal.x()*dir.x()+normal.y()*dir.y()+normal.z()*dir.z());
    return point + dir*t;
}

Vec4 Vec4::orthogonal(Vec4 v)
{
    float x = abs(v.x());
    float y = abs(v.y());
    float z = abs(v.z());

    Vec4 other = x < y ? (x < z ? Vec4(1,0,0) : Vec4(0,0,1)) : (y < z ? Vec4(0,1,0) : Vec4(0,0,1));
    return Vec4::crossProduct(v, other);
}

float Vec4::dot(Vec4 a, Vec4 b)
{
    return a.x()*b.x()+a.y()*b.y()+a.z()*b.z();
}

Vec4 Vec4::operator ^(Vec4 b){
    Vec4 r;
    r.x1 = this->x2*b.x3 - b.x2*this->x3;
    r.x2 = this->x3*b.x1 - b.x3*this->x1;
    r.x3 = this->x1*b.x2 - this->x2*b.x1;
    return r;
}


float Vec4::x()
{
    return x1;
}

void Vec4::setX(float x)
{
    this->x1 = x;
}

float Vec4::y()
{
    return x2;
}

void Vec4::setY(float y)
{
    this->x2 = y;
}

float Vec4::z()
{
    return x3;
}

void Vec4::setZ(float z)
{
    this->x3 = z;
}

float Vec4::w()
{
    return x4;
}

void Vec4::setW(float w)
{
    this->x4 = w;
}

void Vec4::normalize()
{
    float mod = this->module();
    this->x1 = this->x()/mod;
    this->x2 = this->y()/mod;
    this->x3 = this->z()/mod;
}

Vec4 Vec4::onlyX()
{
    return Vec4(this->x(),0,0);
}

Vec4 Vec4::onlyY()
{
    return Vec4(0,this->y(),0);
}

Vec4 Vec4::onlyZ()
{
    return Vec4(0,0,this->z());
}

Vec4 Vec4::projXZ()
{
    Vec4 r(this->x1,0,this->x3);
    return r;
}
