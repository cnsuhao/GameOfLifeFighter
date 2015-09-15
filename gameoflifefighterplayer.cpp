#include "gameoflifefighterplayer.h"

#include <cassert>

golf::Player::Player(const PlayerIndex player_index, const int x, const int y)
  : m_player_index{player_index},
    m_x{x},
    m_y{y},
    m_patterns{CreateInitialPatterns()}
{
  if (m_player_index == PlayerIndex::player2)
  {
    for (auto& pattern: m_patterns) { pattern.FlipHorizontal(); }
  }
}

golf::Player::PrefabPatterns golf::Player::CreateInitialPatterns()
{
  PrefabPatterns v;
  v.push_back(CreateGlider());
  v.push_back(CreateLightWeightSpaceShip());
  v.push_back(CreateGrower());
  return v;
}

const golf::PrefabPattern& golf::Player::GetPattern(const int index) const
{
  assert(index >= 0);
  assert(index < static_cast<int>(m_patterns.size()));
  return m_patterns[index];
}
