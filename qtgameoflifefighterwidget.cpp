#include "qtgameoflifefighterwidget.h"

#include <cassert>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QDesktopWidget>

#include <SFML/Graphics.hpp>

#include "qtgameoflifefighterplayerindex.h"
#include "qtgameoflifefighterhelper.h"
#include "qtgameoflifefightersprite.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefightertrace.h"
#include "gameoflifefighterplayerindex.h"
#include "ui_qtgameoflifefighterwidget.h"

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
  m_keys.insert( (*iter).second );
}

void golf::QtGameOfLifeFighterWidget::keyReleaseEvent(QKeyEvent * e)
{
  const auto iter = m_key_map.find(e->key());
  if (iter == std::end(m_key_map)) return;
  m_keys.erase( (*iter).second );
}

void golf::QtGameOfLifeFighterWidget::OnJoystickCheck()
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
    m_keys.erase(Key::down2 );
    m_keys.erase(Key::left2 );
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

void golf::QtGameOfLifeFighterWidget::OnTimer()
{
  ++m_tick;
  if (m_tick % 3 == 0)
  {
    m_game.Next();
  }
  m_game.PressKeys(m_keys);
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
}

void golf::QtGameOfLifeFighterWidget::paintEvent(QPaintEvent *)
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
