#include "draw.h"
#include "GL/glut.h"

#include "scene.h"


void Draw::GroundInfinity(int size, int steps)
{
    size = size/2;
    glDisable(GL_LIGHTING);
    glColor3f(0.9,0.9,0.9);
    glBegin(GL_QUADS);
    for(int i=-size;i<size;i+=steps){
        for(int j=-size;j<size;j+=steps){
            glVertex3f(i,0,j);
            glVertex3f(i,0,j+steps);
            glVertex3f(i+steps,0,j+steps);
            glVertex3f(i+steps,0,j);
        }
    }
//    glVertex3f(-size,0,-size);
//    glVertex3f(size,0,-size);
//    glVertex3f(size,0,size);
//    glVertex3f(-size,0,size);
    glEnd();

    glLineWidth(0.8);

    glColor3f(0.4,0.4,0.4);
    glBegin(GL_LINES);
    for(int i=-size;i<size;i+=steps){
        for(int j=-size;j<size;j+=steps){
            glVertex3f(i,0.000,j);
            glVertex3f(i,0.000,j+steps);

            glVertex3f(i,0.000,j+steps);
            glVertex3f(i+steps,0.000,j+steps);

            glVertex3f(i+steps,0.000,j+steps);
            glVertex3f(i+steps,0.000,j);

            glVertex3f(i+steps,0.000,j);
            glVertex3f(i,0.000,j);
        }
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glLineWidth(1.0);

}

void Draw::Sphere(Vec4 position, QuaternionQ orientation, Vec4 color, float radius){

    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float angle;
    orientation.toAxisAngle(&vec,&angle);
    glRotatef(angle,vec.x(),vec.y(),vec.z());
    glDisable(GL_LIGHTING);

    const float PI = 3.141592f;
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles
    int gradation = 10;

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glColor3f(color.x(),color.y(),color.z());
    for (alpha = 0.0; alpha < PI; alpha += PI/gradation)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (beta = 0.0; beta < 2.01*PI; beta += PI/gradation)
        {
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            glVertex3f(x, y, z);
            x = radius*cos(beta)*sin(alpha + PI/gradation);
            y = radius*sin(beta)*sin(alpha + PI/gradation);
            z = radius*cos(alpha + PI/gradation);
            glVertex3f(x, y, z);
        }
        glEnd();
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glColor3f(0,0,0);
    glLineWidth(radius*0.7);
    for (alpha = 0; alpha <= PI; alpha += PI/gradation)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (beta = 0.0; beta < 2.01*PI; beta += PI/gradation)
        {
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            glVertex3f(x, y, z);
            x = radius*cos(beta)*sin(alpha + PI/gradation);
            y = radius*sin(beta)*sin(alpha + PI/gradation);
            z = radius*cos(alpha + PI/gradation);
            glVertex3f(x, y, z);
        }
        glEnd();
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable(GL_LIGHTING);
    glPopMatrix();
}


void Draw::Sphere2Shadow(Vec4 position, QuaternionQ orientation, float radius)
{
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float angle;
    orientation.toAxisAngle(&vec,&angle);
    glRotatef(angle,vec.x(),vec.y(),vec.z());
    glDisable(GL_LIGHTING);

    const float PI = 3.141592f;
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles
    int gradation = 10;

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    for (alpha = 0.0; alpha < PI; alpha += PI/gradation)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (beta = 0.0; beta < 2.01*PI; beta += PI/gradation)
        {
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            glVertex3f(x, y, z);
            x = radius*cos(beta)*sin(alpha + PI/gradation);
            y = radius*sin(beta)*sin(alpha + PI/gradation);
            z = radius*cos(alpha + PI/gradation);
            glVertex3f(x, y, z);
        }
        glEnd();
    }



    glEnable(GL_LIGHTING);
    glPopMatrix();
}


void Draw::Shadow(Scene *scene)
{
    GLfloat shadowMat[4][4];
    GLfloat groundplane[4] = {0,1.,0,0};
    GLfloat lightpos[4] = {   -10, 50, 30, 1.0 };

    GLfloat dot;

    // Find dot product between light position vector and ground plane normal
    dot = groundplane[0] * lightpos[0] +
      groundplane[1] * lightpos[1] +
      groundplane[2] * lightpos[2] +
      groundplane[3] * lightpos[3];

    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.f - lightpos[0] * groundplane[3];

    shadowMat[0][1] = 0.f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.f - lightpos[1] * groundplane[3];

    shadowMat[0][2] = 0.f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.f - lightpos[2] * groundplane[3];

    shadowMat[0][3] = 0.f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];

    glPushMatrix();
          /* Draw the floor with stencil value 3.  This helps us only
             draw the shadow once per floor pixel (and only on the
             floor pixels). */
          glEnable(GL_STENCIL_TEST);
          glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
          glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


      /* Draw "top" of floor.  Use blending to blend in reflection (jah foi habilitado o blend). */
      Draw::GroundInfinity();

        /* Render the projected shadow. */

        /* Now, only render where stencil is set above 2 (ie, 3 where
             the top floor is).  Update stencil with 2 where the shadow
             gets drawn so we don't redraw (and accidently reblend) the
             shadow). */
        glStencilFunc(GL_LESS, 2, 0xffffffff);  /* draw if ==1 */
        glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

        /* To eliminate depth buffer artifacts, we use polygon offset
           to raise the depth of the projected shadow slightly so
           that it does not depth buffer alias with the floor. */
        //glEnable(GL_POLYGON_OFFSET_EXT);
        //glStencilFunc( GL_EQUAL, 0x1, 0xffffffff );
        //glStencilOp( GL_KEEP, GL_KEEP, GL_ZERO );
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(-0.9, -0.9f);


        /* Render 50% black shadow color on top of whatever the
           floor appareance is. */
