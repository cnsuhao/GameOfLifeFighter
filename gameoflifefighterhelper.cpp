#include "gameoflifefighterhelper.h"

#include <cassert>
#include <fstream>

golf::Helper::Helper()
{
  #ifndef NDEBUG
  Test();
  #endif
}

bool golf::Helper::IsRegularFile(const std::string& filename) const noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

#ifndef NDEBUG
void golf::Helper::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  Helper h;
  {
    std::remove("tmp.txt");

    //Create a regular file
    assert(!h.IsRegularFile("tmp.txt"));
    {
      std::fstream f;
      f.open("tmp.txt",std::ios::out);
      f << "TEMP TEXT";
      f.close();
    }
    assert(h.IsRegularFile("tmp.txt"));

    std::remove("tmp.txt");

    assert(!h.IsRegularFile("tmp.txt"));
  }
}
#endif



