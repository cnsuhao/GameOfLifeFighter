#include "qtgameoflifefighterwidget.h"

#include <cassert>
#include <chrono>
#include <iostream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QDesktopWidget>
#pragma GCC diagnostic pop

#include <SFML/Graphics.hpp>

#include "qtgameoflifefighterplayerindex.h"
#include "qtgameoflifefighterhelper.h"
#include "qtgameoflifefightersprite.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefighterplayerindex.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_qtgameoflifefighterwidget.h"
#pragma GCC diagnostic pop

golf::QtGameOfLifeFighterWidget::QtGameOfLifeFighterWidget(
  const int width,
  const int height,
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtGameOfLifeFighterWidget),
    m_game{},
    m_key_map{CreateInitialKeyMap()},
    m_keys{},
    m_pixmap(width * QtSprite().GetWidth(),height * QtSprite().GetHeight()),
    m_tick{0}
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
  OnTimer();
  {
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();
    //this->setGeometry(0,0,screen.width() * 20 / 24,screen.height() * 8 / 24);
    this->setGeometry(0,0,m_pixmap.width(),m_pixmap.height());
    this->move( screen.center() - this->rect().center() );
  }
  //Start the timer
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->setInterval(20);
    timer->start();
  }
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnJoystickCheck()));
    timer->setInterval(20);
    timer->start();
  }
}

golf::QtGameOfLifeFighterWidget::~QtGameOfLifeFighterWidget()
{
  delete ui;
}

void golf::QtGameOfLifeFighterWidget::AddKey(const Key key)
{
  if (std::count(std::begin(m_keys),std::end(m_keys),key) == 0)
  {
    m_keys.push_back(key);
  }
  assert(std::count(std::begin(m_keys),std::end(m_keys),key) == 1);
}

std::map<int,golf::Key> golf::QtGameOfLifeFighterWidget::CreateInitialKeyMap() noexcept
{
  std::map<int,Key> m;
  m.insert( { Qt::Key_Q, Key::toggle_cell1}  );
  m.insert( { Qt::Key_W, Key::up1}  );
  m.insert( { Qt::Key_E, Key::toggle_hangar1}  );

  m.insert( { Qt::Key_A, Key::left1}  );
  m.insert( { Qt::Key_S, Key::down1}  );
  m.insert( { Qt::Key_D, Key::right1}  );

  m.insert( { Qt::Key_Z, Key::pattern_a1}  );
  m.insert( { Qt::Key_X, Key::pattern_b1}  );
  m.insert( { Qt::Key_C, Key::pattern_c1}  );

  m.insert( { Qt::Key_U, Key::toggle_cell2}  );
  m.insert( { Qt::Key_I, Key::up2}  );
  m.insert( { Qt::Key_O, Key::toggle_hangar2}  );

  m.insert( { Qt::Key_J, Key::left2}  );
  m.insert( { Qt::Key_K, Key::down2}  );
  m.insert( { Qt::Key_L, Key::right2}  );

  m.insert( { Qt::Key_M     , Key::pattern_a2}  );
  m.insert( { Qt::Key_Comma , Key::pattern_b2}  );
  m.insert( { Qt::Key_Period, Key::pattern_c2}  );

  return m;
}

void golf::QtGameOfLifeFighterWidget::keyPressEvent(QKeyEvent * e)
{
  const auto iter = m_key_map.find(e->key());
  if (iter == std::end(m_key_map)) return;
  AddKey((*iter).second);
}

void golf::QtGameOfLifeFighterWidget::keyReleaseEvent(QKeyEvent * e)
{
  const auto iter = m_key_map.find(e->key());
  if (iter == std::end(m_key_map)) return;
  RemoveKey((*iter).second);
}

void golf::QtGameOfLifeFighterWidget::OnJoystickCheck()
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

void golf::QtGameOfLifeFighterWidget::OnTimer()
{
  ++m_tick;
  if (m_tick % 4 == 0)
  {
    m_game.Next();

    //Game over? Then close the game after 5000 msecs
    if (m_game.GetGameState() != GameState::playing)
    {
      QTimer::singleShot(5000,this,SLOT(close()));
    }
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


  const int grid_rows{m_game.GetHeight()};
  const int grid_cols{m_game.GetWidth()};
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
  m_pixmap = QPixmap::fromImage(image);
  update(); //Essential
  /*
  {
    const auto t_start = std::chrono::system_clock::now();
    while (1)
    {
      const auto t_now = std::chrono::system_clock::now();
      if (std::chrono::duration_cast<std::chrono::seconds>(t_now - t_start).count() > 10.0) //msecs
      {
        std::exit(0);
      }
    }
  }
  */
}

void golf::QtGameOfLifeFighterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    rect(),
    m_pixmap
  );
}

void golf::QtGameOfLifeFighterWidget::RemoveKey(const Key key)
{
  assert(std::count(std::begin(m_keys),std::end(m_keys),key) <= 1);
  const auto new_end = std::remove(
    std::begin(m_keys),
    std::end(m_keys),
    key
  );
  m_keys.erase(new_end,std::end(m_keys));
}

QKeyEvent CreateDel() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier); }
QKeyEvent CreateK() { return QKeyEvent(QEvent::KeyPress,Qt::Key_K,Qt::NoModifier); }
QKeyEvent CreateS() { return QKeyEvent(QEvent::KeyPress,Qt::Key_S,Qt::NoModifier); }
QKeyEvent CreateSpace() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Space,Qt::NoModifier); }
QKeyEvent CreateDown() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::NoModifier); }
QKeyEvent CreateControlDown() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Down,Qt::ControlModifier); }
QKeyEvent CreateControlE() { return QKeyEvent(QEvent::KeyPress,Qt::Key_E,Qt::ControlModifier); }
QKeyEvent CreateControlN() { return QKeyEvent(QEvent::KeyPress,Qt::Key_N,Qt::ControlModifier); }

#ifndef NDEBUG
void golf::QtGameOfLifeFighterWidget::Test() noexcept
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
    QtGameOfLifeFighterWidget w;
    auto down = CreateS();
    const int y_before{w.m_game.GetPlayer(PlayerIndex::player1).GetY()};
    w.keyPressEvent(&down);
    w.OnTimer();
    const int y_after{w.m_game.GetPlayer(PlayerIndex::player1).GetY()};
    assert(y_after == y_before + 1);
  }
  //A key 'K' (arrows do not do anything) press should move player 2 down
  {
    QtGameOfLifeFighterWidget w;
    auto down = CreateK();
    const int y_before{w.m_game.GetPlayer(PlayerIndex::player2).GetY()};
    w.keyPressEvent(&down);
    w.OnTimer();
    const int y_after{w.m_game.GetPlayer(PlayerIndex::player2).GetY()};
    assert(y_after == y_before + 1);
  }
}
#endif
