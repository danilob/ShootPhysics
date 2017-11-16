#include "glwidget.h"

#include <sys/time.h>
#include <stdio.h>

#include "GL/glu.h"
#include "GL/glut.h"
#include "scene/camera.h"


//variaveis para controle da câmera
static bool lbpressed = false;
static bool rbpressed = false;
static float last_x = 0.0;
static float last_y = 0.0;

int frame = 0;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers),parent)
{
    setFocusPolicy(Qt::StrongFocus);
    scene = new Scene();
    scene->defaultScene();
    simTimer = new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(simStep()));
    simTimer->start(0);
}


void GLWidget::initializeGL()
{
    glClearColor(1,1,1,1);

    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations



}

void GLWidget::resizeGL(int w, int h)
{
    const float ar = w>0 ? (float) w / (float) h : 1.0;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.,ar,0.001,6000.);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    scene->setWindow(w,h);
    scene->setProjection(Vec4(30.,ar,0.001,6000));
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    scene->draw();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();


    if (lbpressed && !rbpressed) {
        scene->getCamera()->rotatex(y,last_y);
        scene->getCamera()->rotatey(x,last_x);
    }
    if (!lbpressed && rbpressed) {
        scene->getCamera()->translatex(x,last_x);
        scene->getCamera()->translatey(y,last_y);
    }
    if (lbpressed && rbpressed) {
        scene->getCamera()->zoom(y,last_y);
    }


    last_x = x;
    last_y = y;

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();
    if (event->button() & Qt::LeftButton) {
        // when the button is pressed
        lbpressed = false;
    }
    // if the left button is pressed
    if (event->button() & Qt::RightButton) {
        // when the button is pressed
        rbpressed = false;
    }
    last_x = x;
    last_y = y;

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{

    int y = event->pos().y();
    int x = event->pos().x();
    // if the left button is pressed
    if (event->button() & Qt::LeftButton) {
        // when the button is pressed
        lbpressed = true;
    }
    // if the left button is pressed
    if (event->button() & Qt::RightButton) {
        // when the button is pressed
        rbpressed = true;
    }

    last_x = x;
    last_y = y;


}

void GLWidget::keyPressEvent(QKeyEvent *event){
     if(event->key()==Qt::Key_F){
         scene->addTorque(Vec4(1250,1050,0),0);
     }

     if(event->key()==Qt::Key_Right){
         scene->updateOriCannon(Vec4(0,-1,0));
     }

     if(event->key()==Qt::Key_Left){
         scene->updateOriCannon(Vec4(0,1,0));
     }

     if(event->key()==Qt::Key_Up){
         scene->updateOriCannon(Vec4(-1,0,0));
     }

     if(event->key()==Qt::Key_Down){
         scene->updateOriCannon(Vec4(1,0,0));
     }

     if(event->key()==Qt::Key_Space){
         scene->shotBall();
     }

     if(event->key()==Qt::Key_A){
         scene->increaseVelocity();
     }

     if(event->key()==Qt::Key_D){
         scene->decreaseVelocity();
     }

     if(event->key()==Qt::Key_P){
        singleShotCapture();
     }

     if(event->key()==Qt::Key_V){
        scene->enableSecondViewport(!scene->isEnabledSecondViewport());
     }
}

void GLWidget::simStep(){

    double ti,tf,tempo; // ti = tempo inicial // tf = tempo final
    ti = tf = tempo = 0;
    timeval tempo_inicio,tempo_fim;
    gettimeofday(&tempo_inicio,NULL);

    scene->simulationStep();
    update();


    gettimeofday(&tempo_fim,NULL);
    tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
    ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
    tempo = (tf - ti) / 1000;
    //qDebug() << "TEMPO: "<< tempo;
    //printf("Tempo gasto em milissegundos para desenhar %.3f\n",tempo);

    double tempoFalta = (33.33 - tempo); //considerando 30frames/sec
    if (tempoFalta>0)
       simTimer->setInterval(tempoFalta);


}

void GLWidget::singleShotCapture()
{

    QString temp_str = QString().sprintf("../screenshots/%d.png",frame);
    //QPixmap::grabWindow(this->winId(),0,0,scene->getWidth(),scene->getHeight() ).save( temp_str  );
    //QScreen::grabWindow(this->winId(),0,0,scene->getWidth(),scene->getHeight()).save( temp_str  );
    QScreen * screen = QGuiApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(this->winId());
    pixmap.save(temp_str, "PNG", 0);
    frame++;

}
