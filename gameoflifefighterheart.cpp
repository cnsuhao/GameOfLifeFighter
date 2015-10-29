#include "gameoflifefighterheart.h"

#include <cassert>

#include "gameoflifefightergrid.h"


golf::Heart::Heart(
  const int left,
  const int top,
  const int width,
  const int height,
  const PlayerIndex player_index
) : m_left{left},
    m_top{top},
    m_width{width},
    m_height{height},
    m_player_index{player_index}
{

}

bool golf::Heart::IsIn(const int x, const int y) const noexcept
{
  return
       x >= m_left
    && x  < m_left + m_width
    && y >= m_top
    && y  < m_top + m_height
  ;
}

bool golf::Heart::IsEmpty(const Grid& grid) const
{
  const int right{m_left + m_width};
  const int bottom{m_top + m_height};
  for (int y=m_top; y!=bottom; ++y)
  {
    for (int x=m_left; x!=right; ++x)
    {
      if (grid.Get(x,y) == CellType::alive) return false;
    }
  }
  return true;
}
