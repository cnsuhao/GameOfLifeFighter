#ifndef HANGAR_H
#define HANGAR_H

#include "player.h"

///Hangar is like a Grid without dynamics
///Grid
struct Hangar
{
  Hangar(
    const int left,
    const int top,
    const int width,
    const int height,
    const Player player
  );
  int GetLeft() const noexcept { return m_left; }
  int GetTop() const noexcept { return m_top; }
  int GetWidth() const noexcept { return m_width; }
  int GetHeight() const noexcept { return m_height; }
  Player GetPlayer() const noexcept { return m_player; }
  bool IsIn(const int x, const int y) const noexcept;

  private:
  const int m_left;
  const int m_top;
  const int m_width;
  const int m_height;
  const Player m_player;
};

#endif // HANGAR_H
