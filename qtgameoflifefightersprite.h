#ifndef QTGAMEOFLIFEFIGHTERSPRITE_H
#define QTGAMEOFLIFEFIGHTERSPRITE_H

#include <QImage>
#include "gameoflifefightercelltype.h"
#include "gameoflifefightercellstate.h"

namespace golf {

struct QtSprite
{
  QtSprite();

  QImage Create(const CellState& state) const noexcept;

  QImage Create(
    const int selected_by,   //0: no-one, 1: player1, 2: player2
    const int hangar_of,     //0: no-one, 1: player1, 2: player2
    const int heart_of,      //0: no-one, 1: player1, 2: player2
    const bool is_building,  //Has the player built something on this square?
    const CellType cell_type 
  ) const;

  int GetHeight() const noexcept { return 6; }
  int GetWidth() const noexcept { return 6; }

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};

} //~namespace golf

#endif // QTGAMEOFLIFEFIGHTERSPRITE_H
