LIBS += Urho3D/lib/libUrho3D.a \
    -lpthread \
    -lSDL \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

INCLUDEPATH += \
    Urho3D/include/ \
    Urho3D/include/Urho3D/ThirdParty/ \

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    mastercontrol.cpp \
    inputmaster.cpp \
    golfcam.cpp \
    cell.cpp \
    cellmaster.cpp \
    cellring.cpp

HEADERS += \
    mastercontrol.h \
    inputmaster.h \
    golfcam.h \
    cell.h \
    cellmaster.h \
    cellring.h
