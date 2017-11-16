#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T13:32:38
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShootPhysics
TEMPLATE = app

DEFINES += \
           dDOUBLE \


SOURCES += main.cpp\
        mainwindow.cpp \
    graphics/glwidget.cpp \
    scene/object.cpp \
    scene/scene.cpp \
    physics/physics.cpp \
    scene/draw.cpp \
    math/quaternion.cpp \
    math/vec4.cpp \
    scene/camera.cpp

HEADERS  += mainwindow.h \
    graphics/glwidget.h \
    scene/object.h \
    scene/scene.h \
    physics/physics.h \
    scene/draw.h \
    math/quaternion.h \
    math/vec4.h \
    scene/camera.h

FORMS    += mainwindow.ui

LIBS += -lode -lGLU -lglut


