TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    PE.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    PE.h

