#ifndef SFMLGAMEOFLIFEFIGHTERSPRITE_H
#define SFMLGAMEOFLIFEFIGHTERSPRITE_H

#include <map>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "gameoflifefightercelltype.h"
#include "gameoflifefightercellstate.h"

namespace golf {

///Implementation of SfmlSprites.
///SfmlSprites is the Singleton that holds one instance of SfmlSpritesImpl,
///because SfmlSpritesImpl is expensive to create
struct SfmlSpritesImpl
{
  private:
  SfmlSpritesImpl();
  SfmlSpritesImpl(const SfmlSpritesImpl&) = delete;
  SfmlSpritesImpl& operator=(const SfmlSpritesImpl&) = delete;

  sf::Sprite& Get(const CellState& state) noexcept;

  sf::Sprite& Get(
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

  std::map<CellState,sf::Sprite> m_sprites;
  std::map<CellState,sf::Sprite> m_textures;
  sf::Texture m_texture_alive;
  sf::Texture m_texture_empty;
  sf::Texture m_texture_hangar_blue;
  sf::Texture m_texture_hangar_red;

  friend class SfmlSprites;
};

} //~namespace golf

#endif // SFMLGAMEOFLIFEFIGHTERSPRITE_H
