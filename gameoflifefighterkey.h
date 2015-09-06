#ifndef GAMEOFLIFEFIGHTERKEY_H
#define GAMEOFLIFEFIGHTERKEY_H

namespace golf {

enum class Key
{
  up1, down1, left1, right1,          //Player 1 movement
  open_hangar1, close_hangar1,        //Player 1 hangar interaction
  set_high1, set_low1,                //Player 1 building
  pattern_a1, pattern_b1, pattern_c1, //Player 1 prefab patterns
  up2, down2, left2, right2,          //Player 2 movement
  set_high2, set_low2,                //Player 2 hangar interaction
  open_hangar2, close_hangar2,        //Player 2 building
  pattern_a2, pattern_b2, pattern_c2, //Player 2 prefab patterns
  quit                                //General program interations
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERKEY_H
