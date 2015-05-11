#include "grid.h"
#include <cassert>
#include <cstdlib>
#include <assert.h>

Grid::Grid(const int width, const int height)
  : m_grid(height,std::vector<int>(width,0))
{
  #ifndef NDEBUG
  Test();
  Create_glider();
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

int Grid::Return_active_neighbours(const int x, const int y)
{
  if(x > 0 && x < static_cast<int>(m_grid[0].size())-1 && y > 0 && y < static_cast<int>(m_grid.size())-1)
    {
    int counter = 0;
    for(int q = -1; q < 2; ++q)
        {
        for(int k = -1; k < 2; ++k)
            {
                const int y_co = y+q;
                const int x_co = x+k;
                assert(y_co >= 0);
                assert(y_co < static_cast<int>(m_grid.size()));
                assert(x_co >= 0);
                assert(x_co < static_cast<int>(m_grid[y_co].size()));
                if(m_grid[y_co][x_co] == 1) {counter++;}
            }
        }
    return counter-1;
    }

else{return std::rand() % 8;};

}

int Grid::GetWidth() const
{
  assert(!m_grid.empty());
  return static_cast<int>(m_grid[0].size());
}

void Grid::Create_glider()
{
int hight = GetHeight();
int width = GetWidth();
assert(hight > 20);
assert(width > 20);
m_grid[9][7] = 1;
m_grid[9][8] = 1;
m_grid[9][9] = 1;
m_grid[8][9] = 1;
m_grid[7][8] = 1;
}

void Grid::Create_block()
{
int hight = GetHeight();
int width = GetWidth();
assert(hight > 20);
assert(width > 20);

m_grid[9][9] = 1;
m_grid[8][8] = 1;
m_grid[9][8] = 1;
m_grid[8][9] = 1;
}

void Grid::Create_blinker()
{
int hight = GetHeight();
int width = GetWidth();
assert(hight > 20);
assert(width > 20);

m_grid[9][9] = 1;
m_grid[9][8] = 1;
m_grid[9][7] = 1;
}

void Grid::Next()
{
  //Game Of Life here
  //

  //STUB:

  std::vector<std::vector<int>> grid_temp(m_grid.size(),std::vector<int>(m_grid[0].size(),0));

  for(int x = 0; x < static_cast<int>(m_grid[0].size()); ++x)
  {
    for(int y = 0; y < static_cast<int>(m_grid.size()); ++y)
    {
      const int current_status = m_grid[y][x];
      const int n_neighbours = Return_active_neighbours(x,y);
      if(current_status == 1)
      {
        if(n_neighbours < 2)
        {
          // Any live cell with fewer than two live neighbours dies
          grid_temp[y][x] = 0;
        }
        else if(n_neighbours == 2 || n_neighbours ==  3)
        {
          // Any live cell with two or three live neighbours lives
          grid_temp[y][x] = 1;
        }
        else if(n_neighbours > 3)
        {
          // Any live cell with more than three live neighbours dies
          grid_temp[y][x] = 0;
        }
        else
        {
          grid_temp[y][x] = m_grid[y][x];
        }
      }
      else
      {
        // Any dead cell with exactly three live neighbours becomes a live cell
        assert(current_status == 0);
        if(n_neighbours == 3)
        {
          grid_temp[y][x] = 1;
        }
        else
        {
          grid_temp[y][x] = 0;
        }
      }
    }
  }

  m_grid = grid_temp;
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
    Grid g(200,200);
    g.Set(1,2,42);
    assert(g.Get(1,2) == 42);
  }
  //Add a block
  {
    Grid g(40,40);
    g.Set(11,11,1);
    g.Set(11,12,1);
    g.Set(12,11,1);
    g.Set(12,12,1);
    g.Next();
    assert(g.Get(11,11) == 1);
    assert(g.Get(11,12) == 1);
    assert(g.Get(12,11) == 1);
    assert(g.Get(12,12) == 1);
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

  */
  {
    Grid g(40,40);
    g.Set(10,10,0);
    g.Set(11,10,1);
    g.Set(12,10,0);
    g.Set(10,11,1);
    g.Set(11,11,0);
    g.Set(12,11,1);
    g.Set(10,12,0);
    g.Set(11,12,1);
    g.Set(12,12,0);
    assert(g.Return_active_neighbours(11,11) == 4);
  }
  //Add a blink
  {
    Grid g(40,40);
    g.Set(11,11,1);
    g.Set(12,11,1);
    g.Set(13,11,1);
    g.Next();
    assert(g.Get(12,11) == 1);
    assert(g.Get(12,12) == 1);
    assert(g.Get(12,13) == 1);
  }

}
