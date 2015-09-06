#ifndef GAMEOFLIFEFIGHTERPLAYERINDEX_H
#define GAMEOFLIFEFIGHTERPLAYERINDEX_H

#include <QColor>

namespace golf {

enum class PlayerIndex { player1, player2 };

QColor ToColor(const PlayerIndex player) noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERPLAYERINDEX_H
