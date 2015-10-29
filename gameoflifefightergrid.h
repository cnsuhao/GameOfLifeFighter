#ifndef GAMEOFLIFEFIGHTERGRID_H
#define GAMEOFLIFEFIGHTERGRID_H

#include <vector>
#include "gameoflifefightercelltype.h"

namespace golf {

class Grid
{
  public:
  Grid(const int width, const int height);
  int CountNeighbours(const int x, const int y) const noexcept;
  void FlipHorizontal() noexcept;
  void FlipVertical() noexcept;
  CellType Get(const int x, const int y) const;
  int GetHeight() const noexcept;
  int GetWidth() const;
  void Next();
  void Set(const int x, const int y, const CellType cell);
  void Toggle(const int x, const int y);

  const std::vector<std::vector<CellType>>& GetRawGrid() const noexcept { return m_grid; }

  private:
  //Y-X ordered
  std::vector<std::vector<CellType>> m_grid;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGRID_H
