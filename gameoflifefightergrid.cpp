#include "gameoflifefightergrid.h"
#include <cassert>
#include <cstdlib>
#include <assert.h>

golf::Grid::Grid(const int width, const int height)
  : m_grid(height,std::vector<CellType>(width,CellType::empty))
{
  #ifndef NDEBUG
  Test();
  #endif
}

void golf::Grid::FlipHorizontal() noexcept
{
  const int height{GetHeight()};
  const int width{GetWidth()};
  const int maxx{width / 2};
  for (int y=0; y!=height; ++y)
  {
    auto& row = m_grid[y];
    for (int x=0; x!=maxx; ++x)
    {
      std::swap(row[x],row[width - 1 - x]);
    }
  }
}

void golf::Grid::Set(const int x, const int y, const CellType cell)
{
  assert(!m_grid.empty());
  assert(y >= 0);
  assert(y < static_cast<int>(m_grid.size()));
  assert(x >= 0);
  assert(x < static_cast<int>(m_grid[y].size()));
  m_grid[y][x] = cell;
}

void golf::Grid::Toggle(const int x, const int y)
{
  Set(x,y,GetOther(Get(x,y)));
}

golf::CellType golf::Grid::Get(const int x, const int y) const
{
  assert(!m_grid.empty());
  assert(y >= 0);
  assert(y < static_cast<int>(m_grid.size()));
  assert(x >= 0);
  assert(x < static_cast<int>(m_grid[y].size()));
  return m_grid[y][x];
}

int golf::Grid::GetHeight() const noexcept
{
  return static_cast<int>(m_grid.size());
}

int golf::Grid::CountNeighbours(const int focal_x, const int focal_y)
{
  int n = 0;
  for (int dy = -1; dy != 2; ++dy)
  {
    const int y_co = (focal_y + dy + GetHeight()) % GetHeight();
    for (int dx = -1; dx != 2; ++dx)
    {
      if (dx == 0 && dy == 0) continue;
      const int x_co = (focal_x + dx + GetWidth()) % GetWidth();
      if (Get(x_co,y_co) == CellType::alive) ++n;
    }
  }
  return n;
}

int golf::Grid::GetWidth() const
{
  assert(!m_grid.empty());
  return static_cast<int>(m_grid[0].size());
}

void golf::Grid::Next()
{
  //Game Of Life here
  //
  // Any live cell with fewer than two live neighbours dies
  // Any live cell with two or three live neighbours lives
  // Any live cell with more than three live neighbours dies
  // Any dead cell with exactly three live neighbours becomes a live cell

  const int width{GetWidth()};
  const int height{GetHeight()};

  std::vector<std::vector<CellType>> v(height,std::vector<CellType>(width,CellType::empty));
  for(int y = 0; y != height; ++y)
  {
    for(int x = 0; x != width; ++x)
    {
      const CellType status = Get(x,y);
      const int n_neighbours = CountNeighbours(x,y);
      if(status == CellType::alive)

      {
        if(n_neighbours < 2)
        {
          v[y][x] = CellType::empty;
        }
        else if(n_neighbours == 2 || n_neighbours ==  3)
        {
          v[y][x] = CellType::alive;
        }
        else if(n_neighbours > 3)
        {
          v[y][x] = CellType::empty;
        }
        else
        {
          v[y][x] = status;
        }
      }

      else
      {
        assert(status == CellType::empty);
        if(n_neighbours == 3)
        {
          v[y][x] = CellType::alive;
        }
        else
        {
          v[y][x] = CellType::empty;
        }
      }
    }
  }

  m_grid = v;
}

#ifndef NDEBUG
void golf::Grid::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  //Get and set must be symmetric
  {
    Grid g(200,200);
    g.Set(1,2,CellType::alive);
    assert(g.Get(1,2) == CellType::alive);
  }
  //Add a block
  {
    Grid g(40,40);
    g.Set(11,11,CellType::alive);
    g.Set(11,12,CellType::alive);
    g.Set(12,11,CellType::alive);
    g.Set(12,12,CellType::alive);
    g.Next();
    assert(g.Get(11,11) == CellType::alive);
    assert(g.Get(11,12) == CellType::alive);
    assert(g.Get(12,11) == CellType::alive);
    assert(g.Get(12,12) == CellType::alive);
  }
  //GetNeighbours: must return 4
  /*
   0 1 2
  +-+-+-+
  | |1| |0
  +-+-+-+
  |4|X|2|1 X == empty
  +-+-+-+
  | |3| |2
  +-+-+-+
+
  */
  {
    Grid g(40,40);
    g.Set(10,10,CellType::empty);
    g.Set(11,10,CellType::alive);
    g.Set(12,10,CellType::empty);
    g.Set(10,11,CellType::alive);
    g.Set(11,11,CellType::empty);
    g.Set(12,11,CellType::alive);
    g.Set(10,12,CellType::empty);
    g.Set(11,12,CellType::alive);
    g.Set(12,12,CellType::empty);
    assert(g.CountNeighbours(11,11) == 4);
  }
}
#endif
