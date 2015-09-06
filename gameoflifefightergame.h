#ifndef GAMEOFLIFEFIGHTERGAME_H
#define GAMEOFLIFEFIGHTERGAME_H

#include <set>

#include "gameoflifefighterkey.h"
#include "gameoflifefighterplayer.h"
#include "gameoflifefightergrid.h"
#include "gameoflifefighterhangar.h"

namespace golf {

///Game contains the game logic
///The most important member functions are:
///* PressKeys: process all key presses
///* Next: go to the next game state
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

  ///Press all keys once
  void PressKeys(const std::set<Key>& keys);

  ///Change the content of a Cell
  void Set(const int x, const int y, const CellType cell);

  ///Go to the next game state, make the game tick
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
