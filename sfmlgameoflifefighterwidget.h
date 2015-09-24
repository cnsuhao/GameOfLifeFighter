#ifndef QTGAMEOFLIFEWIDGET_H
#define QTGAMEOFLIFEWIDGET_H

#include <set>
#include <map>

#include "gameoflifefightergame.h"
#include "sfmlgameoflifefightersprites.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace golf {

struct SfmlWidget
{
  using SpriteGrid = std::vector<std::vector<sf::Sprite>>;

  explicit SfmlWidget();
  SfmlWidget(const SfmlWidget&) = delete;
  SfmlWidget& operator=(const SfmlWidget&) = delete;

  void Execute();

private:
  Game m_game;
  std::map<sf::Keyboard::Key,Key> m_key_map;
  std::set<Key> m_keys;
  SfmlSprites m_sprite;
  sf::RenderWindow m_window;

  ///The number of ticks the game is running
  int m_tick;

  static std::map<sf::Keyboard::Key,Key> CreateInitialKeyMap() noexcept;

  void Draw();

  void ProcessJoystick();
  void ProcessKeyboard();

  #ifndef NDEBUG
  void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // QTGAMEOFLIFEWIDGET_H
