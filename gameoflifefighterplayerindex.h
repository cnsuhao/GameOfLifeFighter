#ifndef GAMEOFLIFEFIGHTERPLAYERINDEX_H
#define GAMEOFLIFEFIGHTERPLAYERINDEX_H

#include <vector>

namespace golf {

enum class PlayerIndex { player1, player2 };

std::vector<PlayerIndex> GetAllPlayerIndices() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERPLAYERINDEX_H
