#include "sfmlgameoflifefighterspritesimpl.h"

#include <cassert>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "sfmlgameoflifefighterplayerindex.h"
#include "sfmlgameoflifefighterhelper.h"
#include "gameoflifefighterhelper.h"
#include "gameoflifefightercellstate.h"

golf::SfmlSpritesImpl::SfmlSpritesImpl()
  :
    m_cell_type_textures{},
    m_hangar_of_textures{},
    m_heart_of_textures{},
    m_is_building_textures{},
    m_player_index_textures{}
    //m_selected_by_textures{}
{
  #ifndef NDEBUG
  Test();
  #endif
  m_cell_type_textures.Load(CellType::empty,"../GameOfLifeFighter/Resources/Sprites/Empty.png");
  m_cell_type_textures.Load(CellType::alive,"../GameOfLifeFighter/Resources/Sprites/Alive.png");

  m_hangar_of_textures.Load(HangarOf::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_hangar_of_textures.Load(HangarOf::player1,"../GameOfLifeFighter/Resources/Sprites/HangarBlue.png");
  m_hangar_of_textures.Load(HangarOf::player2,"../GameOfLifeFighter/Resources/Sprites/HangarRed.png");

  m_heart_of_textures.Load(HeartOf::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_heart_of_textures.Load(HeartOf::player1,"../GameOfLifeFighter/Resources/Sprites/HeartBlue.png");
  m_heart_of_textures.Load(HeartOf::player2,"../GameOfLifeFighter/Resources/Sprites/HeartRed.png");

  m_is_building_textures.Load(IsBuilding::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_is_building_textures.Load(IsBuilding::player1,"../GameOfLifeFighter/Resources/Sprites/HeartBlue.png");
  m_is_building_textures.Load(IsBuilding::player2,"../GameOfLifeFighter/Resources/Sprites/HeartRed.png");

  m_player_index_textures.Load(PlayerIndex::player1,"../GameOfLifeFighter/Resources/Sprites/CursorBlue.png");
  m_player_index_textures.Load(PlayerIndex::player2,"../GameOfLifeFighter/Resources/Sprites/CursorRed.png");

  //m_selected_by_textures.Load(SelectedBy::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  //m_selected_by_textures.Load(SelectedBy::player1,"../GameOfLifeFighter/Resources/Sprites/SelectedByBlue.png");
  //m_selected_by_textures.Load(SelectedBy::player2,"../GameOfLifeFighter/Resources/Sprites/SelectedByRed.png");
}

sf::Texture& golf::SfmlSpritesImpl::Get(const CellType& cell_type) noexcept
{
  return m_cell_type_textures.Get(cell_type);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const HangarOf& hangar_of) noexcept
{
  return m_hangar_of_textures.Get(hangar_of);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const HeartOf& heart_of) noexcept
{
  return m_heart_of_textures.Get(heart_of);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const IsBuilding& is_building) noexcept
{
  return m_is_building_textures.Get(is_building);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const PlayerIndex& player_index) noexcept
{
  return m_player_index_textures.Get(player_index);
}

//sf::Texture& golf::SfmlSpritesImpl::Get(const SelectedBy& selected_by) noexcept
//{
//  return m_selected_by_textures.Get(selected_by);
//}

#ifndef NDEBUG
void golf::SfmlSpritesImpl::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Helper();
  }
  //Create one pixmap with all pictures
  {
    SfmlSpritesImpl s;
    for (const auto cell_type: GetAllCellTypes())
    {
      assert(s.Get(cell_type).getSize().x == 6);
      assert(s.Get(cell_type).getSize().y == 6);
    }
    for (const auto hangar_of: GetAllHangarOfs())
    {
      assert(s.Get(hangar_of).getSize().x == 6);
      assert(s.Get(hangar_of).getSize().y == 6);
    }
    for (const auto heart_of: GetAllHeartOfs())
    {
      assert(s.Get(heart_of).getSize().x == 6);
      assert(s.Get(heart_of).getSize().y == 6);
    }
    //for (const auto selected_by: GetAllSelectedBys())
    //{
    //  assert(s.Get(selected_by).getSize().x == 6);
    //  assert(s.Get(selected_by).getSize().y == 6);
    //}
    for (const auto player_index: GetAllPlayerIndices())
    {
      assert(s.Get(player_index).getSize().x == 6);
      assert(s.Get(player_index).getSize().y == 6);
    }
    for (const auto is_building: GetAllIsBuildings())
    {
      assert(s.Get(is_building).getSize().x == 6);
      assert(s.Get(is_building).getSize().y == 6);
    }
  }
}
#endif
