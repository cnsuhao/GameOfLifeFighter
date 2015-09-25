#include "sfmlgameoflifefightersprites.h"

#include <cassert>
#include "sfmlgameoflifefighterspritesimpl.h"
#include "gameoflifefighterhelper.h"

golf::SfmlSpritesImpl * golf::SfmlSprites::sm_impl{nullptr};


golf::SfmlSprites::SfmlSprites()
{
  #ifndef NDEBUG
  Test();
  #endif

  if (!sm_impl) { sm_impl = new SfmlSpritesImpl; }
  assert(sm_impl);
}

sf::Texture& golf::SfmlSprites::Get(const CellType cell_type) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(cell_type);
}

sf::Texture& golf::SfmlSprites::Get(const HangarOf hangar_of) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(hangar_of);
}

sf::Texture& golf::SfmlSprites::Get(const HeartOf heart_of) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(heart_of);
}

sf::Texture& golf::SfmlSprites::Get(const IsBuilding is_building) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(is_building);
}

sf::Texture& golf::SfmlSprites::Get(const PlayerIndex player_index) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(player_index);
}

//sf::Texture& golf::SfmlSprites::Get(const SelectedBy selected_by) noexcept
//{
//  assert(sm_impl);
//  return sm_impl->Get(selected_by);
//}


int golf::SfmlSprites::GetHeight() const noexcept
{
  assert(sm_impl);
  return sm_impl->GetHeight();
}

int golf::SfmlSprites::GetWidth() const noexcept
{
  assert(sm_impl);
  return sm_impl->GetWidth();
}

#ifndef NDEBUG
void golf::SfmlSprites::Test() noexcept
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
    SfmlSprites s;
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
  }
}
#endif
