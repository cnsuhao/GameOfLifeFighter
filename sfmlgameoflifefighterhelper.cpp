#include "sfmlgameoflifefighterhelper.h"

#include <cassert>

#include "gameoflifefightertrace.h"

golf::SfmlHelper::SfmlHelper()
{
  #ifndef NDEBUG
  Test();
  #endif
}


#ifndef NDEBUG
void golf::SfmlHelper::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  const bool verbose{false};
  if (verbose) { TRACE("Default-construction of SfmlHelper"); }
  {
    const SfmlHelper q;
  }

}
#endif
