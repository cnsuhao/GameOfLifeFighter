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
  using Hangars = std::vector<Hangar>;
  using Players = std::vector<Player>;

  Game();

  ///Obtain the Grid its value
  CellType GetGrid(const int x, const int y) const;
  const Hangars& GetHangars() const noexcept { return m_hangars; }
  int GetHeight() const noexcept { return 60; }

  const Players& GetPlayers() const noexcept { return m_players; }

  int GetWidth() const noexcept { return 200; }
  void PressKeys(const std::set<Key>& keys);
  void Set(const int x, const int y, const CellType cell);
  void Next();

  private:
  Grid m_grid;
  Hangars m_hangars;
  Players m_players;

  static Hangars CreateInitialHangars(const int width, const int height);
  static Players CreateInitialPlayers(const int width, const int height);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGAME_H
