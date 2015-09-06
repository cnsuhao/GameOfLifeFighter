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

void golf::Grid::Set(const int x, const int y, const CellType cell)
{
  assert(!m_grid.empty());
  assert(y >= 0);
  assert(y < static_cast<int>(m_grid.size()));
  assert(x >= 0);
  assert(x < static_cast<int>(m_grid[y].size()));
  m_grid[y][x] = cell;
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

int golf::Grid::Return_active_neighbours(const int x, const int y)
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
        if(m_grid[y_co][x_co] == CellType::alive) {counter++;}
      }
    }

    if(m_grid[y][x] == CellType::alive) {return counter-1;}
    else{assert(m_grid[y][x] == CellType::empty); return counter;};
  }

  else{return std::rand() % 8;};
}

int golf::Grid::GetWidth() const
{
  assert(!m_grid.empty());
  return static_cast<int>(m_grid[0].size());
}

void golf::Grid::Create_glider()
{
  int hight = GetHeight();
  int width = GetWidth();
  assert(hight > 20);
  assert(width > 20);
  m_grid[9][7] = CellType::alive;
  m_grid[9][8] = CellType::alive;
  m_grid[9][9] = CellType::alive;
  m_grid[8][9] = CellType::alive;
  m_grid[7][8] = CellType::alive;
}

void golf::Grid::Create_block()
{
  int hight = GetHeight();
  int width = GetWidth();
  assert(hight > 20);
  assert(width > 20);

  m_grid[9][9] = CellType::alive;
  m_grid[8][8] = CellType::alive;
  m_grid[9][8] = CellType::alive;
  m_grid[8][9] = CellType::alive;
}

void golf::Grid::Create_blinker()
{
  int hight = GetHeight();
  int width = GetWidth();
  assert(hight > 20);
  assert(width > 20);

  m_grid[9][9] = CellType::alive;
  m_grid[9][8] = CellType::alive;
  m_grid[9][7] = CellType::alive;
}

void golf::Grid::Next()
{
  //Game Of Life here
  //
  // Any live cell with fewer than two live neighbours dies
  // Any live cell with two or three live neighbours lives
  // Any live cell with more than three live neighbours dies
  // Any dead cell with exactly three live neighbours becomes a live cell

  std::vector<std::vector<CellType>> grid_temp(m_grid.size(),std::vector<CellType>(m_grid[0].size(),CellType::empty));

  for(int i = 0; i < static_cast<int>(m_grid[0].size()); ++i)
  {
    for(int j = 0; j < static_cast<int>(m_grid.size()); ++j)
    {
      CellType temp_status = m_grid[j][i];
      int temp_neighbours = Return_active_neighbours(i,j);
      if(temp_status == CellType::alive)
      {
        if(temp_neighbours < 2)
        {
          grid_temp[j][i] = CellType::empty;
        }
        else if(temp_neighbours == 2 || temp_neighbours ==  3)
        {
          grid_temp[j][i] = CellType::alive;
        }
        else if(temp_neighbours > 3)
        {
          grid_temp[j][i] = CellType::empty;
        }
        else
        {
          grid_temp[j][i] = m_grid[j][i];
        }
      }

      else
      {
        assert(temp_status == CellType::empty);
        if(temp_neighbours == 3)
        {
          grid_temp[j][i] = CellType::alive;
        }
        else
        {
          grid_temp[j][i] = CellType::empty;
        }
      }
    }
  }

  m_grid = grid_temp;
}

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
    assert(g.Return_active_neighbours(11,11) == 4);
  }
  //Add a blink
  /*
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
  */
}
