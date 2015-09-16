#ifndef SFMLGAMEOFLIFEFIGHTERSPRITE_H
#define SFMLGAMEOFLIFEFIGHTERSPRITE_H

#include <map>

#include <SFML/Graphics/Sprite.hpp>

#include "gameoflifefightercelltype.h"
#include "gameoflifefightercellstate.h"

namespace golf {

struct SfmlSprite
{
  SfmlSprite();

  static sf::Sprite& Get(const CellState& state) noexcept;

  static sf::Sprite& Get(
    const int selected_by,   //0: no-one, 1: player1, 2: player2
    const int hangar_of,     //0: no-one, 1: player1, 2: player2
    const int heart_of,      //0: no-one, 1: player1, 2: player2
    const bool is_building,  //Has the player built something on this square?
    const CellType cell_type 
  );
  int GetHeight() const noexcept { return 6; }
  int GetWidth() const noexcept { return 6; }

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  static std::map<CellState,sf::Sprite> m_sprites;

  static std::map<CellState,sf::Sprite> CreateSprites() noexcept;
};

} //~namespace golf

#endif // SFMLGAMEOFLIFEFIGHTERSPRITE_H
