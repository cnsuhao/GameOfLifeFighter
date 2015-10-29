#include "qtgameoflifefighterplayerindex.h"

#include <cassert>

QColor golf::ToColor(const PlayerIndex player) noexcept
{
  switch (player)
  {
    case PlayerIndex::player1: return qRgb(255,0,0);
    case PlayerIndex::player2: return qRgb(0,0,255);
  }
  assert(!"Should not get here");
  return qRgb(0,0,0);
}

