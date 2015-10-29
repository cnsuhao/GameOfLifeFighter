#include "gameoflifefighterheartof.h"

std::vector<golf::HeartOf> golf::GetAllHeartOfs() noexcept
{
  std::vector<HeartOf> v;
  v.push_back(HeartOf::none);
  v.push_back(HeartOf::player1);
  v.push_back(HeartOf::player2);
  return v;
}
