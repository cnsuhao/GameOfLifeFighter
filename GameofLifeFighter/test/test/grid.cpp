#include "grid.h"

#include <cassert>
#include <cstdlib>
#include <assert.h>

Grid::Grid(const int width, const int height)
  : m_grid(height,std::vector<int>(width,0))
{
  #ifndef NDEBUG
  Test();
  Create_block();
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

void Grid::Next()
{
  //Game Of Life here
  //
  // Any live cell with fewer than two live neighbours dies
  // Any live cell with two or three live neighbours lives
  // Any live cell with more than three live neighbours dies
  // Any dead cell with exactly three live neighbours becomes a live cell

  //STUB:
  for(int i = 0; i < static_cast<int>(m_grid[0].size()); ++i)
    {
    for(int j = 0; j < static_cast<int>(m_grid.size()); ++j)
        {
            int temp_status = m_grid[j][i];
            int temp_neighbours = Return_active_neighbours(i,j);
            if(temp_status == 1)
            {
                if(temp_neighbours < 2)
                    {
                    m_grid[j][i] = 0;
                    }
                else if(temp_neighbours == 2 || temp_neighbours ==  3)
                    {
                    m_grid[j][i] = 1;
                    }
                else if(temp_neighbours > 3)
                    {
                    m_grid[j][i] = 0;
                    }
            }

            else
            {
                assert(temp_status == 0);
                if(temp_neighbours == 3)
                {
                m_grid[j][i] = 1;
                }
            }
        }
    }

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
