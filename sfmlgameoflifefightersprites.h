#ifndef SFMLGAMEOFLIFEFIGHTERSPRITES_H
#define SFMLGAMEOFLIFEFIGHTERSPRITES_H

#include <SFML/Graphics/Sprite.hpp>

#include "gameoflifefightercellstate.h"

namespace golf {

struct SfmlSpritesImpl;

///A Singleton
struct SfmlSprites
{
  SfmlSprites();

  sf::Sprite& Get(const CellState& state) noexcept;

  int GetHeight() const noexcept;
  int GetWidth() const noexcept;

  private:

  static SfmlSpritesImpl * sm_impl;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // SFMLGAMEOFLIFEFIGHTERSPRITES_H
