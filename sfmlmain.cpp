#include "sfmlgameoflifefighterwidget.h"
#include "gameoflifefightertrace.h"
#include "SFML/Audio/Music.hpp"
int main()
{
  START_TRACE();

  sf::Music music;
  const bool can_open{music.openFromFile("../GameOfLifeFighter/Resources/Music/GameOfDeath.ogg")};
  assert(can_open);
  music.play();

  golf::SfmlWidget w;
  w.Execute();
}

