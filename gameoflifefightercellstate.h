#ifndef GAMEOFLIFEFIGHTERCELLSTATE_H
#define GAMEOFLIFEFIGHTERCELLSTATE_H

#include <vector>

#include "gameoflifefightercelltype.h"
#include "gameoflifefighterselectedby.h"
#include "gameoflifefighterhangarof.h"
#include "gameoflifefighterheartof.h"


namespace golf {


struct CellState
{
  CellState(
    const SelectedBy selected_by,
    const HangarOf hangar_of,
    const HeartOf heart_of,
    const bool is_building,  //Has the player built something on this square?
    const CellType cell_type
  );

  ///Get the hashed value of the CellState, which is unique for each CellState
  int GetHash() const noexcept { return m_hash; }

  SelectedBy GetSelectedBy() const noexcept { return m_selected_by; }
  HangarOf GetHangarOf() const noexcept { return m_hangar_of; }
  HeartOf GetHeartOf() const noexcept { return m_heart_of; }
  bool GetIsBuilding() const noexcept { return m_is_building; }
  CellType GetCellType() const noexcept { return m_cell_type; }

  void SetSelectedBy(const SelectedBy selected_by) noexcept { m_selected_by = selected_by; }
  void SetHangarOf(const HangarOf hangar_of) noexcept { m_hangar_of = hangar_of; }
  void SetHeartOf(const HeartOf heart_of) noexcept { m_heart_of = heart_of; }
  void SetIsBuilding(const bool is_building) noexcept { m_is_building = is_building; }
  void SetCellType(const CellType cell_type) noexcept { m_cell_type = cell_type; }

  private:
  SelectedBy m_selected_by;
  HangarOf m_hangar_of;
  int m_hash;
  HeartOf m_heart_of;
  bool m_is_building;
  CellType m_cell_type;

  static int CalculateHash(
    const SelectedBy selected_by,
    const HangarOf hangar_of,     //0: no-one, 1: player1, 2: player2
    const HeartOf heart_of,      //0: no-one, 1: player1, 2: player2
    const bool is_building,  //Has the player built something on this square?
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
