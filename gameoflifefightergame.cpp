#include "gameoflifefightergame.h"

#include <cassert>
#include <cstdlib>
#include "gameoflifefighterhangar.h"
#include "gameoflifefightertrace.h"

golf::Game::Game()
  : m_grid(GetWidth(),GetHeight()),
    m_hangars{CreateInitialHangars(GetWidth(),GetHeight())},
    m_players{CreateInitialPlayers(GetWidth(),GetHeight())}
{
  #ifndef NDEBUG
  Test();
  #endif
}

///Obtain the complete global Grid as bit flags
/// 00: empty, not hangar
/// 01: alive, not hangar
/// 10: empty, in hangar
/// 11: alive, in hangar
golf::Game::BitFlagGrid golf::Game::GetBitFlagGrid() const
{
  const int h{m_grid.GetHeight()};
  const int w{m_grid.GetWidth()};
  BitFlagGrid v(
    h,
    std::vector<int>(w,0)
  );
  assert(m_grid.GetRawGrid().size() == v.size());
  assert(m_grid.GetRawGrid()[0].size() == v[0].size());
  for (int y=0; y!=h; ++y)
  {
    for (int x=0; x!=w; ++x)
    {
      int bit = 0;
      bit += (m_grid.Get(x,y) == CellType::alive? 1 : 0);
      bit += (IsHangar(x,y)                     ? 2 : 0);
      v[y][x] = bit;
    }
  }
  return v;
}

golf::Game::Hangars golf::Game::CreateInitialHangars(const int width, const int height)
{
  assert(width > 0);
  assert(height > 0);
  Hangars v;
  v.push_back(Hangar(30,10,10,10,PlayerIndex::player1));
  v.push_back(Hangar(50,10,10,10,PlayerIndex::player1));
  v.push_back(Hangar(30,30,10,10,PlayerIndex::player1));
  v.push_back(Hangar(50,30,10,10,PlayerIndex::player1));
  v.push_back(Hangar(130,20,10,10,PlayerIndex::player2));
  v.push_back(Hangar(150,20,10,10,PlayerIndex::player2));
  v.push_back(Hangar(130,40,10,10,PlayerIndex::player2));
  v.push_back(Hangar(150,40,10,10,PlayerIndex::player2));
  return v;
}

golf::Game::Players golf::Game::CreateInitialPlayers(const int width, const int height)
{
  assert(width > 0);
  assert(height > 0);
  Players v;
  v.push_back(Player(width / 2,height * 1 / 4));
  v.push_back(Player(width / 2,height * 3 / 4));
  return v;
}

void golf::Game::Set(const int x, const int y, const CellType cell)
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  m_grid.Set(x,y,cell);
}

golf::CellType golf::Game::GetCell(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  return m_grid.Get(x,y);
}

bool golf::Game::IsHangar(const int x, const int y) const noexcept
{
  return std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [x,y](const Hangar& hangar) { return hangar.IsIn(x,y); }
  ) != std::end(m_hangars);
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
        m_grid.Set(x,y,CellType::empty);
      }
    }
  }
}

void golf::Game::PressKeys(const std::set<Key>& keys)
{
  auto& player1 = m_players[0];
  auto& player2 = m_players[1];
  for (const auto key: keys)
  {

    switch (key)
    {
      case Key::close_hangar1:
      {
        const auto player1 = this->GetPlayers()[0];
        const auto iter = std::find_if(
          std::begin(m_hangars),
          std::end(m_hangars),
          [player1](const Hangar& hangar) { return hangar.IsIn(player1); }
        );
        if (iter == std::end(m_hangars)) return;
        (*iter).Close(m_grid);
      }
      break;
      case Key::close_hangar2:
      {
        const auto player2 = this->GetPlayers()[1];
        const auto iter = std::find_if(
          std::begin(m_hangars),
          std::end(m_hangars),
          [player2](const Hangar& hangar) { return hangar.IsIn(player2); }
        );
        if (iter == std::end(m_hangars)) return;
        (*iter).Close(m_grid);
      }
      break;
      case Key::down1: player1.SetY((player1.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::down2: player2.SetY((player2.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::left1: player1.SetX((player1.GetX() - 1 + GetWidth()) % GetWidth()); break;
      case Key::left2: player2.SetX((player2.GetX() - 1 + GetWidth()) % GetWidth()); break;
      case Key::open_hangar1:
      {
        const auto player1 = this->GetPlayers()[0];
        const auto iter = std::find_if(
          std::begin(m_hangars),
          std::end(m_hangars),
          [player1](const Hangar& hangar) { return hangar.IsIn(player1); }
        );
        if (iter == std::end(m_hangars)) return;
        (*iter).Open(m_grid);
      }
      break;
      case Key::open_hangar2:
      {
        const auto player2 = this->GetPlayers()[1];
        const auto iter = std::find_if(
          std::begin(m_hangars),
          std::end(m_hangars),
          [player2](const Hangar& hangar) { return hangar.IsIn(player2); }
        );
        if (iter == std::end(m_hangars)) return;
        (*iter).Open(m_grid);
      }
      break;
      case Key::right1: player1.SetX((player1.GetX() + 1 + GetWidth()) % GetWidth()); break;
      case Key::right2: player2.SetX((player2.GetX() + 1 + GetWidth()) % GetWidth()); break;
      case Key::set_high1: Set(player1.GetX(),player1.GetY(),CellType::alive); break;
      case Key::set_high2: Set(player2.GetX(),player2.GetY(),CellType::alive); break;
      case Key::set_low1: Set(player1.GetX(),player1.GetY(),CellType::empty); break;
      case Key::set_low2: Set(player2.GetX(),player2.GetY(),CellType::empty); break;
      case Key::up1: player1.SetY((player1.GetY() - 1 + GetHeight()) % GetHeight()); break;
      case Key::up2: player2.SetY((player2.GetY() - 1 + GetHeight()) % GetHeight()); break;
      case Key::quit: //Cannot handle quit here
        break;
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
    Game g;
    g.Set(1,2,CellType::alive);
    assert(g.GetCell(1,2) == CellType::alive);
  }
  //A key down press should move player 2 down
  {
    Game game;
    const int y_before{game.GetPlayers()[1].GetY()};
    game.PressKeys( { Key::down2 } );
    const int y_after{game.GetPlayers()[1].GetY()};
    assert(y_after == y_before + 1);
  }
}
#endif
