#ifndef GAMEOFLIFEFIGHTERCELL_H
#define GAMEOFLIFEFIGHTERCELL_H

namespace golf {

enum class CellType
{
  empty = 0,
  alive = 1
};

CellType GetOther(const CellType c);

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERCELL_H
