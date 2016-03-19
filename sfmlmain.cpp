#include <iostream>

#include "sfmlgameoflifefighterwidget.h"
#include "gameoflifefightertrace.h"
#include "SFML/Audio/Music.hpp"

int main()
{
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
  golf::SfmlWidget w;
  w.Execute();
}
