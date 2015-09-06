#ifndef GAMEOFLIFEFIGHTERHANGAR_H
#define GAMEOFLIFEFIGHTERHANGAR_H

#include "gameoflifefighterplayerindex.h"

namespace golf {

///Hangar is like a Grid without dynamics
///Grid
struct Hangar
{
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
  bool IsIn(const int x, const int y) const noexcept;

  private:
  const int m_left;
  const int m_top;
  const int m_width;
  const int m_height;
  const PlayerIndex m_player_index;
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHANGAR_H
