#ifndef GAMEOFLIFEFIGHTERHEART_H
#define GAMEOFLIFEFIGHTERHEART_H

#include "gameoflifefighterplayerindex.h"

namespace golf {

struct Grid;

///The heart of a player his/her base
struct Heart
{
  Heart(
    const int left,
    const int top,
    const int width,
    const int height,
    const PlayerIndex player_index
  );

  int GetLeft() const noexcept { return m_left; }
  int GetTop() const noexcept { return m_top; }
  int GetWidth() const noexcept { return m_width; }
  int GetHeight() const noexcept { return m_height; }
  PlayerIndex GetPlayerIndex() const noexcept { return m_player_index; }


  ///Is this coordinat within the Heart?
  bool IsIn(const int x, const int y) const noexcept;

  ///Is the heart empty, thus, has a player lost?
  bool IsEmpty(const Grid& grid) const;

  private:

  const int m_left;
  const int m_top;
  const int m_width;
  const int m_height;
  const PlayerIndex m_player_index;

};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHEART_H
