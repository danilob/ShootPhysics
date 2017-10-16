#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T13:32:38
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShootPhysics
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphics/glwidget.cpp \
    scene/object.cpp \
    scene/scene.cpp \
    physics/physics.cpp \
    scene/draw.cpp

HEADERS  += mainwindow.h \
    graphics/glwidget.h \
    scene/object.h \
    scene/scene.h \
    physics/physics.h \
    scene/draw.h

FORMS    += mainwindow.ui
