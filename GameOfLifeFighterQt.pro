QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TEMPLATE = app

# C++11 for Travis
CONFIG += c++11
QMAKE_CXX = g++-4.8
QMAKE_LINK = g++-4.8
QMAKE_CC = gcc-4.8

# Urho3D and Qt do not go well with -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++11

CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

include(GameOfLifeFighterConsole.pri)
include(GameOfLifeFighterQt.pri)

SOURCES += qtmain.cpp

