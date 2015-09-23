#ifndef GAMEOFLIFEFIGHTERHELPER_H
#define GAMEOFLIFEFIGHTERHELPER_H

#include <string>

namespace golf {

struct Helper
{
  Helper();

  bool IsRegularFile(const std::string& filename) const noexcept;

  private:
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERHELPER_H
