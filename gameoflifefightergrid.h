#ifndef GAMEOFLIFEFIGHTERGRID_H
#define GAMEOFLIFEFIGHTERGRID_H

#include <vector>
#include "gameoflifefightercelltype.h"

namespace golf {

struct Grid
{
  Grid(const int width, const int height);
  CellType Get(const int x, const int y) const;
  int GetHeight() const noexcept;
  int GetWidth() const;
  int Return_active_neighbours(const int x, const int y);
  void Set(const int x, const int y, const CellType cell);
  void Next();

  private:
  //Y-X ordered
<<<<<<< HEAD
  std::vector<std::vector<int> > m_grid;
=======
  std::vector<std::vector<CellType>> m_grid;
>>>>>>> c86aee26e6c75a236b7e7dc95cb047e60a469460
  void Create_glider();
  void Create_block();
  void Create_blinker();

  #ifndef NDEBUG
    static void Test() noexcept;

  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGRID_H
