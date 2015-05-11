#ifndef GAME_H
#define GAME_H

#include "grid.h"

struct Game
{
  Game(const int width, const int height);

  ///Obtain the Grid its value
  int GetGrid(const int x, const int y) const;
  int GetHeight() const;
  int GetWidth() const;

  //bool IsHangarRed(const int x, const int y) const;

  void Set(const int x, const int y, const int i);
  void Next();

  private:
  Grid m_grid;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GAME_H
