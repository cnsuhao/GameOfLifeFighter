#ifndef GAMEOFLIFEFIGHTERKEY_H
#define GAMEOFLIFEFIGHTERKEY_H

namespace golf {

enum class Key
{
  up1, down1, left1, right1,          //Player 1 movement
  toggle_hangar1,                     //Player 1 hangar interaction
  toggle_cell1,                       //Player 1 building
  pattern_a1, pattern_b1, pattern_c1, //Player 1 prefab patterns
  up2, down2, left2, right2,          //Player 2 movement
  toggle_cell2,                       //Player 2 building
  toggle_hangar2,                     //Player 2 hangar interaction
  pattern_a2, pattern_b2, pattern_c2, //Player 2 prefab patterns
  quit                                //General program interations
};

} //~namespace golf

#endif // GAMEOFLIFEFIGHTERKEY_H
