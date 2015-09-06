#ifndef GAMEOFLIFEFIGHTERPLAYER_H
#define GAMEOFLIFEFIGHTERPLAYER_H

namespace golf {

struct Player
{
  Player(const int x, const int y);
  int GetX() const noexcept { return m_x; }
  int GetY() const noexcept { return m_y; }
  void SetX(const int x) noexcept { m_x = x; }
  void SetY(const int y) noexcept { m_y = y; }

  private:
  int m_x;
  int m_y;
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERPLAYER_H
