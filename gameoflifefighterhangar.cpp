#include "gameoflifefighterhangar.h"

#include <cassert>
#include <stdexcept>

#include "gameoflifefightergrid.h"
#include "gameoflifefighterplayerindex.h"
#include "gameoflifefighterplayer.h"

golf::Hangar::Hangar(
  const int left,
  const int top,
  const int width,
  const int height,
  const PlayerIndex player_index
) :
    m_grid(width,height),
    m_height{height},
    m_left{left},
    m_player_index{player_index},
    m_state{HangarState::open},
    m_top{top},
    m_width{width}
{

}

void golf::Hangar::BuildPattern(
  const int cursor_x,
  const int cursor_y,
  const PrefabPattern& pattern
)
{
  const int w = pattern.GetWidth();
  const int h = pattern.GetHeight();
  for (int y=0; y!=h; ++y)
  {
    for (int x=0; x!=w; ++x)
    {
      SetCell(
        x + cursor_x - pattern.GetCursorX(),
        y + cursor_y - pattern.GetCursorY(),
        pattern.GetCell(x,y)
      );
    }
  }
}

void golf::Hangar::Close(
  Grid& grid
) noexcept
{
  ///If the Hangar is closed, the content of the Grid will be the initial content of the Hangar and put into statis
  if (m_state == HangarState::closed) return;

  //If the Hangar is opened, the content of the Hangar will be transferred to the Grid
  const int left{GetLeft()};
  const int width{GetWidth()};
  const int top{GetTop()};
  const int height{GetHeight()};
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      //Transfer
      m_grid.Set(x,y,grid.Get(left + x,top + y));
      //Empty global grid
      grid.Set(left + x,top + y,CellType::empty);
    }
  }
  m_state = HangarState::closed;

}

bool golf::Hangar::IsIn(const int x, const int y) const noexcept
{
  return
       x >= m_left
    && x  < m_left + m_width
    && y >= m_top
    && y  < m_top + m_height
  ;
}

bool golf::Hangar::IsIn(const Player& player) const noexcept
{
  return IsIn(player.GetX(), player.GetY());
}

void golf::Hangar::Open(
  Grid& grid
) noexcept
{
  if (m_state == HangarState::open) return;

  //If the Hangar is opened, the content of the Hangar will be transferred to the Grid
  const int left{GetLeft()};
  const int width{GetWidth()};
  const int top{GetTop()};
  const int height{GetHeight()};
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      //Transfer
      grid.Set(left + x,top + y,m_grid.Get(x,y));
      //Empty Hangar
      m_grid.Set(x,y,CellType::empty);
    }
  }
  m_state = HangarState::open;
}

void golf::Hangar::SetState(
  const HangarState state,
  Grid& grid
) noexcept
{
  switch (state)
  {
    case HangarState::closed: Close(grid); return;
    case HangarState::open: Open(grid); return;
    default: assert(!"Should not get here");
  }
}

void golf::Hangar::ToggleCell(const int x, const int y)
{
  m_grid.Toggle(x,y);
}

void golf::Hangar::ToggleStasis(Grid& global_grid) noexcept
{
  switch (m_state)
  {
    case HangarState::open: Close(global_grid); break;
    case HangarState::closed: Open(global_grid); break;
    default:
      assert(!"golf::Hangar::Toggle: unimplemented HangarState");
      throw std::logic_error("golf::Hangar::Toggle: unimplemented HangarState");
  }
}
