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
    const SelectedBy selected_by,
    const HangarOf hangar_of,
    const HeartOf heart_of,
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
