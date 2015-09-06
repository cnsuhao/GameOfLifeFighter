#ifndef GAMEOFLIFEFIGHTERHANGAR_H
#define GAMEOFLIFEFIGHTERHANGAR_H

#include "gameoflifefighterplayerindex.h"
#include "gameoflifefighterhangarstate.h"
#include "gameoflifefightergrid.h"

namespace golf {

struct Grid;
struct Player;

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
    const PlayerIndex player
  );
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

  ///Open or close the Hangar, calls Open or Close
  void SetState(const HangarState state,Grid& global_grid) noexcept;

  ///If the Hangar is opened, the content of the Hangar will be transferred to the Grid
  void Open(Grid& global_grid) noexcept;

  private:
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
