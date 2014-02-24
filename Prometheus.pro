#-------------------------------------------------
#
# Project created by QtCreator 2012-09-07T15:08:52
#
#-------------------------------------------------

QT       += core gui opengl widgets

TARGET = Prometheus
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cpsviewer.cpp \
    pandora.cpp \
    atlas.cpp \
    atomtooldialog.cpp \
    hephaestus.cpp \
    apollo.cpp \
    prometheusstatus.cpp

HEADERS  += mainwindow.h \
    cpsviewer.h \
    pandora.h \
    atlas.h \
    atomtooldialog.h \
    hephaestus.h \
    apollo.h \
    prometheusstatus.h

FORMS    += mainwindow.ui \
    atomtooldialog.ui \
    prometheusstatus.ui

ICON = Prometheus_Logo.icns

RESOURCES += \
    PrometheusResources.qrc

OTHER_FILES += \
    vertexShader.vert \
    fragmentShader.frag \
    geometryShader.geom
