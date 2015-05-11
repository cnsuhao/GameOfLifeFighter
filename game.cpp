#include "game.h"

#include <cassert>
#include <cstdlib>

Game::Game(const int width, const int height)
  : m_grid(width,height)
{
  #ifndef NDEBUG
  Test();
  #endif
}

void Game::Set(const int x, const int y, const int i)
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  m_grid.Set(x,y,i);
}

int Game::GetGrid(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  return m_grid.Get(x,y);
}

int Game::GetHeight() const
{
  return m_grid.GetHeight();
}

int Game::GetWidth() const
{
  return m_grid.GetWidth();
}

void Game::Next()
{
  m_grid.Next();
}

#ifndef NDEBUG
void Game::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  //Get and set must be symmetric
  {
    Game g(4,3);
    g.Set(1,2,42);
    assert(g.GetGrid(1,2) == 42);
  }
}
#endif
