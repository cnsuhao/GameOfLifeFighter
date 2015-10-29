#ifndef GAMEOFLIFEFIGHTERHANGAROF_H
#define GAMEOFLIFEFIGHTERHANGAROF_H

#include <vector>

namespace golf {

enum class HangarOf { none, player1, player2 };

std::vector<HangarOf> GetAllHangarOfs() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHANGAROF_H
