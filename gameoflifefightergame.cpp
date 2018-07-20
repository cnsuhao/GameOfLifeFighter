#include "gameoflifefightergame.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <stdexcept>

#include "gameoflifefighterhangar.h"
#include "gameoflifefightertrace.h"

golf::Game::Game()
  :
    m_game_state{GameState::playing},
    m_grid(GetWidth(),GetHeight()),
    m_hangars{CreateInitialHangars()},
    m_hearts{CreateInitialHearts()},
    m_players{CreateInitialPlayers()}
{
  #ifndef NDEBUG
  Test();
  #endif

  for (const auto& heart: m_hearts)
  {
    const int left{heart.GetLeft()};
    const int top{heart.GetTop()};
    const int width{heart.GetWidth()};
    const int height{heart.GetHeight()};
    m_grid.Set( 0 + (left + (width / 2)), 0 + (top + (height / 2)),CellType::alive);
    m_grid.Set( 0 + (left + (width / 2)),-1 + (top + (height / 2)),CellType::alive);
    m_grid.Set(-1 + (left + (width / 2)), 0 + (top + (height / 2)),CellType::alive);
    m_grid.Set(-1 + (left + (width / 2)),-1 + (top + (height / 2)),CellType::alive);
  }
}

bool golf::Game::CanBuild(const PlayerIndex player_index) const noexcept
{
  const auto player = GetPlayer(player_index);
  return CanBuildHere(
    player_index,
    player.GetX(),
    player.GetY()
  );
}

bool golf::Game::CanBuildHere(const PlayerIndex player_index, const int x, const int y) const noexcept
{
  const Hangar * const hangar = FindHangar(x,y);
  if (!hangar) return false;
  if (hangar->GetState() == HangarState::open) return false;
  if (hangar->GetPlayerIndex() != player_index) return false;
  if (IsHeart(x,y)) return false;
  return true;
}

bool golf::Game::CanBuildPattern(const PlayerIndex player_index, const int pattern_index) const noexcept
{
  if (!CanBuild(player_index)) return false;

  const auto player = GetPlayer(player_index);
  //Pattern can be built if the four corners are within a hangar and outside of the heart
  const auto pattern = GetPattern(player_index,pattern_index);
  const auto left = player.GetX() - pattern.GetCursorX();
  const auto right = player.GetX() - pattern.GetCursorX() + pattern.GetWidth() - 1;
  const auto top = player.GetY() - pattern.GetCursorY();
  const auto bottom = player.GetY() - pattern.GetCursorY() + pattern.GetHeight() - 1;
  if (!CanBuildHere(player_index,left,top)) return false;
  if (!CanBuildHere(player_index,left,bottom)) return false;
  if (!CanBuildHere(player_index,right,top)) return false;
  if (!CanBuildHere(player_index,right,bottom)) return false;
  return true;
}


void golf::Game::ToggleCell(const PlayerIndex player_index)
{
  if (!CanBuild(player_index)) return;

  const auto player = GetPlayer(player_index);
  Hangar * const hangar = FindHangar(player.GetX(),player.GetY());
  assert(hangar);
  hangar->ToggleCell(
    player.GetX() - hangar->GetLeft(),
    player.GetY() - hangar->GetTop()
  );
}

