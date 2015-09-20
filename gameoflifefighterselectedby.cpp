#include "gameoflifefighterselectedby.h"

std::vector<golf::SelectedBy> golf::GetAllSelectedBys() noexcept
{
  std::vector<SelectedBy> v;
  v.push_back(SelectedBy::none);
  v.push_back(SelectedBy::player1);
  v.push_back(SelectedBy::player2);
  return v;
}
