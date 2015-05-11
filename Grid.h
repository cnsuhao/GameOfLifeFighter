#ifndef GRID_H
#define GRID_H

#include <vector>

struct Grid
{
  Grid(const int width, const int height);
  void Set(const int x, const int y, const int i);
  int Get(const int x, const int y) const;
  void Next();

  private:
  //Y-X ordered
  std::vector<std::vector<int>> m_grid;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GRID_H
