#ifndef SFMLGAMEOFLIFEFIGHTERSPRITES_H
#define SFMLGAMEOFLIFEFIGHTERSPRITES_H

#include <SFML/Graphics/Sprite.hpp>

#include "gameoflifefightercellstate.h"
#include "gameoflifefighterplayerindex.h"

namespace golf {

struct SfmlSpritesImpl;

///A Singleton
struct SfmlSprites
{
  SfmlSprites();

  ///The cell its state
  sf::Texture& Get(const CellType cell_type) noexcept;

  ///Border to indicate a player can build here
  sf::Texture& Get(const HangarOf hangar_of) noexcept;

  ///Border to indicate this is a heart
  sf::Texture& Get(const HeartOf heart_of) noexcept;

  ///Is a player building here (that is not activated yet)?
  sf::Texture& Get(const IsBuilding is_building) noexcept;

  ///Cursor of player
  sf::Texture& Get(const PlayerIndex player_index) noexcept;

  ///Is the cursor of a player here?
  //sf::Texture& Get(const SelectedBy selected_by) noexcept;

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
