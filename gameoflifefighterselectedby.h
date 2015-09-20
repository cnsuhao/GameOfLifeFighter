#ifndef GAMEOFLIFEFIGHTERSELECTEDBY_H
#define GAMEOFLIFEFIGHTERSELECTEDBY_H

#include <vector>

namespace golf {

enum class SelectedBy { none, player1, player2 };

std::vector<SelectedBy> GetAllSelectedBys() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERSELECTEDBY_H
