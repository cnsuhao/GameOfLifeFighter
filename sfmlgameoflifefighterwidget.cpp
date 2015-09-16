#include "sfmlgameoflifefighterwidget.h"

#include <cassert>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio/Music.hpp>

#include "sfmlgameoflifefighterplayerindex.h"
#include "sfmlgameoflifefighterhelper.h"
#include "sfmlgameoflifefightersprite.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefighterplayerindex.h"

golf::SfmlWidget::SfmlWidget()
  : m_game{},
    m_key_map{CreateInitialKeyMap()},
    m_keys{},
    m_pixmap(
      sf::VideoMode(
        Game().GetWidth() * SfmlSprite().GetWidth(),
        Game().GetHeight() * SfmlSprite().GetHeight()
      ),
      "Game Of Life Fighter"
    ),
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
  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};

  m_pixmap.clear();

  QImage image(
    grid_cols * QtSprite().GetWidth(),
    grid_rows * QtSprite().GetHeight(),
    QImage::Format_ARGB32
  );

  const auto grid = m_game.GetCellStateGrid();
  for (int y=0; y!=grid_rows; ++y)
  {
    const auto& grid_row = grid[y];
    for (int x=0; x!=grid_cols; ++x)
    {
      QtHelper().DrawImage(
        image,
        QtSprite().Create(grid_row[x]),
        x * QtSprite().GetWidth(),
        y * QtSprite().GetHeight()
      );

    }
  }

  m_pixmap.draw(shape);
  shape.setRotation(angle);
  m_pixmap.display();
  angle += 1.0;

}

void golf::SfmlWidget::Execute()
{
  sf::RectangleShape shape(sf::Vector2f(500.0,250.0));
  shape.setFillColor(sf::Color::Green);
  shape.setOrigin(250.0,125.0);
  shape.setPosition(500,300);
  double angle = 0.0;

  while (m_pixmap.isOpen())
  {
    sf::Event event;
    while (m_pixmap.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_pixmap.close();
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
    if (pressed0) { m_keys.insert(Key::toggle_hangar2); }
    const bool pressed1{sf::Joystick::isButtonPressed(0,1)};
    if ( pressed1) { m_keys.insert(Key::toggle_cell2); }
    if (!pressed1) { m_keys.erase(Key::toggle_cell2); }
    const bool pressed2{sf::Joystick::isButtonPressed(0,2)};
    if ( pressed2) { m_keys.insert(Key::pattern_a2); }
    const bool pressed3{sf::Joystick::isButtonPressed(0,3)};
    if ( pressed3) { m_keys.insert(Key::pattern_b2); }
    const bool pressed4{sf::Joystick::isButtonPressed(0,4)};
    if ( pressed4) { m_keys.insert(Key::pattern_c2); }

    m_keys.erase(Key::up2);
    m_keys.erase(Key::right2);
    m_keys.erase(Key::down2);
    m_keys.erase(Key::left2);
    if (sf::Joystick::hasAxis(0, sf::Joystick::X))
    {
      const double dx{sf::Joystick::getAxisPosition(0, sf::Joystick::X)};
      if (dx < -50.0) { m_keys.insert(Key::left2 ); }
      if (dx >  50.0) { m_keys.insert(Key::right2); }
    }
    if (sf::Joystick::hasAxis(0, sf::Joystick::Y))
    {
      const double dy{sf::Joystick::getAxisPosition(0, sf::Joystick::Y)};
      if (dy < -50.0) { m_keys.insert(Key::up2  );  }
      if (dy >  50.0) { m_keys.insert(Key::down2);  }
    }
  }
}

void golf::SfmlWidget::ProcessKeyboard()
{
  m_keys.clear();
  for (const auto key: m_key_map)
  {
    if (sf::Keyboard::isKeyPressed(key.first)) m_keys.insert(key.second);
  }
}

void golf::SfmlWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    rect(),
    m_pixmap
  );
}

QKeyEvent CreateDel() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier); }
QKeyEvent CreateK() { return QKeyEvent(QEvent::KeyPress,Qt::Key_K,Qt::NoModifier); }
QKeyEvent CreateS() { return QKeyEvent(QEvent::KeyPress,Qt::Key_S,Qt::NoModifier); }
QKeyEvent CreateSpace() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier); }
QKeyEvent CreateDown() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateControlDown() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::ControlModifier); }
QKeyEvent CreateControlE() { return QKeyEvent(QEvent::KeyPress,Qt::Key_E,Qt::ControlModifier); }
QKeyEvent CreateControlN() { return QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::ControlModifier); }

void golf::SfmlWidget::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  QtHelper();
  QtSprite();
  //A key 'S' press should move player 1 down
  {
    SfmlWidget w;
    auto down = CreateS();
    const int y_before{w.m_game.GetPlayer(PlayerIndex::player1).GetY()};
    w.keyPressEvent(&down);
    w.OnTimer();
    const int y_after{w.m_game.GetPlayer(PlayerIndex::player1).GetY()};
    assert(y_after == y_before + 1);
  }
  //A key 'K' (arrows do not do anything) press should move player 2 down
  {
    SfmlWidget w;
    auto down = CreateK();
    const int y_before{w.m_game.GetPlayer(PlayerIndex::player2).GetY()};
    w.keyPressEvent(&down);
    w.OnTimer();
    const int y_after{w.m_game.GetPlayer(PlayerIndex::player2).GetY()};
    assert(y_after == y_before + 1);
  }
}
