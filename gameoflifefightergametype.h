#ifndef GAMEOFLIFEFIGHTERGAMETYPE_H
#define GAMEOFLIFEFIGHTERGAMETYPE_H

namespace golf {

///The game type:
/// * classic: four hangars that put part of the grid in statis
/// * free_fight: can build everywhere, patterns can be released
enum class GameType
{
  classic,
  free_fight
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERGAMETYPE_H
