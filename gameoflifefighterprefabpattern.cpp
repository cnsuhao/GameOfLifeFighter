#include "gameoflifefighterprefabpattern.h"

golf::PrefabPattern::PrefabPattern(const int width, const int height)
  :
    m_cursor_x{width / 2},
    m_cursor_y{height / 2},
    m_grid(width,height)

{

}

golf::PrefabPattern golf::CreateGlider()
{
  PrefabPattern pattern(3,3);
  pattern.SetCell(1,0,CellType::alive);
  pattern.SetCell(2,1,CellType::alive);
  pattern.SetCell(2,2,CellType::alive);
  pattern.SetCell(1,2,CellType::alive);
  pattern.SetCell(0,2,CellType::alive);
  pattern.SetCursor(1,1);
  return pattern;
}

golf::PrefabPattern golf::CreateLightWeightSpaceShip()
{
  return CreateGlider();
}

golf::PrefabPattern golf::CreateGrower()
{
  return CreateGlider();
}
