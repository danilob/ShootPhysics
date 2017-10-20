#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>

#include "scene/scene.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    Scene* scene;
    QTimer *simTimer;
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);




signals:

public slots:
    //funções de controle do tempo de atualização da janela
    void simStep();
    //captura um frame da simulação
    void singleShotCapture();
};

#endif // GLWIDGET_H
