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
  pattern.SetCell(0,0,CellType::empty);
  pattern.SetCell(1,0,CellType::alive);
  pattern.SetCell(2,0,CellType::empty);

  pattern.SetCell(0,1,CellType::empty);
  pattern.SetCell(1,1,CellType::empty);
  pattern.SetCell(2,1,CellType::alive);

  pattern.SetCell(0,2,CellType::alive);
  pattern.SetCell(1,2,CellType::alive);
  pattern.SetCell(2,2,CellType::alive);

  pattern.SetCursor(1,1);
  return pattern;
}

golf::PrefabPattern golf::CreateLightWeightSpaceShip()
{
  PrefabPattern pattern(5,4);
  pattern.SetCell(0,0,CellType::empty);
  pattern.SetCell(1,0,CellType::alive);
  pattern.SetCell(2,0,CellType::alive);
  pattern.SetCell(3,0,CellType::empty);
  pattern.SetCell(4,0,CellType::empty);

  pattern.SetCell(0,1,CellType::alive);
  pattern.SetCell(1,1,CellType::alive);
  pattern.SetCell(2,1,CellType::alive);
  pattern.SetCell(3,1,CellType::alive);
  pattern.SetCell(4,1,CellType::empty);

  pattern.SetCell(0,2,CellType::alive);
  pattern.SetCell(1,2,CellType::alive);
  pattern.SetCell(2,2,CellType::empty);
  pattern.SetCell(3,2,CellType::alive);
  pattern.SetCell(4,2,CellType::alive);

  pattern.SetCell(0,3,CellType::empty);
  pattern.SetCell(1,3,CellType::empty);
  pattern.SetCell(2,3,CellType::alive);
  pattern.SetCell(3,3,CellType::alive);
  pattern.SetCell(4,3,CellType::empty);
  pattern.SetCursor(2,2);
  return pattern;
}

golf::PrefabPattern golf::CreateGrower()
{
  return CreateGlider();
}
