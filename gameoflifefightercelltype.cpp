#include "gameoflifefightercelltype.h"

#include <cassert>
#include <stdexcept>

std::vector<golf::CellType> golf::GetAllCellTypes() noexcept
{
  std::vector<CellType> v;
  v.push_back(CellType::empty);
  v.push_back(CellType::alive);
  return v;
}

golf::CellType golf::GetOther(const CellType c) noexcept
{
  switch (c)
  {
    case CellType::alive: return CellType::empty;
    case CellType::empty: return CellType::alive;
    default:
      assert(!"golf::GetOther: unimplemented CellType");
      return CellType::alive;
      //    ssssssssssssssssswthrow std::logic_error("golf::GetOther: unimplemented CellType");
  }
}
