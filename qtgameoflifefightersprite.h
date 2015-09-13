#ifndef QTGAMEOFLIFEFIGHTERSPRITE_H
#define QTGAMEOFLIFEFIGHTERSPRITE_H

#include "gameoflifefightercelltype.h"
#include <QImage>

namespace golf {

struct QtSprite
{
  QtSprite();
  QImage Create(
    const int width,
    const int height,
    const int selected_by,   //0: no-one, 1: player1, 2: player2
    const int hangar_of,     //0: no-one, 1: player1, 2: player2
    const int heart_of,      //0: no-one, 1: player1, 2: player2
    const bool is_building,  //Has the player built something on this square?
    const CellType cell_type 
  ) const;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

};

} //~namespace golf

#endif // QTGAMEOFLIFEFIGHTERSPRITE_H
