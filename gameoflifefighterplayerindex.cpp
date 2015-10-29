#include "gameoflifefighterplayerindex.h"

#include <cassert>

std::vector<golf::PlayerIndex> golf::GetAllPlayerIndices() noexcept
{
  std::vector<PlayerIndex> v;
  v.push_back(PlayerIndex::player1);
  v.push_back(PlayerIndex::player2);
  return v;
}


