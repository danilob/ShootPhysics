#include "quaternion.h"
#include "math.h"
#include "ode/ode.h"

//---------------------------------------------------------------------------
//Construtor
float radToGrau(float angle){
    return (180*angle)/M_PI;
}

float grauToRad(float angle){
    return (angle*M_PI)/180;

}

QuaternionQ::QuaternionQ()
{
  this->w = 1.0;
  this->x = 0.0;
  this->y = 0.0;
  this->z = 0.0;
}
//---------------------------------------------------------------------------
//Construtor
QuaternionQ::QuaternionQ( float w, Vec4 v )
{
  this->w = w;
    this->setVector(v);
}

QuaternionQ::QuaternionQ(float thetaX, float thetaY, float thetaZ)
{
    thetaX = (thetaX / 2.0) * (M_PI / 180.0);
    thetaY = (thetaY / 2.0) * (M_PI / 180.0);
    thetaZ = (thetaZ / 2.0) * (M_PI / 180.0);

    QuaternionQ qThetaX(cos(thetaX), sin(thetaX), 0.0, 0.0);
    QuaternionQ qThetaY(cos(thetaY), 0.0, sin(thetaY), 0.0);
    QuaternionQ qThetaZ(cos(thetaZ), 0.0, 0.0, sin(thetaZ));

    QuaternionQ q = qThetaZ* qThetaY;
    q = q*qThetaX;
    q.normalize();
    //QuaternionQ norm = q;

    *this = q;
}
//---------------------------------------------------------------------------
//Construtor
QuaternionQ::QuaternionQ( Vec4 euler) //ordem XYZ
{
  fromEuler( euler );
}
//QuaternionQ::Quaternion(float r, Vec4 v)
//{
//    this->w = r;
//    this->x = v.x();
//    this->y = v.y();
//    this->z = v.z();
//}
QuaternionQ::QuaternionQ(float r, float x, float y, float z)
{
    this->w = r;
    this->x = x;
    this->y = y;
    this->z = z;
}
//---------------------------------------------------------------------------
//Destrutor
QuaternionQ::~QuaternionQ()
{
}