void golf::Game::BuildPattern(const PlayerIndex player_index, const int pattern_index)
{
  if (!CanBuildPattern(player_index,pattern_index)) return;

  const auto player = GetPlayer(player_index);

  //Player must be in hangar
  Hangar * const hangar = FindHangar(player.GetX(),player.GetY());
  assert(hangar);
  hangar->BuildPattern(
    player.GetX() - hangar->GetLeft(), player.GetY() - hangar->GetTop(), //Position of cursor in Hangar
    player.GetPattern(pattern_index)
  );
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

golf::Game::Hangars golf::Game::CreateInitialHangars()
{
  Hangars v;
  const bool do_transfer_up = false;
  const int w{GetWidth()};
  const int h{GetHeight()};
  v.push_back(Hangar(             0,0,w / 2,h,PlayerIndex::player1,do_transfer_up));
  v.push_back(Hangar(GetWidth() / 2,0,w / 2,h,PlayerIndex::player2,do_transfer_up));
  return v;
}

golf::Game::Hearts golf::Game::CreateInitialHearts()
{
  Hearts v;
  const int sz = 10;
  const int w{GetWidth()};
  const int h{GetHeight()};
  v.push_back(Heart((w * 1 / 4) - (sz / 2),(h / 2) - (sz / 2),sz,sz,PlayerIndex::player1));
  v.push_back(Heart((w * 3 / 4) - (sz / 2),(h / 2) - (sz / 2),sz,sz,PlayerIndex::player2));
  return v;
}

golf::Game::Players golf::Game::CreateInitialPlayers()
{
  Players v;
  const int w{GetWidth()};
  const int h{GetHeight()};
  v.push_back(Player(PlayerIndex::player1,w * 1 / 4, h / 2));
  v.push_back(Player(PlayerIndex::player2,w * 3 / 4, h / 2));
  return v;
}

golf::Game::CellStateGrid golf::Game::GetCellStateGrid() const
{
  const int h{m_grid.GetHeight()};
  const int w{m_grid.GetWidth()};
  CellStateGrid cell_states(
    h,
    std::vector<CellState>(w,
      CellState(
        HangarOf::none,
        HeartOf::none,
        IsBuilding::none,
        CellType::empty
      )
    )
  );
  assert(m_grid.GetRawGrid().size() == cell_states.size());
  assert(m_grid.GetRawGrid()[0].size() == cell_states[0].size());
  //Cell empty/alive
  for (int y=0; y!=h; ++y)
  {
    auto& cell_states_row = cell_states[y];
    for (int x=0; x!=w; ++x)
    {
      auto& cell_state = cell_states_row[x];
      cell_state.SetCellType(m_grid.Get(x,y));
    }
  }
  //Display hangars
  for (const Hangar& hangar: GetHangars())
  {
    const int left{hangar.GetLeft()};
    const int top{hangar.GetTop()};
    const int width{hangar.GetWidth()};
    const int height{hangar.GetHeight()};
    const auto player_index = hangar.GetPlayerIndex();
    HangarOf is_hangar_of = HangarOf::none;
    switch (player_index)
    {
      case PlayerIndex::player1: is_hangar_of = HangarOf::player1; break;
      case PlayerIndex::player2: is_hangar_of = HangarOf::player2; break;
    }

    //Darker if the Hangar is open
    //const auto player_color = ToColor(player_index);
    //const auto hangar_color = hangar.GetState() == HangarState::open
    //  ? QtHelper().Blend(player_color,qRgb(0,0,0))
    //  : player_color
    //;
    for (int y=0; y!=height; ++y)
    {
      assert(y + top >= 0);
      assert(y + top < static_cast<int>(cell_states.size()));
      auto& row = cell_states[y + top];
      for (int x=0; x!=width; ++x)
      {
        assert(x + left >= 0);
        assert(x + left < static_cast<int>(row.size()));
        auto& cell = row[x + left];
        cell_states[y + top][x + left].SetHangarOf(is_hangar_of);
        if (hangar.GetCell(x,y) == CellType::alive)
        {
          cell.SetIsBuilding(hangar.GetPlayerIndex());
        }
        else
        {
          cell.SetIsBuilding(IsBuilding::none);
        }
      }
    }

  }

  //Display hearts
  for (const Heart& heart: GetHearts())
  {
    const int left{heart.GetLeft()};
    const int top{heart.GetTop()};
    const int width{heart.GetWidth()};
    const int height{heart.GetHeight()};
    const auto player_index = heart.GetPlayerIndex();
    HeartOf is_heart_of = HeartOf::none;
    switch (player_index)
    {
      case PlayerIndex::player1: is_heart_of = HeartOf::player1; break;
      case PlayerIndex::player2: is_heart_of = HeartOf::player2; break;
    }

    for (int y=0; y!=height; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        assert(y + top >= 0);
        assert(y + top < static_cast<int>(cell_states.size()));
        assert(x + left >= 0);
        assert(x + left < static_cast<int>(cell_states[0].size()));
        cell_states[y + top][x + left].SetHeartOf(is_heart_of);

      }
    }

  }
  return cell_states;
}

const golf::PrefabPattern& golf::Game::GetPattern(const PlayerIndex player_index, const int pattern_index) const
{
  return GetPlayer(player_index).GetPattern(pattern_index);
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
      //throw std::logic_error("golf::Game::GetPlayer: Unimplemented PlayerIndex");
      return m_players[0];
  }
}

const golf::Hangar * golf::Game::FindHangar(const int x, const int y) const noexcept
{
  const auto iter = std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [x,y](const Hangar& hangar) { return hangar.IsIn(x,y); }
  );
  if (iter == std:: end(m_hangars)) return nullptr;
  return &(*iter);
}

golf::Hangar * golf::Game::FindHangar(const int x, const int y) noexcept
{
  return const_cast<Hangar*>(const_cast<const Game *>(this)->FindHangar(x,y));
}

const golf::Heart * golf::Game::FindHeart(const int x, const int y) const noexcept
{
  const auto iter = std::find_if(
    std::begin(m_hearts),
    std::end(m_hearts),
    [x,y](const Heart& heart) { return heart.IsIn(x,y); }
  );
  if (iter == std::end(m_hearts)) return nullptr;
  return &(*iter);
}

