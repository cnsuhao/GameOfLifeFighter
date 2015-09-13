#include "gameoflifefightercellstate.h"

#include <cassert>

golf::CellState::CellState(
    const int selected_by,
    const int hangar_of,
    const int heart_of,
    const bool is_building,
    const CellType cell_type
) : m_selected_by{selected_by},
    m_hangar_of{hangar_of},
    m_heart_of{heart_of},
    m_is_building{is_building},
    m_cell_type{cell_type}
{
  #ifndef NDEBUG
  Test();
  #endif

  assert(selected_by >= 0 && selected_by <= 2 && "0: no-one, 1: player1, 2: player2");
  assert(hangar_of >= 0 && hangar_of <= 2 && "0: no-one, 1: player1, 2: player2");
  assert(heart_of >= 0 && heart_of <= 2 && "0: no-one, 1: player1, 2: player2");

}

void golf::CellState::SetSelectedBy(const int selected_by)
{
  assert(selected_by >= 0 && selected_by <= 2 && "0: no-one, 1: player1, 2: player2");
  m_selected_by = selected_by;
}

void golf::CellState::SetHangarOf(const int hangar_of)
{
  assert(hangar_of >= 0 && hangar_of <= 2 && "0: no-one, 1: player1, 2: player2");
  m_hangar_of = hangar_of;

}

void golf::CellState::SetHeartOf(const int heart_of)
{
  assert(heart_of >= 0 && heart_of <= 2 && "0: no-one, 1: player1, 2: player2");
  m_heart_of = heart_of;
}


std::vector<golf::CellState> golf::GetAllCellStates()
{
  //Check if all pictures are different
  std::vector<CellState> v;
  for (int selected_by = 0; selected_by <= 2; ++selected_by)
  {
    for (int hangar_of = 0; hangar_of <= 2; ++hangar_of)
    {
      for (int heart_of = 0; heart_of <= 2; ++heart_of)
      {
        for (bool is_building: { true, false} )
        {
          for (CellType cell_type: { CellType::empty, CellType::alive} )
          {
            v.push_back(CellState(selected_by,hangar_of,heart_of,is_building,cell_type));
          }
        }
      }
    }
  }

  return v;
}


#ifndef NDEBUG
void golf::CellState::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  assert(!GetAllCellStates().empty());
}
#endif
