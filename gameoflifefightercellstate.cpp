#include "gameoflifefightercellstate.h"

#include <cassert>

golf::CellState::CellState(
    const HangarOf hangar_of,
    const HeartOf heart_of,
    const IsBuilding is_building,
    const CellType cell_type
) : m_hangar_of{hangar_of},
    m_hash{CalculateHash(hangar_of,heart_of,is_building,cell_type)},
    m_heart_of{heart_of},
    m_is_building{is_building},
    m_cell_type{cell_type}
{
  #ifndef NDEBUG
  Test();
  #endif
}

int golf::CellState::CalculateHash(
  const HangarOf hangar_of,     //0: no-one, 1: player1, 2: player2
  const HeartOf heart_of,      //0: no-one, 1: player1, 2: player2
  const IsBuilding is_building,  //Has the player built something on this square?
  const CellType cell_type
) noexcept
{
  int hash = 0;
  hash += (cell_type == CellType::alive ? 1 : 0);
  hash += (is_building == IsBuilding::player1 ? 2 : 0);
  hash += (is_building == IsBuilding::player2 ? 4 : 0);
  hash += (heart_of == HeartOf::player1 ? 8 : 0);
  hash += (heart_of == HeartOf::player2 ? 16 : 0);
  hash += (hangar_of == HangarOf::player1 ? 32 : 0);
  hash += (hangar_of == HangarOf::player2 ? 64 : 0);
  return hash;
}

std::vector<golf::CellState> golf::GetAllCellStates()
{
  //Check if all pictures are different
  std::vector<CellState> v;
  for (const HangarOf hangar_of: GetAllHangarOfs())
  {
    for (const HeartOf heart_of: GetAllHeartOfs())
    {
      for (const IsBuilding is_building: GetAllIsBuildings() )
      {
        for (const CellType cell_type: GetAllCellTypes())
        {
          v.push_back(CellState(hangar_of,heart_of,is_building,cell_type));
        }
      }
    }
  }
  return v;
}

void golf::CellState::SetIsBuilding(const PlayerIndex is_building) noexcept
{
  switch (is_building)
  {
    case PlayerIndex::player1: SetIsBuilding(IsBuilding::player1); break;
    case PlayerIndex::player2: SetIsBuilding(IsBuilding::player2); break;
  }
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