golf::Heart * golf::Game::FindHeart(const int x, const int y) noexcept
{
  return const_cast<Heart*>(const_cast<const Game *>(this)->FindHeart(x,y));
}

bool golf::Game::IsHangar(const int x, const int y) const noexcept
{
  return FindHangar(x,y);
}

bool golf::Game::IsHeart(const int x, const int y) const noexcept
{
  return FindHeart(x,y);
}

void golf::Game::Next()
{
  if (m_game_state != GameState::playing) return;

  //In free fight mode, a Hangar cannot be open for more than one turn
  for (auto& hangar: m_hangars) { hangar.Close(m_grid); }

  m_grid.Next();

  for (const auto heart: m_hearts)
  {
    if (heart.IsEmpty(m_grid))
    {
      if (heart.GetPlayerIndex() == PlayerIndex::player1) { m_game_state = GameState::player1_won; }
      if (heart.GetPlayerIndex() == PlayerIndex::player2) { m_game_state = GameState::player2_won; }
    }
  }

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

void golf::Game::PressKeys(const std::vector<Key>& keys)
{
  if (m_game_state != GameState::playing) return;
  auto& player1 = m_players[0];
  auto& player2 = m_players[1];
  for (const auto key: keys) {
    switch (key) {
      case Key::up1: player1.SetY((player1.GetY() - 1 + GetHeight()) % GetHeight()); break;
      case Key::up2: player2.SetY((player2.GetY() - 1 + GetHeight()) % GetHeight()); break;
      case Key::down1: player1.SetY((player1.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::down2: player2.SetY((player2.GetY() + 1 + GetHeight()) % GetHeight()); break;
      case Key::left1:
      {
        const int new_x{(player1.GetX() - 1 + GetWidth()) % GetWidth()};
        const auto hangar = FindHangar(new_x,player1.GetY());
        if (hangar && hangar->GetPlayerIndex() == PlayerIndex::player1) {
          player1.SetX(new_x);
        }
      }
      break;
      case Key::left2:
      {
        const int new_x{(player2.GetX() - 1 + GetWidth()) % GetWidth()};
        const auto hangar = FindHangar(new_x,player2.GetY());
        if (hangar && hangar->GetPlayerIndex() == PlayerIndex::player2) {
          player2.SetX(new_x);
        }
      }
      break;
      case Key::right1:
      {
        const int new_x{(player1.GetX() + 1 + GetWidth()) % GetWidth()};
        const auto hangar = FindHangar(new_x,player1.GetY());
        if (hangar && hangar->GetPlayerIndex() == PlayerIndex::player1) {
          player1.SetX(new_x);
        }
      }
      break;
      case Key::right2:
      {
        const int new_x{(player2.GetX() + 1 + GetWidth()) % GetWidth()};
        const auto hangar = FindHangar(new_x,player2.GetY());
        if (hangar && hangar->GetPlayerIndex() == PlayerIndex::player2) {
          player2.SetX(new_x);
        }
      }
      break;
      case Key::toggle_hangar1: ToggleHangar(PlayerIndex::player1); break;
      case Key::toggle_hangar2: ToggleHangar(PlayerIndex::player2); break;
      case Key::pattern_a1: BuildPattern(PlayerIndex::player1,0); break;
      case Key::pattern_a2: BuildPattern(PlayerIndex::player2,0); break;
      case Key::pattern_b1: BuildPattern(PlayerIndex::player1,1); break;
      case Key::pattern_b2: BuildPattern(PlayerIndex::player2,1); break;
      case Key::pattern_c1: BuildPattern(PlayerIndex::player1,2); break;
      case Key::pattern_c2: BuildPattern(PlayerIndex::player2,2); break;
      case Key::quit: break; //Cannot handle quit here
      case Key::toggle_cell1: ToggleCell(PlayerIndex::player1); break;
      case Key::toggle_cell2: ToggleCell(PlayerIndex::player2); break;
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
    const int y_before{game.GetPlayer(PlayerIndex::player2).GetY()};
    std::vector<Key> keys = { Key::down2 };
    game.PressKeys(keys);
    const int y_after{game.GetPlayer(PlayerIndex::player2).GetY()};
    assert(y_after == y_before + 1);
  }
}
#endif

void golf::Game::ToggleHangar(const PlayerIndex player_index)
{
  const auto player = this->GetPlayer(player_index);
  const auto iter = std::find_if(
    std::begin(m_hangars),
    std::end(m_hangars),
    [player](const Hangar& hangar) { return hangar.IsIn(player); }
  );
  if (iter == std::end(m_hangars)) return;
  (*iter).ToggleStasis(m_grid);
}
