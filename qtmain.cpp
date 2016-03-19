#include <cassert>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#include <iostream>
#pragma GCC diagnostic pop
#include "qtgameoflifefighterwidget.h"
#include "gameoflifefightertrace.h"

#include <SFML/Audio/Music.hpp>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  START_TRACE();

  sf::Music music;
  const std::string filename{"../GameOfLifeFighter/Resources/Music/GameOfDeath.ogg"};
  const bool can_open{music.openFromFile(filename)};
  if (can_open)
  {
    music.play();
  }
  else
  {
    std::cerr << "Warning: cannot open file '" << filename << "'\n";
  }
  golf::QtGameOfLifeFighterWidget w;
  w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
  //w.setWindowState(Qt::WindowFullScreen);
  w.show();
  a.exec();
}
