#ifndef GRID_H
#define GRID_H

#include <vector>

struct Grid
{
  Grid(const int width, const int height);
  int Get(const int x, const int y) const;
  int GetHeight() const;
  int GetWidth() const;
  int Return_active_neighbours(const int x, const int y);
  void Set(const int x, const int y, const int i);
  void Next();

  private:
  //Y-X ordered
  std::vector<std::vector<int>> m_grid;
  void Create_glider();

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GRID_H
