#ifndef GAMEOFLIFEFIGHTERHEARTOF_H
#define GAMEOFLIFEFIGHTERHEARTOF_H

#include <vector>

namespace golf {

enum class HeartOf { none, player1, player2 };

std::vector<HeartOf> GetAllHeartOfs() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHEARTOF_H
