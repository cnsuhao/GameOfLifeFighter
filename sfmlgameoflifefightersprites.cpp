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

sf::Sprite& golf::SfmlSprites::Get(const CellState& state) noexcept
{
  assert(sm_impl);
  return sm_impl->Get(state);
}

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
    for (const auto cell_state: GetAllCellStates())
    {
      assert(s.Get(cell_state).getTexture());
    }

  }
}
#endif
