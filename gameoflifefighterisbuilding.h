#ifndef GAMEOFLIFEFIGHTERISBUILDING_H
#define GAMEOFLIFEFIGHTERISBUILDING_H

#include <vector>

namespace golf {

enum class IsBuilding { none, player1, player2 };

std::vector<IsBuilding> GetAllIsBuildings() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERISBUILDING_H
