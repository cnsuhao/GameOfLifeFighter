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
  using BitFlagGrid = std::vector<std::vector<int>>;

  Game();

  ///Obtain the cell type from the global Grid, excluding the Hangars
  CellType GetCell(const int x, const int y) const;

  ///Obtain the complete y-x-ordered global Grid as bit flags
  /// 00: empty, not hangar
  /// 01: alive, not hangar
  /// 10: empty, in hangar
  /// 11: alive, in hangar
  BitFlagGrid GetBitFlagGrid() const;

  const Hangars& GetHangars() const noexcept { return m_hangars; }

  int GetHeight() const noexcept { return 60; }

  const Players& GetPlayers() const noexcept { return m_players; }

  ///Is there a Hangar at this coordinat?
  ///Returns nullptr if not
  ///Returns a read-only Hanger if there is one
  const Hangar * FindHangar(const int x, const int y) const noexcept;
        Hangar * FindHangar(const int x, const int y)       noexcept;

  ///The width of the global GOL grid
  int GetWidth() const noexcept { return 200; }

  ///Is this coordinat a hangar?
  bool IsHangar(const int x, const int y) const noexcept;

  ///Press all keys once
  void PressKeys(const std::set<Key>& keys);

  ///Set the content of a global grid cell
  ///When the player is building in a hangar, use Hangar::Set
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
