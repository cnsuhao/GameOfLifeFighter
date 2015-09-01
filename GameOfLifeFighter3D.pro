LIBS += ../GameOfLifeFighter/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

INCLUDEPATH += \
    ../GameOfLifeFighter/Urho3D/include/ \
    ../GameOfLifeFighter/Urho3D/include/Urho3D/ThirdParty/ \

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
    cellring.cpp \
    helper.cpp

HEADERS += \
    mastercontrol.h \
    inputmaster.h \
    golfcam.h \
    cell.h \
    cellmaster.h \
    cellring.h
    helper.h
