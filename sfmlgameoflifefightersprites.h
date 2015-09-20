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

  sf::Texture& Get(const CellType& cell_type) noexcept;
  sf::Texture& Get(const HangarOf& hangar_of) noexcept;
  sf::Texture& Get(const HeartOf& heart_of) noexcept;
  sf::Texture& Get(const SelectedBy& selected_by) noexcept;

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
