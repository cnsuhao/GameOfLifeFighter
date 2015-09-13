#ifndef GAMEOFLIFEFIGHTERHANGAR_H
#define GAMEOFLIFEFIGHTERHANGAR_H

#include "gameoflifefighterplayerindex.h"
#include "gameoflifefighterhangarstate.h"
#include "gameoflifefightergrid.h"

namespace golf {

struct Grid;
struct Player;
struct PrefabPattern;

///Hangar is like a Grid without dynamics; it can be in stasis (Closed) or active (Open)
///A Hangar has a Grid of its own, where the player can build cell compositions
struct Hangar
{
  ///A Hangar has a position, size and a Player that can use it
  Hangar(
    const int left,
    const int top,
    const int width,
    const int height,
    const PlayerIndex player,
    const bool do_transfer_up
  );

  void BuildPattern(
    const int cursor_x, const int cursor_y,
    const PrefabPattern& pattern
  );

  CellType GetCell(const int x, const int y) const { return m_grid.Get(x,y); }
  int GetLeft() const noexcept { return m_left; }
  int GetTop() const noexcept { return m_top; }
  int GetWidth() const noexcept { return m_width; }
  int GetHeight() const noexcept { return m_height; }
  PlayerIndex GetPlayerIndex() const noexcept { return m_player_index; }

  HangarState GetState() const noexcept { return m_state; }

  ///If the Hangar is closed, the content of the global Grid will be the initial content of the Hangar and put into statis
  void Close(Grid& global_grid) noexcept;

  ///Is the coordinat within the Hangar?
  bool IsIn(const int x, const int y) const noexcept;

  ///Is the Player within the Hangar?
  bool IsIn(const Player& player) const noexcept;

  void SetCell(const int x, const int y, const CellType cell) { m_grid.Set(x,y,cell); }

  ///Open or close the Hangar, calls Open or Close
  void SetState(const HangarState state,Grid& global_grid) noexcept;

  ///If the Hangar is opened, the content of the Hangar will be transferred to the Grid
  void Open(Grid& global_grid) noexcept;

  ///Build/remove a cell
  void ToggleCell(const int x, const int y);

  ///Open or close the hangar
  void ToggleStasis(Grid& global_grid) noexcept;

  private:
  ///When the Hangar goes to stasis, will it pick up the cells from the main grid?
  const bool m_do_transfer_up;
  Grid m_grid;
  const int m_height;
  const int m_left;
  const PlayerIndex m_player_index;
  HangarState m_state;
  const int m_top;

  const int m_width;
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHANGAR_H
