#include "grid.h"

#include <cassert>
#include <cstdlib>

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

int Grid::GetHeight() const
{
  return static_cast<int>(m_grid.size());
}

int Grid::GetWidth() const
{
  assert(!m_grid.empty());
  return static_cast<int>(m_grid[0].size());
}

void Grid::Next()
{
  //Game Of Life here
  //
  // Any live cell with fewer than two live neighbours dies
  // Any live cell with two or three live neighbours lives
  // Any live cell with more than three live neighbours dies
  // Any dead cell with exactly three live neighbours becomes a live cell

  //STUB:
  const int x{std::rand() % GetWidth()};
  const int y{std::rand() % GetHeight()};
  Set(x,y,1);
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
