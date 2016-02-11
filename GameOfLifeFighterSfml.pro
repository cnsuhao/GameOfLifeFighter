QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++
QT       -= core gui

TEMPLATE = app

CONFIG(debug, debug|release) {
  message(Debug mode)
}

CONFIG(release, debug|release) {
  message(Release mode)
  DEFINES += NDEBUG NTRACE_BILDERBIKKEL
}

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

include(GameOfLifeFighterConsole.pri)
include(GameOfLifeFighterSfml.pri)

SOURCES += sfmlmain.cpp

