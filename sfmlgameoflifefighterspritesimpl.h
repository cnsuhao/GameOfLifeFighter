#ifndef SFMLGAMEOFLIFEFIGHTERSPRITE_H
#define SFMLGAMEOFLIFEFIGHTERSPRITE_H

#include <map>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "gameoflifefightercelltype.h"
#include "gameoflifefightercellstate.h"
#include "sfmlgameoflifefighterresourceholder.h"

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

  sf::Texture& Get(const CellType& cell_type) noexcept;
  sf::Texture& Get(const HangarOf& hangar_of) noexcept;
  sf::Texture& Get(const HeartOf& heart_of) noexcept;
  sf::Texture& Get(const SelectedBy& selected_by) noexcept;

  int GetHeight() const noexcept { return 6; }
  int GetWidth() const noexcept { return 6; }

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
  ResourceHolder<sf::Texture,CellType> m_cell_type_textures;
  ResourceHolder<sf::Texture,HangarOf> m_hangar_of_textures;
  ResourceHolder<sf::Texture,HeartOf> m_heart_of_textures;
  ResourceHolder<sf::Texture,SelectedBy> m_selected_by_textures;
  friend class SfmlSprites;
};

} //~namespace golf

#endif // SFMLGAMEOFLIFEFIGHTERSPRITE_H