//            glDisable( GL_DEPTH_TEST );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glDisable(GL_LIGHTING);  /* Force the 20% black. */
        glColor3f(0,0,0);
        glColor4f(0.0, 0.0, 0.0, 0.4);

        glPushMatrix();
          /* Project the shadow. */
          glMultMatrixf((GLfloat *) shadowMat);
          glPushMatrix();
                scene->drawShadow();
          glPopMatrix();
        glPopMatrix();

        glDisable(GL_BLEND);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_STENCIL_TEST);
        glEnable( GL_DEPTH_TEST );
        glEnable(GL_LIGHTING);


      glPopMatrix();

}


void Draw::Box(Vec4 position, QuaternionQ orientation, Vec4 scale, Vec4 color)
{
    glDisable(GL_LIGHTING);
    Vec4 vertexs[8];

    vertexs[0] = (Vec4(-scale.x()/2,-scale.y()/2,-scale.z()/2));
    vertexs[1] = (Vec4(-scale.x()/2,-scale.y()/2, scale.z()/2));
    vertexs[2] = (Vec4( scale.x()/2,-scale.y()/2, scale.z()/2));
    vertexs[3] = (Vec4( scale.x()/2,-scale.y()/2,-scale.z()/2));
    vertexs[4] = (Vec4(-scale.x()/2, scale.y()/2,-scale.z()/2));
    vertexs[5] = (Vec4(-scale.x()/2, scale.y()/2, scale.z()/2));
    vertexs[6] = (Vec4( scale.x()/2, scale.y()/2, scale.z()/2));
    vertexs[7] = (Vec4( scale.x()/2, scale.y()/2,-scale.z()/2));



    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float angle;
    orientation.toAxisAngle(&vec,&angle);
    glRotatef(angle,vec.x(),vec.y(),vec.z());
    glColor3f(color.x(),color.y(),color.z());
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glBegin(GL_QUADS);
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());

        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());

    glEnd();
    glLineWidth(fmax(scale.x(),fmax(scale.y(),scale.z()))*0.2);
    glColor3f(0,0,0);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glBegin(GL_QUADS);
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());

        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());

    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glLineWidth(1.0);
}


void Draw::Box2Shadow(Vec4 position, QuaternionQ orientation, Vec4 scale)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    Vec4 vertexs[8];

    vertexs[0] = (Vec4(-scale.x()/2,-scale.y()/2,-scale.z()/2));
    vertexs[1] = (Vec4(-scale.x()/2,-scale.y()/2, scale.z()/2));
    vertexs[2] = (Vec4( scale.x()/2,-scale.y()/2, scale.z()/2));
    vertexs[3] = (Vec4( scale.x()/2,-scale.y()/2,-scale.z()/2));
    vertexs[4] = (Vec4(-scale.x()/2, scale.y()/2,-scale.z()/2));
    vertexs[5] = (Vec4(-scale.x()/2, scale.y()/2, scale.z()/2));
    vertexs[6] = (Vec4( scale.x()/2, scale.y()/2, scale.z()/2));
    vertexs[7] = (Vec4( scale.x()/2, scale.y()/2,-scale.z()/2));



    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float angle;
    orientation.toAxisAngle(&vec,&angle);
    glRotatef(angle,vec.x(),vec.y(),vec.z());

    glBegin(GL_QUADS);
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());

        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[2].x(),vertexs[2].y(),vertexs[2].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[6].x(),vertexs[6].y(),vertexs[6].z());

        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[1].x(),vertexs[1].y(),vertexs[1].z());
        glVertex3f(vertexs[5].x(),vertexs[5].y(),vertexs[5].z());

        glVertex3f(vertexs[0].x(),vertexs[0].y(),vertexs[0].z());
        glVertex3f(vertexs[4].x(),vertexs[4].y(),vertexs[4].z());
        glVertex3f(vertexs[7].x(),vertexs[7].y(),vertexs[7].z());
        glVertex3f(vertexs[3].x(),vertexs[3].y(),vertexs[3].z());

    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}


