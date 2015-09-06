LIBS += ../GameOfLifeFighter/Urho3D/lib/libUrho3D.a \
    -lpthread \
    -ldl \
    -lGL

DEFINES += URHO3D_COMPILE_QT

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
    ../GameOfLifeFighter/Urho3D/include \
    ../GameOfLifeFighter/Urho3D/include/Urho3D/ThirdParty

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    gameoflifefightergame.cpp \
    mastercontrol.cpp \
    inputmaster.cpp \
    golfcam.cpp \
    cell.cpp \
    cellmaster.cpp \
    cellring.cpp \
    helper.cpp

HEADERS += \
    gameoflifefightergame.h \
    mastercontrol.h \
    inputmaster.h \
    golfcam.h \
    cell.h \
    cellmaster.h \
    cellring.h
    helper.h
