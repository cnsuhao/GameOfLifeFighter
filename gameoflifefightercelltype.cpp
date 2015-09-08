#include "gameoflifefightercelltype.h"

#include <cassert>
#include <stdexcept>

golf::CellType golf::GetOther(const CellType c)
{
  switch (c)
  {
    case CellType::alive: return CellType::empty;
    case CellType::empty: return CellType::alive;
    default:
      assert(!"golf::GetOther: unimplemented CellType");
      throw std::logic_error("golf::GetOther: unimplemented CellType");
  }
}
