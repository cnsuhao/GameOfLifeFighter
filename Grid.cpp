#include "Grid.h"

#include <cassert>

Grid::Grid(const int width, const int height)
  : m_grid(height,std::vector<int>(width,0))
{
  #ifndef NDEBUG
  Test();
  #endif


}

void Grid::Set(const int x, const int y, const int i)
{
  assert(!m_grid.empty());
  assert(y >= 0);
  assert(y < static_cast<int>(m_grid.size()));
  assert(x >= 0);
  assert(x < static_cast<int>(m_grid[y].size()));
  m_grid[y][x] = i;
}

int Grid::Get(const int x, const int y) const
{
  assert(!m_grid.empty());
  assert(y >= 0);
  assert(y < static_cast<int>(m_grid.size()));
  assert(x >= 0);
  assert(x < static_cast<int>(m_grid[y].size()));
  return m_grid[y][x];
}


void Grid::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  //Get and set must be symmetric
  {
    Grid g(4,3);
    g.Set(1,2,42);
    assert(g.Get(1,2) == 42);
  }
}
