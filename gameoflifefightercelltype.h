#ifndef GAMEOFLIFEFIGHTERCELL_H
#define GAMEOFLIFEFIGHTERCELL_H

#include <vector>

namespace golf {

enum CellType
{
  empty = 0,
  alive = 1
};

CellType GetOther(const CellType c) noexcept;

std::vector<CellType> GetAllCellTypes() noexcept;

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERCELL_H
