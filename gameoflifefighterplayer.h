#ifndef GAMEOFLIFEFIGHTERPLAYER_H
#define GAMEOFLIFEFIGHTERPLAYER_H

#include <vector>
#include "gameoflifefighterprefabpattern.h"
#include "gameoflifefighterplayerindex.h"

namespace golf {

struct Player
{
  using PrefabPatterns = std::vector<PrefabPattern>;

  Player(const PlayerIndex player_index, const int x, const int y);

  const PrefabPattern& GetPattern(const int index) const;
  int GetX() const noexcept { return m_x; }
  int GetY() const noexcept { return m_y; }
  void SetX(const int x) noexcept { m_x = x; }
  void SetY(const int y) noexcept { m_y = y; }

  private:
  PlayerIndex m_player_index;
  int m_x;
  int m_y;
  PrefabPatterns m_patterns;

  static PrefabPatterns CreateInitialPatterns();
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERPLAYER_H
