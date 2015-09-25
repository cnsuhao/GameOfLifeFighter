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
      //sf::VideoMode::getFullscreenModes()[0],
      sf::VideoMode(
        Game().GetWidth() * SfmlSprites().GetWidth(),
        Game().GetHeight() * SfmlSprites().GetHeight()
      ),
      "Game Of Life Fighter",
      sf::Style::None
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

  //Center the window
  {
    const int sw{static_cast<int>(sf::VideoMode::getDesktopMode().width)};
    const int sh{static_cast<int>(sf::VideoMode::getDesktopMode().height)};
    const int ww{static_cast<int>(m_window.getSize().x)};
    const int wh{static_cast<int>(m_window.getSize().y)};
    m_window.setPosition(
      sf::Vector2i(
        (sw - ww) / 2,
        (sh - wh) / 2
      )
    );
  }
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


void golf::SfmlWidget::Execute()
{
  const int time_per_frame_msecs = 1000 / 60;
  sf::RectangleShape shape(sf::Vector2f(500.0,250.0));
  shape.setFillColor(sf::Color::Green);
  shape.setOrigin(250.0,125.0);
  shape.setPosition(500,300);
  assert(m_window.isOpen());

  while (m_window.isOpen())
  {
    sf::Clock clock;
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

    Render();
    //Delay to get a desired frameframe
    while (clock.getElapsedTime().asMilliseconds() < time_per_frame_msecs) { }
  }

}


void golf::SfmlWidget::ProcessJoystick()
{
  sf::Joystick::update();
  const int key_activate{0};
  const int key_build{2};
  const int key_glider{3};
  const int key_spaceship{1};
  const int key_grower{4};

  if (sf::Joystick::isConnected(0))
  {
    //First move, then build
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

    const bool pressed0{sf::Joystick::isButtonPressed(0,key_activate)};
    if (pressed0) { AddKey(Key::toggle_hangar2); }
    const bool pressed1{sf::Joystick::isButtonPressed(0,key_build)};
    if ( pressed1) { AddKey(Key::toggle_cell2); }
    if (!pressed1) { RemoveKey(Key::toggle_cell2); }
    const bool pressed2{sf::Joystick::isButtonPressed(0,key_glider)};
    if ( pressed2) { AddKey(Key::pattern_a2); }
    const bool pressed3{sf::Joystick::isButtonPressed(0,key_spaceship)};
    if ( pressed3) { AddKey(Key::pattern_b2); }
    const bool pressed4{sf::Joystick::isButtonPressed(0,key_grower)};
    if ( pressed4) { AddKey(Key::pattern_c2); }
  }

  if (sf::Joystick::isConnected(1))
  {
    //First move, then build
    RemoveKey(Key::up1);
    RemoveKey(Key::right1);
    RemoveKey(Key::down1);
    RemoveKey(Key::left1);
    if (sf::Joystick::hasAxis(1, sf::Joystick::X))
    {
      const double dx{sf::Joystick::getAxisPosition(1, sf::Joystick::X)};
      if (dx < -50.0) { AddKey(Key::left1 ); }
      if (dx >  50.0) { AddKey(Key::right1); }
    }
    if (sf::Joystick::hasAxis(1, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(1, sf::Joystick::Y)};
      if (dy < -50.0) { AddKey(Key::up1  );  }
      if (dy >  50.0) { AddKey(Key::down1);  }
    }

    const bool pressed0{sf::Joystick::isButtonPressed(1,key_activate)};
    if (pressed0) { AddKey(Key::toggle_hangar1); }
    const bool pressed1{sf::Joystick::isButtonPressed(1,key_build)};
    if ( pressed1) { AddKey(Key::toggle_cell1); }
    if (!pressed1) { RemoveKey(Key::toggle_cell1); }
    const bool pressed2{sf::Joystick::isButtonPressed(1,key_glider)};
    if ( pressed2) { AddKey(Key::pattern_a1); }
    const bool pressed3{sf::Joystick::isButtonPressed(1,key_spaceship)};
    if ( pressed3) { AddKey(Key::pattern_b1); }
    const bool pressed4{sf::Joystick::isButtonPressed(1,key_grower)};
    if ( pressed4) { AddKey(Key::pattern_c1); }
  }
}

void golf::SfmlWidget::ProcessKeyboard()
{
  m_keys.clear();
  for (const auto key: m_key_map)
  {
    if (sf::Keyboard::isKeyPressed(key.first)) AddKey(key.second);
  }

  const bool esc_pressed{sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)};
  const bool lalt_pressed{sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)};
  const bool ralt_pressed{sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt)};
  const bool f4_pressed{sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4)};
  const bool alt_pressed{lalt_pressed || ralt_pressed};
  const bool alt_f4_pressed{alt_pressed && f4_pressed};
  if (esc_pressed || alt_f4_pressed)
  {
    std::exit(0);
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

void golf::SfmlWidget::Render()
{
  sf::Clock clock;

  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};

  m_window.clear();

  // Draw background.
  sf::Sprite bgsprite(m_background);
  m_window.draw(bgsprite);

  // Draw hangars.
  sf::Sprite hsprite(m_hangars);
  m_window.draw(hsprite);


  //Draw cells
  const auto grid = m_game.GetCellStateGrid();
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

      //Building here?
      if (cell_state.GetIsBuilding() != IsBuilding::none)
      {
        sf::Sprite sprite(
          m_sprite.Get(cell_state.GetIsBuilding()),
          sf::IntRect(0,0,6,6)
        );
        sprite.setPosition(
          x * m_sprite.GetWidth(),
          y * m_sprite.GetHeight()
        );
        m_window.draw(sprite);
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


  //Draw cursors
  for (const PlayerIndex player_index: GetAllPlayerIndices())
  {
    const auto player = m_game.GetPlayer(player_index);
    sf::Sprite sprite(
      m_sprite.Get(player_index),
      sf::IntRect(0,0,6,6)
    );
    sprite.setPosition(
      player.GetX() * m_sprite.GetWidth(),
      player.GetY() * m_sprite.GetHeight()
    );
    m_window.draw(sprite);
  }
  //Draw time
  #ifndef NDEBUG
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
  #endif
  m_window.display();
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