void QuaternionQ::setVector(Vec4 v)
{
    this->x = v.x();
    this->y = v.y();
    this->z = v.z();
}
//---------------------------------------------------------------------------
//retorna o quaternion que leva de qi para qf
QuaternionQ QuaternionQ::deltaQuat( QuaternionQ quatf, QuaternionQ quati ) {
//  dQuaternion q,qit,qf; //qit - qi transposto (inverso, conjugado)
//  //qit
//    qit[0] = quati.w();
//    qit[1] = -quati.x;
//    qit[2] = -quati.xyz.y();
//    qit[3] = -quati.xyz.z();
//  //qf
//    qf[0] = quatf.w;
//    qf[1] = quatf.xyz.x();
//    qf[2] = quatf.xyz.y();
//    qf[3] = quatf.xyz.z();
//  //q
    //dQMultiply0 (q, qit, qf); //ou o contrario
//    dQMultiply0 (q, qf, qit); //ou o contrario
  return quatf*quati.conjugate();
  //return Quaternion( q[0], Vec4(q[1],q[2],q[3]) );
}
//---------------------------------------------------------------------------
//retorna o modulo do quaternion
float QuaternionQ::module() {
  return sqrt(this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
}
//---------------------------------------------------------------------------
//retorna o quadrado do modulo do quaternion (nao tira a raiz quadrada)
float QuaternionQ::module2() {
  return (this->w * this->w + this->x * this->x + this->y * this->y + this->z * this->z);
}
//---------------------------------------------------------------------------
//normaliza o quaternion
void QuaternionQ::normalize() {
  float m = this->module();

  if (m > 0.0) {
    m = 1 / m;

    this->w *= m;     // W
    this->x *= m; // X
    this->y *= m; // Y
    this->z *= m; // Z
  }
}

QuaternionQ QuaternionQ::normalizeR()
{
    this->normalize();
    return *this;
}
//---------------------------------------------------------------------------
//retorna o produto escalar entre this e quat
float QuaternionQ::dot(QuaternionQ quat) {
    return (this->w*quat.qw() + this->x*quat.qx() + this->y*quat.qy() + this->z*quat.qz());
}
//---------------------------------------------------------------------------
//retorna -quat
QuaternionQ QuaternionQ::minusQuaternion(QuaternionQ quat) {
//  quat.w = -quat.w;
//  quat.xyz.x1 = -quat.xyz.x();
//  quat.xyz.x2 = -quat.xyz.y();
//  quat.xyz.x3 = -quat.xyz.z();

  return QuaternionQ(-quat.qw(),-quat.qx(),-quat.qy(),-quat.qz());
}
//---------------------------------------------------------------------------
//calcula o menor arco (entre: this ate q ou this ate -q)
QuaternionQ QuaternionQ::lessArc(QuaternionQ q) {
  if ( this->dot(q) >= this->dot(minusQuaternion(q)) ) { //agora foi do jeito que eu tinha calculado - nao sei pq na implementacao de interpolacao de quaternions foi diferente
    return q;
  }
  else {
    return minusQuaternion(q);
  }
}
float QuaternionQ::getScalar()
{
    return this->w;
}

float QuaternionQ::getPosX()
{
    return this->x;
}

float QuaternionQ::getPosY()
{
    return this->y;
}

float QuaternionQ::getPosZ()
{
    return this->z;
}

float QuaternionQ::qw()
{
    return this->w;
}

float QuaternionQ::qx()
{
    return this->x;
}

float QuaternionQ::qy()
{
    return this->y;
}

float QuaternionQ::qz()
{
    return this->z;
}
void QuaternionQ::setQuaternion(float r, float x, float y, float z)
{
    this->w = r;
    this->x = x;
    this->y = y;
    this->z = z;
}
//---------------------------------------------------------------------------
//interpolacao linear esferica
QuaternionQ QuaternionQ::slerp( QuaternionQ quat, float t ) {
  QuaternionQ qt = QuaternionQ();

  quat = lessArc(quat);

  double omega,cosom,sinom,sclp,sclq;
  //int i;

  cosom = dot(quat);

  if ( (1.0 + cosom) > EPSILON ) {
    if ( (1.0 - cosom) > EPSILON ) {
        omega = acos(cosom);
        sinom = sin(omega);
        sclp = sin( (1.0 - t)*omega )/sinom;
        sclq = sin( t*omega )/sinom;
    }
    else {
        sclp = 1.0 - t;
        sclq = t;
    }
    //for (i=0;i<4;i++) qt.q[i] = sclp*q[i] + sclq*quat.q[i];
    qt.setPosX(sclp*this->x + sclq*quat.qx());
    qt.setPosY(sclp*this->y + sclq*quat.qy());
    qt.setPosZ(sclp*this->z + sclq*quat.qz());
    qt.setScalar(sclp*this->w + sclq*quat.qw());
  }
  else {
    //qt.q[X] = -q[Y]; qt.q[Y] = q[X];
    //qt.q[Z] = -q[W]; qt.q[W] = q[Z];
      qt.setPosX(-this->y);   qt.setPosY(this->x);
      qt.setPosZ( -this->w);       qt.w = this->z;
    sclp = sin((1.0 - t)*HALFPI);
    sclq = sin(t*HALFPI);
    //for (i=0;i<4;i++) qt.q[i] = sclp*q[i] + sclq*qt.q[i]; //i=0;i<3(livro) ou i=0;i<4(Zordan's code)?
    qt.setPosX(sclp*this->x + sclq*qt.qx());
    qt.setPosY(sclp*this->y + sclq*qt.qy());
    qt.setPosZ(sclp*this->z + sclq*qt.qz());
    qt.setScalar(sclp*this->w + sclq*qt.qw());
  }

  //normalizando qt
  qt.normalize();

  return qt;
}

void QuaternionQ::setQuaternion(QuaternionQ quat)
{
    this->w = quat.qw();
    this->x = quat.qx();
    this->y = quat.qy();
    this->z = quat.qz();
}

void QuaternionQ::setQuaternion(float w, Vec4 quat)
{
    this->w = w;
    this->x = quat.x();
    this->y = quat.y();
    this->z = quat.z();
}
QuaternionQ QuaternionQ::operator *(float k)
{
    QuaternionQ res;
    res.setScalar(this->w*k);
    res.setPosX(this->x*k);
    res.setPosY(this->y*k);
    res.setPosZ(this->z*k);
    return res;
}

QuaternionQ QuaternionQ::conjugate()
{
    return QuaternionQ(this->getScalar(),this->getVector()*(-1));

}
void QuaternionQ::setScalar(float x)
{
    this->w = x;
}

void QuaternionQ::setPosX(float x)
{
    this->x = x;
}

void QuaternionQ::setPosY(float x)
{
    this->y = x;
}

void QuaternionQ::setPosZ(float x)
{
    this->z = x;
}

Vec4 QuaternionQ::getVector()
{
    return Vec4(this->x,this->y,this->z);
}


QuaternionQ operator*(QuaternionQ q1, QuaternionQ q2) {

    QuaternionQ qr;
    qr.setScalar(q1.w*q2.w - q1.getPosX()*q2.getPosX() - q1.getPosY()*q2.getPosY() - q1.getPosZ()*q2.getPosZ());
    qr.setPosX(q1.w*q2.getPosX() + q2.w*q1.getPosX() + q1.getPosY()*q2.getPosZ() - q1.getPosZ()*q2.getPosY());
    qr.setPosY(q1.w*q2.getPosY() + q2.w*q1.getPosY() + q1.getPosZ()*q2.getPosX() - q1.getPosX()*q2.getPosZ());
    qr.setPosZ(q1.w*q2.getPosZ() + q2.w*q1.getPosZ() + q1.getPosX()*q2.getPosY() - q1.getPosY()*q2.getPosX());


    return qr;
//    return Quaternion(p.getScalar()*q.getScalar() - p.getPosX()*q.getPosX() - p.getPosY()*q.getPosY() - p.getPosZ()*q.getPosZ(),
//                      p.getScalar()*q.getPosX() + p.getPosX()*q.getScalar() + p.getPosY()*q.getPosZ() - p.getPosZ()*q.getPosY(),
//                      p.getScalar()*q.getPosY() - p.getPosX()*q.getPosZ() + p.getPosY()*q.getScalar() + p.getPosZ()*q.getPosX(),
//                      p.getScalar()*q.getPosZ() + p.getPosX()*q.getPosY() - p.getPosY()*q.getPosX() + p.getPosZ()*q.getScalar());
}

QuaternionQ operator+(QuaternionQ p, QuaternionQ q)
{
    return QuaternionQ(p.qw() + q.qw(), p.qx() + q.qx(), p.qy() + q.qy(), p.qz() + q.qz());
}
//---------------------------------------------------------------------------
//converte o quaternion em angulos de Euler (ordem XYZ)
Vec4 QuaternionQ::toEuler() {

    Vec4 euler;
    euler.x1 = atan2(2*(w*x+y*z),(1-2*(x*x+y*y)))*(180.0/M_PI);
    euler.x2 = asin(2*(w*y-z*x))*(180.0/M_PI);
    euler.x3 = atan2(2*(w*z+x*y),(1-2*(y*y+z*z)))*(180.0/M_PI);


  return euler;
}
//---------------------------------------------------------------------------
//converte os angulos de Euler em um quaternion e atribui a this
void QuaternionQ::fromEuler( Vec4 euler) {
    float thetaX,thetaY,thetaZ;
        thetaX = (euler.x() / 2.0) * (M_PI / 180.0);
        thetaY = (euler.y() / 2.0) * (M_PI / 180.0);
        thetaZ = (euler.z() / 2.0) * (M_PI / 180.0);

        QuaternionQ qThetaX(cos(thetaX), sin(thetaX), 0.0, 0.0);
        QuaternionQ qThetaY(cos(thetaY), 0.0, sin(thetaY), 0.0);
        QuaternionQ qThetaZ(cos(thetaZ), 0.0, 0.0, sin(thetaZ));

        QuaternionQ q = qThetaZ* qThetaY;
        q = q*qThetaX;
        //Quaternion norm = ;
        q.normalize();

        *this = q;
}

QuaternionQ QuaternionQ::fromEuler2Quat(Vec4 euler)
{
    QuaternionQ quat;
    quat.fromEuler(euler);
    return quat;

}
//---------------------------------------------------------------------------
//converte o quaternion em eixo e angulo (passados por referencia)
//baseado no site http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm


void QuaternionQ::showQuaternion()
{
    printf("Quaternion: (%.3f,%.3f,%.3f,%.3f)\n",this->w,this->x,this->y,this->z);
}
//---------------------------------------------------------------------------
//converte Quaternion em dQuaternion
//void QuaternionQ::to_dQuaternion( dQuaternion q ) {
//  q[0] = this->w;
//  q[1] = this->x;
//  q[2] = this->y;
//  q[3] = this->z;
//}
//---------------------------------------------------------------------------
//converte em Quaternion a partir de dQuaternion
//void QuaternionQ::from_dQuaternion( dQuaternion q ) {
//  this->w = q[0];
//  this->x = q[1];
//  this->y = q[2];
//  this->z = q[3];
//}
////---------------------------------------------------------------------------
////converte em Quaternion a partir de dQuaternion
//void QuaternionQ::from_dQuaternion( const dQuaternion q ) {
//  this->w = q[0];
//  this->x = q[1];
//  this->y = q[2];
//  this->z = q[3];
//}
////---------------------------------------------------------------------------
////convert dVector3 (normalized_axis*angle) to Quaternion
//void QuaternionQ::from_dVector3( dVector3 v ) {
//  Vec4 v3D( v[0],v[1],v[2] );
//    dReal angle = v3D.module();
//    v3D = v3D.unitary();
//  this->fromAxisAngle( v3D, radToGrau(angle) );
//}
////---------------------------------------------------------------------------
////convert Vec4 (normalized_axis*angle) to Quaternion
//void QuaternionQ::from_Vec4( Vec4 v3D ) {
//    dReal angle = v3D.module();
//    v3D = v3D.unitary();
//  this->fromAxisAngle( v3D, radToGrau(angle) );
//}
////---------------------------------------------------------------------------
////convert Quaternion to dVector3 (normalized_axis*angle)
//void QuaternionQ::to_dVector3( dVector3 v ) {
//  Vec4 axis;
//  dReal angle;
//    this->toAxisAngle( &axis, &angle );
//    angle = grauToRad( angle );
//  Vec4 v3D = axis*( angle );
//  //v3D.to_dVector3( v );
//}
////---------------------------------------------------------------------------
////convert Quaternion to Vec4 (normalized_axis*angle)
//void QuaternionQ::to_Vec4( Vec4& v3D ) {
//  Vec4 axis;
//  dReal angle;
//    this->toAxisAngle( &axis, &angle );
//    angle = grauToRad( angle );
//  v3D = axis*( angle );
//}
////---------------------------------------------------------------------------
Vec4 QuaternionQ::getVecRotation(QuaternionQ q, Vec4 v)
{
    QuaternionQ res;
    QuaternionQ pos(0,v.x1,v.x2,v.x3);
    q.normalize();
    res = (q)*(pos*((q.conjugate())));
    Vec4 r(res.getPosX(),res.getPosY(),res.getPosZ());
    return r;

}

float QuaternionQ::normal()
{
    float res;
    res = sqrt(this->getScalar()*this->getScalar()+
               this->getPosX()*this->getPosX()+
               this->getPosY()*this->getPosY()+
               this->getPosZ()*this->getPosZ());
    return res;
}

QuaternionQ QuaternionQ::getRotation(Vec4 u, Vec4 v)
{
    // It is important that the inputs are of equal length when
      // calculating the half-way vector.
      u = u.unitary();
      v = v.unitary();

      // Unfortunately, we have to check for when u == -v, as u + v
      // in this case will be (0, 0, 0), which cannot be normalized.
      if (u == v*(-1))
      {
        // 180 degree rotation around any orthogonal vector
        return QuaternionQ(0, Vec4::orthogonal(u).unitary());
      }

      Vec4 half = (u + v).unitary();
      return QuaternionQ().lessArc(QuaternionQ(Vec4::dot(u, half), Vec4::crossProduct(u, half).unitary()));
}

QuaternionQ QuaternionQ::inverse()
{
    QuaternionQ q = (*this);
    q =  q.conjugate();
    return q / QuaternionQ::dot((*this), (*this));
}

void QuaternionQ::toAxisAngle(Vec4 *axis, float *angle)
{
    *angle = 2*acos(this->w);
    float s = 1 - this->w*this->w; //0<=s<=1
    if (s > 0.0) { //evita divisao por zero
      s = dRecipSqrt( s );
      axis->x1 = this->x * s;
      axis->x2 = this->y * s;
      axis->x3 = this->z * s;
    }
    else { //s==0 => this->w==1 => angle==0 => nao importa o eixo (define eixo arbitrario)
      *angle = 0;
      axis->x1 = 1;
      axis->x2 = 0;
      axis->x3 = 0;
    }

    //radToGrau
    *angle = (180.0/3.14159265359)*(*angle);

    //truncando em 10 casas decimais
    //*
    *angle = trunc((*angle)*pow(10.0,10.0))/pow(10.0,10.0);
    axis->x1 = trunc(axis->x()*pow(10.0,10.0))/pow(10.0,10.0);
    axis->x2 = trunc(axis->y()*pow(10.0,10.0))/pow(10.0,10.0);
    axis->x3 = trunc(axis->z()*pow(10.0,10.0))/pow(10.0,10.0);
}

bool operator==(QuaternionQ p, QuaternionQ q)
{
    if(p.getScalar()==q.getScalar() && p.getPosX()==q.getPosX() && p.getPosY()==q.getPosX() && p.getPosZ()==q.getPosZ()) return true;
    return false;
}



QuaternionQ operator-(QuaternionQ p, QuaternionQ q)
{
    return QuaternionQ(p.getScalar() - q.getScalar(), p.getPosX() - q.getPosX(), p.getPosY() - q.getPosY(), p.getPosZ() - q.getPosZ());
}

//Quaternion QuaternionQ::operator -(Quaternion q)
//{
//    Quaternion res;
//    res.setScalar(this->getScalar()-q.getScalar());
//    res.setPosX(this->getPosX()-q.getPosX());
//    res.setPosY(this->getPosY()-q.getPosY());
//    res.setPosZ(this->getPosZ()-q.getPosZ());
//    return res;
//}


QuaternionQ QuaternionQ::operator /(double k)
{
    QuaternionQ res;
    res.setScalar(this->getScalar()/k);
    res.setPosX(this->getPosX()/k);
    res.setPosY(this->getPosY()/k);
    res.setPosZ(this->getPosZ()/k);
    return res;
}

float QuaternionQ::dot(QuaternionQ p, QuaternionQ q)
{
    return sqrt(p.getScalar()*q.getScalar() + p.getPosX()*q.getPosX() + p.getPosY()*q.getPosY() + p.getPosZ()*q.getPosZ());
}

//Quaternion QuaternionQ::operator *(Quaternion q)
//{
//    //q1q2 = (s1 , v1)(s2 , v2) = (s1*s2 − v1⋅v 2 , s1v2 + s2v1 + v1 × v2 )
//    Quaternion res;
//    Vec4 v;
//    res.setScalar(this->getScalar()*q.getScalar() - this->getVector()*this->getVector());
//    v.setVec4(q.getVector()*this->getScalar()+this->getVector()*q.getScalar()+Vec4::crossProduct(this->getVector(),q.getVector()));
//    res.setPosX(v.x1);
//    res.setPosY(v.x2);
//    res.setPosZ(v.x3);
//    return res;
//}

//Quaternion operator*(Quaternion q1, Quaternion q2) {

//    Quaternion qr;
//    qr.setScalar(q1.getScalar()*q2.getScalar() - q1.getPosX()*q2.getPosX() - q1.getPosY()*q2.getPosY() - q1.getPosZ()*q2.getPosZ());
//    qr.setPosX(q1.getScalar()*q2.getPosX() + q2.getScalar()*q1.getPosX() + q1.getPosY()*q2.getPosZ() - q1.getPosZ()*q2.getPosY());
//    qr.setPosY(q1.getScalar()*q2.getPosY() + q2.getScalar()*q1.getPosY() + q1.getPosZ()*q2.getPosX() - q1.getPosX()*q2.getPosZ());
//    qr.setPosZ(q1.getScalar()*q2.getPosZ() + q2.getScalar()*q1.getPosZ() + q1.getPosX()*q2.getPosY() - q1.getPosY()*q2.getPosX());

//    return qr;
////    return Quaternion(p.getScalar()*q.getScalar() - p.getPosX()*q.getPosX() - p.getPosY()*q.getPosY() - p.getPosZ()*q.getPosZ(),
////                      p.getScalar()*q.getPosX() + p.getPosX()*q.getScalar() + p.getPosY()*q.getPosZ() - p.getPosZ()*q.getPosY(),
////                      p.getScalar()*q.getPosY() - p.getPosX()*q.getPosZ() + p.getPosY()*q.getScalar() + p.getPosZ()*q.getPosX(),
////                      p.getScalar()*q.getPosZ() + p.getPosX()*q.getPosY() - p.getPosY()*q.getPosX() + p.getPosZ()*q.getScalar());
//}

//Quaternion operator/(Quaternion p, Quaternion q) {
//    return p * q.inverse();
//}
