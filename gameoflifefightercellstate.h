#ifndef GAMEOFLIFEFIGHTERCELLSTATE_H
#define GAMEOFLIFEFIGHTERCELLSTATE_H

#include <vector>

#include "gameoflifefightercelltype.h"
#include "gameoflifefighterisbuilding.h"
#include "gameoflifefighterplayerindex.h"
#include "gameoflifefighterhangarof.h"
#include "gameoflifefighterheartof.h"


namespace golf {


struct CellState
{
  //Remove if the cursor is present (which was confusingly called 'IsSelected')
  CellState(
    const HangarOf hangar_of,
    const HeartOf heart_of,
    const IsBuilding is_building,
    const CellType cell_type
  );

  ///Get the hashed value of the CellState, which is unique for each CellState
  int GetHash() const noexcept { return m_hash; }

  HangarOf GetHangarOf() const noexcept { return m_hangar_of; }
  HeartOf GetHeartOf() const noexcept { return m_heart_of; }
  IsBuilding GetIsBuilding() const noexcept { return m_is_building; }
  CellType GetCellType() const noexcept { return m_cell_type; }

  void SetHangarOf(const HangarOf hangar_of) noexcept { m_hangar_of = hangar_of; }
  void SetHeartOf(const HeartOf heart_of) noexcept { m_heart_of = heart_of; }
  void SetIsBuilding(const IsBuilding is_building) noexcept { m_is_building = is_building; }
  void SetIsBuilding(const PlayerIndex is_building) noexcept;

  void SetCellType(const CellType cell_type) noexcept { m_cell_type = cell_type; }

  private:
  HangarOf m_hangar_of;
  int m_hash;
  HeartOf m_heart_of;
  IsBuilding m_is_building;
  CellType m_cell_type;

  static int CalculateHash(
    const HangarOf hangar_of,     //0: no-one, 1: player1, 2: player2
    const HeartOf heart_of,      //0: no-one, 1: player1, 2: player2
    const IsBuilding is_building,  //Has the player built something on this square?
    const CellType cell_type
  ) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::vector<CellState> GetAllCellStates();

bool operator<(const CellState& lhs, const CellState& rhs) noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERCELLSTATE_H
