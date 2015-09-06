#ifndef GAMEOFLIFEFIGHTERPREFABPATTERN_H
#define GAMEOFLIFEFIGHTERPREFABPATTERN_H

#include "gameoflifefightergrid.h"

namespace golf {

///Class to store pre-fab patterns.
///A pre-fab pattern consists out of a grid and a cursor position.
///Examples:
/// 0 1 0
/// 0 0 1 with cursor = (1,1)
/// 1 1 1
///This is a glider that is built around the cursor
/// 0 1 0
/// 0 0 1 with cursor = (2,2)
/// 1 1 1
///This is a glider that will have its tip at the cursor
struct PrefabPattern
{
  PrefabPattern(const int width, const int height);

  int GetCursorX() const noexcept { return m_cursor_x; }
  int GetCursorY() const noexcept { return m_cursor_y; }
  int GetHeight() const noexcept { return m_grid.GetHeight(); }
  int GetWidth() const noexcept { return m_grid.GetWidth(); }
  void SetCursor(const int x, const int y) { m_cursor_x = x; m_cursor_y = y; }
  void SetCell(const int x, const int y, const CellType cell) { m_grid.Set(x,y,cell); }
  private:
  int m_cursor_x;
  int m_cursor_y;
  Grid m_grid;
};

PrefabPattern CreateGlider();
PrefabPattern CreateLightWeightSpaceShip();
PrefabPattern CreateGrower();

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERPREFABPATTERN_H
