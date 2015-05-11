#ifndef HANGAR_H
#define HANGAR_H

#include "player.h"

struct Hangar
{
  Hangar(
    const int left, const int right,
    const int width, const int height,
    const Player player);
};

#endif // HANGAR_H
