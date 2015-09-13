#ifndef GAMEOFLIFEFIGHTERCELLSTATE_H
#define GAMEOFLIFEFIGHTERCELLSTATE_H

#include <vector>

#include "gameoflifefightercelltype.h"

namespace golf {


struct CellState
{
  CellState(
    const int selected_by,   //0: no-one, 1: player1, 2: player2
    const int hangar_of,     //0: no-one, 1: player1, 2: player2
    const int heart_of,      //0: no-one, 1: player1, 2: player2
    const bool is_building,  //Has the player built something on this square?
    const CellType cell_type
  );

  int GetSelectedBy() const noexcept { return m_selected_by; }
  int GetHangarOf() const noexcept { return m_hangar_of; }
  int GetHeartOf() const noexcept { return m_heart_of; }
  bool GetIsBuilding() const noexcept { return m_is_building; }
  CellType GetCellType() const noexcept { return m_cell_type; }

  void SetSelectedBy(const int selected_by);
  void SetHangarOf(const int hangar_of);
  void SetHeartOf(const int heart_of);
  void SetIsBuilding(const bool is_building) noexcept { m_is_building = is_building; }
  void SetCellType(const CellType cell_type) noexcept { m_cell_type = cell_type; }

  private:
  int m_selected_by;
  int m_hangar_of;
  int m_heart_of;
  bool m_is_building;
  CellType m_cell_type;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

std::vector<CellState> GetAllCellStates();

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERCELLSTATE_H
