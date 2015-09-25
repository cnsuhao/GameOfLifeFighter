#include <cassert>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#pragma GCC diagnostic pop

#include "qtgameoflifefighterwidget.h"
#include "gameoflifefightertrace.h"

#include <SFML/Audio/Music.hpp>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  START_TRACE();

  sf::Music music;
  const bool can_open{music.openFromFile("../GameOfLifeFighter/Resources/Music/GameOfDeath.ogg")};
  assert(can_open);
  music.play();

  golf::QtGameOfLifeFighterWidget w;
  w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  //w.setWindowState(Qt::WindowFullScreen);
  w.show();
  a.exec();
}
