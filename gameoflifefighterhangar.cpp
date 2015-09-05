#include "hangar.h"

Hangar::Hangar(
  const int left,
  const int top,
  const int width,
  const int height,
  const Player player
) :
    m_left{left},
    m_top{top},
    m_width{width},
    m_height{height},
    m_player{player}
{

}

bool Hangar::IsIn(const int x, const int y) const noexcept
{
  return
       x >= m_left
    && x  < m_left + m_width
    && y >= m_top
    && y  < m_top + m_height
  ;
}


