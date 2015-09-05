#ifndef GAMEOFLIFEFIGHTERGAME_H
#define GAMEOFLIFEFIGHTERGAME_H

#include <set>

#include "gameoflifefighterkey.h"
#include "gameoflifefighterplayer.h"
#include "gameoflifefightergrid.h"
#include "gameoflifefighterhangar.h"

namespace golf {

struct Game
{
  Game(const int width, const int height);

  ///Obtain the Grid its value
  int GetGrid(const int x, const int y) const;
  const std::vector<Hangar>& GetHangars() const noexcept { return m_hangars; }
  int GetHeight() const noexcept { return 60; }
  int GetWidth() const noexcept { return 200; }
  void PressKeys(const std::set<Key>& keys);
  void Set(const int x, const int y, const int i);
  void Next();

  private:
  Grid m_grid;
  std::vector<Hangar> m_hangars;
  std::vector<Player> m_players;

  static std::vector<Hangar> CreateInitialHangars(const int width, const int height);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGAME_H
