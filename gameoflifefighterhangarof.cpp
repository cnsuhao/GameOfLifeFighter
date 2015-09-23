#include "gameoflifefighterhangarof.h"

std::vector<golf::HangarOf> golf::GetAllHangarOfs() noexcept
{
  std::vector<HangarOf> v;
  v.push_back(HangarOf::none);
  v.push_back(HangarOf::player1);
  v.push_back(HangarOf::player2);
  return v;
}
