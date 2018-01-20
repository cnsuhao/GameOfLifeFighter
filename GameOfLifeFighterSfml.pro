QT       -= core gui
TEMPLATE = app

# C++11 for Travis
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

# Urho3D and Qt do not go well with -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++11

CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG
}

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

include(GameOfLifeFighterConsole.pri)
include(GameOfLifeFighterSfml.pri)

SOURCES += sfmlmain.cpp

