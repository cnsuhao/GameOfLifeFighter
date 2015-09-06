#include "gameoflifefighterhangar.h"
#include "gameoflifefighterplayerindex.h"

golf::Hangar::Hangar(
  const int left,
  const int top,
  const int width,
  const int height,
  const PlayerIndex player_index
) :
    m_left{left},
    m_top{top},
    m_width{width},
    m_height{height},
    m_player_index{player_index}
{

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


