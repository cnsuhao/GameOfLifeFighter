#include "sfmlgameoflifefighterwidget.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "sfmlgameoflifefighterplayerindex.h"
#include "sfmlgameoflifefighterhelper.h"
#include "sfmlgameoflifefightersprites.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefighterplayerindex.h"

golf::SfmlWidget::SfmlWidget()
  : m_game{},
    m_key_map{CreateInitialKeyMap()},
    m_keys{},
    m_sprite{},
    m_window(
      sf::VideoMode(
        Game().GetWidth() * SfmlSprites().GetWidth(),
        Game().GetHeight() * SfmlSprites().GetHeight()
      ),
      "Game Of Life Fighter",
      sf::Style::Titlebar | sf::Style::Close
    ),
    m_background(CreateBackground()),
    m_hangars(CreateHangars()),
    m_tick{0}
{
  #ifndef NDEBUG
  Test();
  #endif
  sf::Music music;
  const bool can_open{music.openFromFile("../GameOfLifeFighter/Resources/Music/GameOfDeath.ogg")};
  assert(can_open);
  music.play();


}

sf::Texture golf::SfmlWidget::CreateBackground()
{
  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};
  const int ww{Game().GetWidth() * SfmlSprites().GetWidth()};
  const int wh{Game().GetHeight() * SfmlSprites().GetHeight()};
  const int sw{m_sprite.GetWidth()};
  const int sh{m_sprite.GetHeight()};
  sf::Sprite bgSprite(m_sprite.Get(CellType::empty), sf::IntRect(0,0,6,6));

  sf::RenderTexture rt;
  if (!rt.create(ww, wh)) {
    throw std::runtime_error("cannot create background texture");
  }

  rt.clear();

  for (int y=0; y!=grid_rows; ++y) {
    for (int x=0; x!=grid_cols; ++x) {
      bgSprite.setPosition(x * sw, y * sh);
      rt.draw(bgSprite);
    }
  }

  rt.display();
  sf::Texture texture = rt.getTexture();

  return texture;
}

sf::Texture golf::SfmlWidget::CreateHangars()
{
  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};
  const int ww{Game().GetWidth() * SfmlSprites().GetWidth()};
  const int wh{Game().GetHeight() * SfmlSprites().GetHeight()};
  const int sw{m_sprite.GetWidth()};
  const int sh{m_sprite.GetHeight()};
  const auto grid = m_game.GetCellStateGrid();

  sf::RenderTexture rt;
  if (!rt.create(ww, wh)) {
    throw std::runtime_error("cannot create background texture");
  }

  rt.clear(sf::Color(0, 0, 0, 0));

  for (int y=0; y!=grid_rows; ++y) {
    for (int x=0; x!=grid_cols; ++x) {
      const auto& s = grid[y][x];
      sf::Sprite sprite(m_sprite.Get(s.GetHangarOf()), sf::IntRect(0,0,6,6));
      sprite.setPosition(x * sw, y * sh);
      rt.draw(sprite);
    }
  }

  rt.display();
  sf::Texture texture = rt.getTexture();

  return texture;
}

void golf::SfmlWidget::AddKey(const Key key)
{
  if (std::count(std::begin(m_keys),std::end(m_keys),key) == 0)
  {
    m_keys.push_back(key);
  }
  assert(std::count(std::begin(m_keys),std::end(m_keys),key) == 1);
}

std::map<sf::Keyboard::Key,golf::Key> golf::SfmlWidget::CreateInitialKeyMap() noexcept
{
  std::map<sf::Keyboard::Key,Key> m;
  m.insert( { sf::Keyboard::Q, Key::toggle_cell1}  );
  m.insert( { sf::Keyboard::W, Key::up1}  );
  m.insert( { sf::Keyboard::E, Key::toggle_hangar1}  );

  m.insert( { sf::Keyboard::A, Key::left1}  );
  m.insert( { sf::Keyboard::S, Key::down1}  );
  m.insert( { sf::Keyboard::D, Key::right1}  );

  m.insert( { sf::Keyboard::Z, Key::pattern_a1}  );
  m.insert( { sf::Keyboard::X, Key::pattern_b1}  );
  m.insert( { sf::Keyboard::C, Key::pattern_c1}  );

  m.insert( { sf::Keyboard::U, Key::toggle_cell2}  );
  m.insert( { sf::Keyboard::I, Key::up2}  );
  m.insert( { sf::Keyboard::O, Key::toggle_hangar2}  );

  m.insert( { sf::Keyboard::J, Key::left2}  );
  m.insert( { sf::Keyboard::K, Key::down2}  );
  m.insert( { sf::Keyboard::L, Key::right2}  );

  m.insert( { sf::Keyboard::M     , Key::pattern_a2}  );
  m.insert( { sf::Keyboard::Comma , Key::pattern_b2}  );
  m.insert( { sf::Keyboard::Period, Key::pattern_c2}  );

  return m;
}

