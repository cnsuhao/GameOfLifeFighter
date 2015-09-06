#ifndef GAMEOFLIFEFIGHTERGRID_H
#define GAMEOFLIFEFIGHTERGRID_H

#include <vector>

namespace golf {

struct Grid
{
  Grid(const int width, const int height);
  int Get(const int x, const int y) const;
  int GetHeight() const noexcept;
  int GetWidth() const;
  int Return_active_neighbours(const int x, const int y);
  void Set(const int x, const int y, const int i);
  void Next();

  private:
  //Y-X ordered
  std::vector<std::vector<int> > m_grid;
  void Create_glider();
  void Create_block();
  void Create_blinker();

  #ifndef NDEBUG
    static void Test() noexcept;

  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGRID_H
