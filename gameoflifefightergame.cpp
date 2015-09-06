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

void golf::Game::BuildOrRemove(const PlayerIndex player_index, const CellType cell_type)
{
  const auto player = GetPlayer(player_index);
  if (Hangar * const hangar = FindHangar(player.GetX(),player.GetY()))
  {
    if (hangar->GetState() == HangarState::closed
      && hangar->GetPlayerIndex() == player_index
    )
    {
      hangar->SetCell(
        player.GetX() - hangar->GetLeft(),
        player.GetY() - hangar->GetTop(),
        cell_type
      );
    }
  }
}

void golf::Game::BuildPattern(const PlayerIndex player_index, const int pattern_index)
{
  const auto player = GetPlayer(player_index);

  //Player must be in hangar
  if (Hangar * const hangar = FindHangar(player.GetX(),player.GetY()))
  {
    if (hangar->GetState() == HangarState::closed
      && hangar->GetPlayerIndex() == player_index
    )
    {
      hangar->BuildPattern(
        player.GetX() - hangar->GetLeft(), player.GetY() - hangar->GetTop(), //Position of cursor in Hangar
        player.GetPattern(pattern_index)
      );
    }
  }
}

void golf::Game::CloseHangar(const PlayerIndex player_index)
{
  const auto player = this->GetPlayer(player_index);
  const auto iter = std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [player](const Hangar& hangar) { return hangar.IsIn(player); }
  );
  if (iter == std::end(m_hangars)) return;
  (*iter).Close(m_grid);
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
  v.push_back(Player( (width * 1 / 4) - 5,(height / 2) - 5));
  v.push_back(Player( (width * 3 / 4) - 5,(height / 2) + 5));
  return v;
}

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

golf::CellType golf::Game::GetCell(const int x, const int y) const
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  return m_grid.Get(x,y);
}

const golf::Player& golf::Game::GetPlayer(const PlayerIndex player_index) const noexcept
{
  switch (player_index)
  {
    case PlayerIndex::player1:
      assert(m_players.size() >= 2);
      return m_players[0];
    case PlayerIndex::player2:
      assert(m_players.size() >= 2);
      return m_players[1];
    default:
      assert(!"golf::Game::GetPlayer: Unimplemented PlayerIndex");
      throw std::logic_error("golf::Game::GetPlayer: Unimplemented PlayerIndex");
  }
}

const golf::Game::Players& golf::Game::GetPlayers() const noexcept
{
  TRACE("Prefer using GetPlayer(PlayerIndex)");
  return m_players;
}

const golf::Hangar * golf::Game::FindHangar(const int x, const int y) const noexcept
{
  const auto iter = std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [x,y](const Hangar& hangar) { return hangar.IsIn(x,y); }
  );
  if (iter == std::end(m_hangars)) return nullptr;
  return &(*iter);
}

golf::Hangar * golf::Game::FindHangar(const int x, const int y) noexcept
{
  return const_cast<Hangar*>(const_cast<const Game *>(this)->FindHangar(x,y));
}

bool golf::Game::IsHangar(const int x, const int y) const noexcept
{
  return FindHangar(x,y);
}

void golf::Game::Next()
{
  m_grid.Next();

  //Kill all cells within closed hangars (note: the cells within hangars are stored within Hangar
  const bool kill_all_cells_under_hangars{false};
  if (kill_all_cells_under_hangars)
  {
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
}

void golf::Game::OpenHangar(const PlayerIndex player_index)
{
  const auto player = this->GetPlayer(player_index);
  const auto iter = std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [player](const Hangar& hangar) { return hangar.IsIn(player); }
  );
  if (iter == std::end(m_hangars)) return;
  (*iter).Open(m_grid);
}

void golf::Game::PressKeys(const std::set<Key>& keys)
{
  auto& player1 = m_players[0];
  auto& player2 = m_players[1];
  for (const auto key: keys)
  {

    switch (key)
    {
      case Key::close_hangar1: CloseHangar(PlayerIndex::player1); break;
      case Key::close_hangar2: CloseHangar(PlayerIndex::player2); break;
      case Key::down1: player1.SetY((player1.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::down2: player2.SetY((player2.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::left1: player1.SetX((player1.GetX() - 1 + GetWidth()) % GetWidth()); break;
      case Key::left2: player2.SetX((player2.GetX() - 1 + GetWidth()) % GetWidth()); break;
      case Key::open_hangar1: OpenHangar(PlayerIndex::player1); break;
      case Key::open_hangar2: OpenHangar(PlayerIndex::player2); break;
      case Key::pattern_a1: BuildPattern(PlayerIndex::player1,0); break;
      case Key::pattern_a2: BuildPattern(PlayerIndex::player2,0); break;
      case Key::pattern_b1: BuildPattern(PlayerIndex::player1,1); break;
      case Key::pattern_b2: BuildPattern(PlayerIndex::player2,1); break;
      case Key::pattern_c1: BuildPattern(PlayerIndex::player1,2); break;
      case Key::pattern_c2: BuildPattern(PlayerIndex::player2,2); break;
      case Key::quit: break; //Cannot handle quit here
      case Key::right1: player1.SetX((player1.GetX() + 1 + GetWidth()) % GetWidth()); break;
      case Key::right2: player2.SetX((player2.GetX() + 1 + GetWidth()) % GetWidth()); break;
      case Key::set_high1: BuildOrRemove(PlayerIndex::player1,CellType::alive); break;
      case Key::set_high2: BuildOrRemove(PlayerIndex::player2,CellType::alive); break;
      case Key::set_low1: BuildOrRemove(PlayerIndex::player1,CellType::empty); break;
      case Key::set_low2: BuildOrRemove(PlayerIndex::player2,CellType::empty); break;
      case Key::up1: player1.SetY((player1.GetY() - 1 + GetHeight()) % GetHeight()); break;
      case Key::up2: player2.SetY((player2.GetY() - 1 + GetHeight()) % GetHeight()); break;
    }
  }

}

void golf::Game::Set(const int x, const int y, const CellType cell)
{
  assert(y >= 0);
  assert(y < GetHeight());
  assert(x >= 0);
  assert(x < GetWidth());
  m_grid.Set(x,y,cell);
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
