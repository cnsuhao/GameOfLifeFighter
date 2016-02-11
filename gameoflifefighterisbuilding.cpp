#include "gameoflifefighterisbuilding.h"

std::vector<golf::IsBuilding> golf::GetAllIsBuildings() noexcept
{
  std::vector<IsBuilding> v;
  v.push_back(IsBuilding::none);
  v.push_back(IsBuilding::player1);
  v.push_back(IsBuilding::player2);
  return v;
}
