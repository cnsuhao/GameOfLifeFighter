#ifndef GAMEOFLIFEFIGHTERGAME_H
#define GAMEOFLIFEFIGHTERGAME_H

#include <set>

#include "gameoflifefighterkey.h"
#include "gameoflifefighterplayer.h"
#include "gameoflifefightergrid.h"
#include "gameoflifefighterhangar.h"
#include "gameoflifefighterheart.h"
#include "gameoflifefightergamestate.h"
#include "gameoflifefightergametype.h"

namespace golf {

///Game contains the game logic
///The most important member functions are:
///* PressKeys: process all key presses
///* Next: go to the next game state
struct Game
{
  using Hangars = std::vector<Hangar>;
  using Hearts = std::vector<Heart>;
  using Players = std::vector<Player>;
  using BitFlagGrid = std::vector<std::vector<int>>;

  Game(const GameType game_type = GameType::free_fight);

  bool CanBuild(const PlayerIndex player_index) const noexcept;
  bool CanBuildHere(const PlayerIndex player_index, const int x, const int y) const noexcept;
  bool CanBuildPattern(const PlayerIndex player_index, const int pattern_index) const noexcept;

  ///Obtain the cell type from the global Grid, excluding the Hangars
  CellType GetCell(const int x, const int y) const;

  ///Obtain the complete y-x-ordered global Grid as bit flags
  /// 00: empty, not hangar
  /// 01: alive, not hangar
  /// 10: empty, in hangar
  /// 11: alive, in hangar
  BitFlagGrid GetBitFlagGrid() const;

  ///Get a Player his/her i'th pre-fab pattern
  const PrefabPattern& GetPattern(const PlayerIndex player_index, const int pattern_index) const;

  const Hangars& GetHangars() const noexcept { return m_hangars; }

  const Hearts& GetHearts() const noexcept { return m_hearts; }

  static int GetHeight() noexcept { return 60; }

  const Player& GetPlayer(const PlayerIndex player_index) const noexcept;

  ///Is there a Hangar at this coordinat?
  ///Returns nullptr if not
  ///Returns a read-only Hanger if there is one
  const Hangar * FindHangar(const int x, const int y) const noexcept;
        Hangar * FindHangar(const int x, const int y)       noexcept;

  ///Is there a Heart at this coordinat?
  ///Returns nullptr if not
  ///Returns a read-only Heart if there is one
  const Heart * FindHeart(const int x, const int y) const noexcept;
        Heart * FindHeart(const int x, const int y)       noexcept;


  GameState GetGameState() const noexcept { return m_game_state; }

  ///The width of the global GOL grid
  static int GetWidth() noexcept { return 200; }

  ///Is this coordinat a hangar?
  bool IsHangar(const int x, const int y) const noexcept;

  ///Is this coordinat a heart?
  bool IsHeart(const int x, const int y) const noexcept;

  ///Press all keys once
  //Players can move throughout the complete field, but only build on their half of it
  void PressKeys(const std::set<Key>& keys);

  ///Set the content of a global grid cell
  ///When the player is building in a hangar, use Hangar::Set
  void Set(const int x, const int y, const CellType cell);

  ///Go to the next game state, make the game tick
  void Next();

  private:
  GameState m_game_state;
  GameType m_game_type;
  Grid m_grid;
  Hangars m_hangars;
  Hearts m_hearts;
  Players m_players;

  ///Let a player try to close a hangar
  void CloseHangar(const PlayerIndex player_index);

  static Hangars CreateInitialHangars(const GameType game_type);
  static Hearts CreateInitialHearts(const GameType game_type);
  static Players CreateInitialPlayers(const GameType game_type);

  ///Let a player try to open a hangar
  void OpenHangar(const PlayerIndex player_index);

  ///Let a player try to build/remove
  void ToggleCell(const PlayerIndex player_index);

  ///Let a player try to open a hangar
  void ToggleHangar(const PlayerIndex player_index);

  ///Let a player try to build a patterm
  void BuildPattern(const PlayerIndex player_index, const int pattern_index);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGAME_H
