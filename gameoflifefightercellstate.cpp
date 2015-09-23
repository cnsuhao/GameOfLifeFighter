#include "gameoflifefightercellstate.h"

#include <cassert>

golf::CellState::CellState(
    const SelectedBy selected_by,
    const HangarOf hangar_of,
    const HeartOf heart_of,
    const bool is_building,
    const CellType cell_type
) : m_selected_by{selected_by},
    m_hangar_of{hangar_of},
    m_hash{CalculateHash(selected_by,hangar_of,heart_of,is_building,cell_type)},
    m_heart_of{heart_of},
    m_is_building{is_building},
    m_cell_type{cell_type}
{
  #ifndef NDEBUG
  Test();
  #endif
}

int golf::CellState::CalculateHash(
  const SelectedBy selected_by,
  const HangarOf hangar_of,     //0: no-one, 1: player1, 2: player2
  const HeartOf heart_of,      //0: no-one, 1: player1, 2: player2
  const bool is_building,  //Has the player built something on this square?
  const CellType cell_type
) noexcept
{
  int hash = 0;
  hash += (cell_type == CellType::alive ? 1 : 0);
  hash += (is_building ? 2 : 0);
  hash += (heart_of == HeartOf::player1 ? 4 : 0);
  hash += (heart_of == HeartOf::player2 ? 8 : 0);
  hash += (hangar_of == HangarOf::player1 ? 16 : 0);
  hash += (hangar_of == HangarOf::player2 ? 32 : 0);
  hash += (selected_by == SelectedBy::player1 ?  64 : 0);
  hash += (selected_by == SelectedBy::player2 ? 128 : 0);
  return hash;
}

std::vector<golf::CellState> golf::GetAllCellStates()
{
  //Check if all pictures are different
  std::vector<CellState> v;
  for (const SelectedBy selected_by: GetAllSelectedBys())
  {
    for (const HangarOf hangar_of: GetAllHangarOfs())
    {
      for (const HeartOf heart_of: GetAllHeartOfs())
      {
        for (bool is_building: { true, false} )
        {
          for (const CellType cell_type: GetAllCellTypes())
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
  const auto v = GetAllCellStates();
  assert(!v.empty());
  const int n{static_cast<int>(v.size())};
  //Check if all hashes are unique
  for (int i=0; i!=n; ++i)
  {
    const auto lhs = v[i];
    for (int j=0; j!=n; ++j)
    {
      const auto rhs = v[j];
      if (i == j) continue;
      assert(lhs.GetHash() != rhs.GetHash());
    }
  }
}
#endif

bool golf::operator<(const CellState& lhs, const CellState& rhs) noexcept
{
  return lhs.GetHash() < rhs.GetHash();
}