void golf::SfmlWidget::Draw()
{
  sf::Clock clock;

  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};

  m_window.clear();

  // Draw background.
  sf::Sprite bgsprite(m_background);
  m_window.draw(bgsprite);

  const auto grid = m_game.GetCellStateGrid();

  //Draw cells
  for (int y=0; y!=grid_rows; ++y)
  {
    const auto& grid_row = grid[y];
    for (int x=0; x!=grid_cols; ++x)
    {
      const auto& cell_state = grid_row[x];

      //Cell type
      {
        if (cell_state.GetCellType() == CellType::alive) {
          sf::Sprite sprite(
            m_sprite.Get(CellType::alive),
            sf::IntRect(0,0,6,6)
          );
          sprite.setPosition(
            x * m_sprite.GetWidth(),
            y * m_sprite.GetHeight()
          );
          m_window.draw(sprite);
        }
      }

      //Heart of
      {
        sf::Sprite sprite(
          m_sprite.Get(cell_state.GetHeartOf()),
          sf::IntRect(0,0,6,6)
        );
        sprite.setPosition(
          x * m_sprite.GetWidth(),
          y * m_sprite.GetHeight()
        );
        m_window.draw(sprite);
      }
    }
  }

  // Draw hangars.
  sf::Sprite hsprite(m_hangars);
  m_window.draw(hsprite);

  //Draw selected
  for (const PlayerIndex player_index: GetAllPlayerIndices())
  {
    const auto player = m_game.GetPlayer(player_index);
    SelectedBy selected_by = SelectedBy::none;
    switch (player_index)
    {
      case PlayerIndex::player1: selected_by = SelectedBy::player1; break;
      case PlayerIndex::player2: selected_by = SelectedBy::player2; break;
    }
    sf::Sprite sprite(
      m_sprite.Get(selected_by),
      sf::IntRect(0,0,6,6)
    );
    sprite.setPosition(
      player.GetX() * m_sprite.GetWidth(),
      player.GetY() * m_sprite.GetHeight()
    );
    m_window.draw(sprite);
  }
  //Draw time
  {
    sf::Font font;
    const bool can_find{font.loadFromFile("../GameOfLifeFighter/Resources/Fonts/Courier.ttf")};
    assert(can_find);
    std::stringstream s;
    s << clock.getElapsedTime().asMilliseconds() << " ms";
    sf::Text text(s.str(),font);
    text.setString(s.str());
    m_window.draw(text);
  }

  m_window.display();

  #ifndef NDEBUG

  if (!"Create a screenshot")
  {
    const sf::Image screenshot = m_window.capture();
    screenshot.saveToFile("screenshot.png");
  }
  #endif
}

void golf::SfmlWidget::Execute()
{
  sf::RectangleShape shape(sf::Vector2f(500.0,250.0));
  shape.setFillColor(sf::Color::Green);
  shape.setOrigin(250.0,125.0);
  shape.setPosition(500,300);
  assert(m_window.isOpen());

  while (m_window.isOpen())
  {
    //Poll all events
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
        return;
      }
    }
    ProcessKeyboard();
    ProcessJoystick();

    ++m_tick;
    if (m_tick % 3 == 0)
    {
      m_game.Next();
    }
    m_game.PressKeys(m_keys);

    //Keys manipulation
    RemoveKey(Key::toggle_hangar1);
    RemoveKey(Key::toggle_hangar2);
    RemoveKey(Key::pattern_a1);
    RemoveKey(Key::pattern_a2);
    RemoveKey(Key::pattern_b1);
    RemoveKey(Key::pattern_b2);
    RemoveKey(Key::pattern_c1);
    RemoveKey(Key::pattern_c2);

    Draw();
  }

}


void golf::SfmlWidget::ProcessJoystick()
{
  sf::Joystick::update();
  if (sf::Joystick::isConnected(0))
  {
    //const int n_buttons = sf::Joystick::getButtonCount(0);

    const bool pressed0{sf::Joystick::isButtonPressed(0,0)};
    if (pressed0) { AddKey(Key::toggle_hangar2); }
    const bool pressed1{sf::Joystick::isButtonPressed(0,1)};
    if ( pressed1) { AddKey(Key::toggle_cell2); }
    if (!pressed1) { RemoveKey(Key::toggle_cell2); }
    const bool pressed2{sf::Joystick::isButtonPressed(0,2)};
    if ( pressed2) { AddKey(Key::pattern_a2); }
    const bool pressed3{sf::Joystick::isButtonPressed(0,3)};
    if ( pressed3) { AddKey(Key::pattern_b2); }
    const bool pressed4{sf::Joystick::isButtonPressed(0,4)};
    if ( pressed4) { AddKey(Key::pattern_c2); }

    RemoveKey(Key::up2);
    RemoveKey(Key::right2);
    RemoveKey(Key::down2);
    RemoveKey(Key::left2);
    if (sf::Joystick::hasAxis(0, sf::Joystick::X))
    {
      const double dx{sf::Joystick::getAxisPosition(0, sf::Joystick::X)};
      if (dx < -50.0) { AddKey(Key::left2 ); }
      if (dx >  50.0) { AddKey(Key::right2); }
    }
    if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(0, sf::Joystick::Y)};
      if (dy < -50.0) { AddKey(Key::up2  );  }
      if (dy >  50.0) { AddKey(Key::down2);  }
    }
  }
}

void golf::SfmlWidget::ProcessKeyboard()
{
  m_keys.clear();
  for (const auto key: m_key_map)
  {
    if (sf::Keyboard::isKeyPressed(key.first)) AddKey(key.second);
  }
}

void golf::SfmlWidget::RemoveKey(const Key key)
{
  assert(std::count(std::begin(m_keys),std::end(m_keys),key) <= 1);
  const auto new_end = std::remove(
    std::begin(m_keys),
    std::end(m_keys),
    key
  );
  m_keys.erase(new_end,std::end(m_keys));
}

#ifndef NDEBUG
void golf::SfmlWidget::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  SfmlHelper();
  SfmlSprites();
}
#endif