void Draw::Cylinder(Vec4 position, QuaternionQ orientation, Vec4 scale, Vec4 color)
{
    glPushMatrix();
    float height = scale.y();
    float radius = scale.x();
    glDisable(GL_LIGHTING);
    const float PI = 3.141592f;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.15;
    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float anglerot;
    orientation.toAxisAngle(&vec,&anglerot);
    glRotatef(anglerot,vec.x(),vec.y(),vec.z());
    glColor3f(color.x(),color.y(),color.z());
    /** Draw the tube */

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height/2.);
        glVertex3f(x, y , -height/2.);
        angle = angle + angle_stepsize;

    }
    glVertex3f(radius, 0.0, height/2);
    glVertex3f(radius, 0.0, -height/2);
    glEnd();

    /** Draw the circle on top of cylinder */

    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height/2.);
    glEnd();


    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , -height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0, -height/2.);
    glEnd();


    //desenho das linhas
    glLineWidth(fmax(scale.x(),fmax(scale.y(),scale.z()))*0.2);
    glColor3f(0,0,0);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height/2.);
        glVertex3f(x, y , -height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height/2);
    glVertex3f(radius, 0.0, -height/2);
    glEnd();

    /** Draw the circle on top of cylinder */

    glPopMatrix();
    glEnable(GL_LIGHTING);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

}


void Draw::SphereColor(Vec4 position, QuaternionQ orientation, Vec4 color, float radius)
{
    glPushMatrix();
    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float angle;
    orientation.toAxisAngle(&vec,&angle);
    glRotatef(angle,vec.x(),vec.y(),vec.z());
    glDisable(GL_LIGHTING);

    const float PI = 3.141592f;
    GLfloat x, y, z, alpha, beta; // Storage for coordinates and angles
    int gradation = 10;

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glColor3f(color.x(),color.y(),color.z());
    for (alpha = 0.0; alpha < PI; alpha += PI/gradation)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (beta = 0.0; beta < 2.01*PI; beta += PI/gradation)
        {
            x = radius*cos(beta)*sin(alpha);
            y = radius*sin(beta)*sin(alpha);
            z = radius*cos(alpha);
            glVertex3f(x, y, z);
            x = radius*cos(beta)*sin(alpha + PI/gradation);
            y = radius*sin(beta)*sin(alpha + PI/gradation);
            z = radius*cos(alpha + PI/gradation);
            glVertex3f(x, y, z);
        }
        glEnd();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}


void Draw::Cylinder2Shadow(Vec4 position, QuaternionQ orientation, Vec4 scale)
{
    glPushMatrix();
    float height = scale.y();
    float radius = scale.x();
    glDisable(GL_LIGHTING);
    const float PI = 3.141592f;
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.15;
    glTranslatef(position.x(),position.y(),position.z());
    Vec4 vec;
    float anglerot;
    orientation.toAxisAngle(&vec,&anglerot);
    glRotatef(anglerot,vec.x(),vec.y(),vec.z());

    /** Draw the tube */

    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height/2.);
        glVertex3f(x, y , -height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height/2);
    glVertex3f(radius, 0.0, -height/2);
    glEnd();

    /** Draw the circle on top of cylinder */

    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0.0, height/2.);
    glEnd();


    glBegin(GL_POLYGON);
    angle = 0.0;
    while( angle < 2*PI ) {
        x = radius * cos(angle);
        y = radius * sin(angle);
        glVertex3f(x, y , -height/2.);
        angle = angle + angle_stepsize;
    }
    glVertex3f(radius, 0, -height/2.);
    glEnd();


    glPopMatrix();
    glEnable(GL_LIGHTING);

}


void Draw::TrajetoryBall(std::vector<Vec4> positions,Vec4 color)
{
    if (!(positions.size()>0)) return;
    //qDebug() << positions.size();
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLineWidth(2.0);
    glColor3f(color.x(),color.y(),color.z());
    //glEnable(GL_LINE_STIPPLE);
    //glLineStipple( 10.0, 0xF0F0);
    bool show = true;
    glBegin(GL_LINES);
        for(int i=0;i<positions.size()-15;i+=15){
            if(show){
               glVertex3f(positions[i].x(),positions[i].y(),positions[i].z());
               glVertex3f(positions[i+15].x(),positions[i+15].y(),positions[i+15].z());
               show = false;
            }else{
               show = true;
            }
        }
    glEnd();
    glPopMatrix();
    glDisable(GL_LINE_STIPPLE);
    //glEnable(GL_LIGHTING);
}

void Draw::Text(QString text, int x1_o2d,int x2_o2d, int x3_o2d,int x4_o2d,float posWin, float posHei,Vec4 color,int size)
{
    glDisable(GL_LIGHTING);
    glColor3f(color.x(),color.y(),color.z());
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(x1_o2d, x2_o2d, x3_o2d,x4_o2d,30,300);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    int k = 0;

    glRasterPos2f(posWin, posHei);
    k = 0;
    while (k<text.size()){
        if(size==0)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,text.at(k).toLatin1());
        else
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text.at(k).toLatin1());
        k++;
    }

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}
