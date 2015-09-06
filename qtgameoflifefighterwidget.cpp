#include "qtgameoflifefighterwidget.h"

#include <cassert>
#include <iostream>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QDesktopWidget>

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
    m_keys{},
    m_pixmap(width,height)
{
  #ifndef NDEBUG
  Test();
  #endif
  ui->setupUi(this);
  OnTimer();
  {
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();

    this->setGeometry(0,0,screen.height() * 20 / 24,screen.height() * 6 / 24);
    this->move( screen.center() - this->rect().center() );
  }
  //Start a timer
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->setInterval(100);
    timer->start();
  }
}

golf::QtGameOfLifeFighterWidget::~QtGameOfLifeFighterWidget()
{
  delete ui;
}

void golf::QtGameOfLifeFighterWidget::Blend(
  QImage& image,
  const int x, const int y,
  const QColor color
)
{
  Blend(image,x,y,color.red(),color.green(),color.blue());
}

void golf::QtGameOfLifeFighterWidget::Blend(
  QImage& image,
  const int x, const int y,
  const int r, const int g, const int b
)
{
  const QColor here{image.pixel(x,y)};
  image.setPixel(
    x,y,
    qRgb(
      (here.red()   + r) / 2,
      (here.green() + g) / 2,
      (here.blue() + b) / 2
    )
  );
}

std::map<int,QColor> golf::QtGameOfLifeFighterWidget::CreateColorMap() noexcept
{
  std::map<int,QColor> m;
  return m;
}

void golf::QtGameOfLifeFighterWidget::keyPressEvent(QKeyEvent * e)
{
  std::clog << e->key() << std::endl;
  switch (e->key())
  {
    case Qt::Key_A: m_keys.insert(Key::left1); break;
    case Qt::Key_D: m_keys.insert(Key::right1); break;
    case Qt::Key_W: m_keys.insert(Key::up1); break;
    case Qt::Key_S: m_keys.insert(Key::down1); break;
    case Qt::Key_Q: m_keys.insert(Key::set_low1); break;
    case Qt::Key_E: m_keys.insert(Key::set_high1); break;
    case Qt::Key_Z: m_keys.insert(Key::close_hangar1); break;
    case Qt::Key_C: m_keys.insert(Key::open_hangar1); break;

    case Qt::Key_J: m_keys.insert(Key::left2); break;
    case Qt::Key_L: m_keys.insert(Key::right2); break;
    case Qt::Key_I: m_keys.insert(Key::up2); break;
    case Qt::Key_K: m_keys.insert(Key::down2); break;
    case Qt::Key_U: m_keys.insert(Key::set_low2); break;
    case Qt::Key_O: m_keys.insert(Key::set_high2); break;
    case Qt::Key_M: m_keys.insert(Key::close_hangar2); break;
    case Qt::Key_Period: m_keys.insert(Key::open_hangar2); break;
  }
}

void golf::QtGameOfLifeFighterWidget::keyReleaseEvent(QKeyEvent * e)
{
  std::clog << e->key() << std::endl;

  switch (e->key())
  {
    case Qt::Key_A: m_keys.erase(Key::left1); break;
    case Qt::Key_D: m_keys.erase(Key::right1); break;
    case Qt::Key_W: m_keys.erase(Key::up1); break;
    case Qt::Key_S: m_keys.erase(Key::down1); break;
    case Qt::Key_Q: m_keys.erase(Key::set_low1); break;
    case Qt::Key_E: m_keys.erase(Key::set_high1); break;
    case Qt::Key_Z: m_keys.erase(Key::close_hangar1); break;
    case Qt::Key_C: m_keys.erase(Key::open_hangar1); break;

    case Qt::Key_J: m_keys.erase(Key::left2); break;
    case Qt::Key_L: m_keys.erase(Key::right2); break;
    case Qt::Key_I: m_keys.erase(Key::up2); break;
    case Qt::Key_K: m_keys.erase(Key::down2); break;
    case Qt::Key_U: m_keys.erase(Key::set_low2); break;
    case Qt::Key_O: m_keys.erase(Key::set_high2); break;
    case Qt::Key_M: m_keys.erase(Key::close_hangar2); break;
    case Qt::Key_Period: m_keys.erase(Key::open_hangar2); break;
  }
}

void golf::QtGameOfLifeFighterWidget::OnTimer()
{
  m_game.Next();
  m_game.PressKeys(m_keys);
  const int height{m_pixmap.height()};
  const int width{m_pixmap.width()};
  QImage image(width,height,QImage::Format_RGB32);

  //Display grid
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const auto i = m_game.GetGrid(x,y);
      image.setPixel(x,y,
        i == 0 ? qRgb(0,0,0) : qRgb(255,255,255)
      );
    }
  }
  //Display hangars
  for (const Hangar& hangar: m_game.GetHangars())
  {
    const int left{hangar.GetLeft()};
    const int top{hangar.GetTop()};
    const int width{hangar.GetWidth()};
    const int height{hangar.GetHeight()};
    const auto player_index = hangar.GetPlayerIndex();
    for (int y=0; y!=height; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        Blend(image,x+left,y+top,ToColor(player_index));
      }
    }

  }

  //Display players
  const auto players = m_game.GetPlayers();
  assert(players.size() == 2);
  Blend(image,players[0].GetX(),players[0].GetY(),ToColor(PlayerIndex::player1));
  Blend(image,players[1].GetX(),players[1].GetY(),ToColor(PlayerIndex::player2));


  m_pixmap = QPixmap::fromImage(image);
  update(); //Essential
}

void golf::QtGameOfLifeFighterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    m_pixmap
  );
}

QKeyEvent CreateDel() { return QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier); }
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
  //A key down press should move player 2 down
  {
    QtGameOfLifeFighterWidget w;
    auto down = CreateDown();
    const int y_before{w.m_game.GetPlayers()[1].GetY()};
    TRACE(y_before);
    w.keyPressEvent(&down);
    w.OnTimer();
    const int y_after{w.m_game.GetPlayers()[1].GetY()};
    TRACE(y_after);
    assert(y_after == y_before + 1);
  }
}



