#include "gameoflifefightergame.h"

#include <cassert>
#include <cstdlib>
#include "gameoflifefighterhangar.h"

golf::Game::Game()
  : m_grid(GetWidth(),GetHeight()),
    m_hangars{CreateInitialHangars(width,height)}
{
  #ifndef NDEBUG
  Test();
  #endif

    m_x1{width / 2},
    m_y1{height * 1 / 4},
    m_x2{width / 2},
    m_y2{height * 3 / 4}

}

std::vector<golf::Hangar> golf::Game::CreateInitialHangars(const int width, const int height)
{
  assert(width > 0);
  assert(height > 0);
  std::vector<Hangar> v;
  v.push_back(Hangar(30,10,10,10,Player::player1));
  v.push_back(Hangar(50,10,10,10,Player::player1));
  v.push_back(Hangar(30,30,10,10,Player::player1));
  v.push_back(Hangar(50,30,10,10,Player::player1));
  v.push_back(Hangar(130,20,10,10,Player::player2));
  v.push_back(Hangar(150,20,10,10,Player::player2));
  v.push_back(Hangar(130,40,10,10,Player::player2));
  v.push_back(Hangar(150,40,10,10,Player::player2));
  return v;
}

void golf::Game::Set(const int x, const int y, const int i)
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  m_grid.Set(x,y,i);
}

int golf::Game::GetGrid(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  return m_grid.Get(x,y);
}

int golf::Game::GetHeight() const
{
  return m_grid.GetHeight();
}

int golf::Game::GetWidth() const
{
  return m_grid.GetWidth();
}

void golf::Game::Next()
{
  m_grid.Next();

  //Kill all cells within closed hangars (note: the cells within hangars are stored within Hangar
  for (const Hangar& hangar: this->GetHangars())
  {
    const int left{hangar.GetLeft()};
    const int right{hangar.GetLeft() + hangar.GetWidth()};
    const int top{hangar.GetTop()};
    const int bottom{hangar.GetTop() + hangar.GetHeight()};
    for (int y=top; y!=bottom; ++y)
    {
      for (int x=left; x!=right; ++x)
      {
        m_grid.Set(x,y,0);
      }
    }
  }
}

void golf::Game::PressKeys(const std::set<Key>& keys)
{
  for (const auto key: keys)
  {
    switch (key)
    {
      case Qt::Key_A: m_x1 = (m_x1 - 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_D: m_x1 = (m_x1 + 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_W: m_y1 = (m_y1 - 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_S: m_y1 = (m_y1 + 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_Q: m_game.Set(m_x1,m_y1,1); break;

      case Qt::Key_J: m_x2 = (m_x2 - 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_L: m_x2 = (m_x2 + 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_I: m_y2 = (m_y2 - 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_K: m_y2 = (m_y2 + 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_U: m_game.Set(m_x2,m_y2,1); break;
    }
  }

}

#ifndef NDEBUG
void golf::Game::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  //Get and set must be symmetric
  {
    Game g(40,30);
    g.Set(1,2,42);
    assert(g.GetGrid(1,2) == 42);
  }
}
#endif
