#include "gameoflifefighterhangar.h"

#include <cassert>

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
    m_height{height},
    m_left{left},
    m_player_index{player_index},
    m_state{HangarState::open},
    m_top{top},
    m_width{width}
{

}

void golf::Hangar::Close(
  Grid& grid
) noexcept
{
  assert(grid.GetWidth() > 0);
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
  assert(grid.GetWidth() > 0);

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
