#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "grid.h"
#include "hangar.h"

struct Game
{
  Game(const int width, const int height);

  ///Obtain the Grid its value
  int GetGrid(const int x, const int y) const;
  const std::vector<Hangar>& GetHangars() const noexcept { return m_hangars; }
  int GetHeight() const;
  int GetWidth() const;

  void Set(const int x, const int y, const int i);
  void Next();

  private:
  Grid m_grid;
  std::vector<Hangar> m_hangars;


  static std::vector<Hangar> CreateInitialHangars(const int width, const int height);

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

#endif // GAME_H
