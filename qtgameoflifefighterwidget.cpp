#include "qtgameoflifefighterwidget.h"

#include <cassert>

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QKeyEvent>
#include <QDesktopWidget>
#include "ui_qtgameoflifefighterwidget.h"

QtGameOfLifeFighterWidget::QtGameOfLifeFighterWidget(
  const int width,
  const int height,
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtGameOfLifeFighterWidget),
    m_pixmap(width,height),
    m_game(width,height),
    m_color_map{},
    m_keys_pressed{},
    m_x1{width / 2},
    m_y1{height * 1 / 4},
    m_x2{width / 2},
    m_y2{height * 3 / 4}
{
  ui->setupUi(this);
  OnTimer();
  {
    //Put the dialog in the screen center
    const QRect screen = QApplication::desktop()->screenGeometry();

    this->setGeometry(0,0,screen.height() * 6 / 24,screen.height() * 20 / 24);
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

QtGameOfLifeFighterWidget::~QtGameOfLifeFighterWidget()
{
  delete ui;
}

void QtGameOfLifeFighterWidget::Blend(
  QImage& image,
  const int x, const int y,
  const QColor color
)
{
  Blend(image,x,y,color.red(),color.green(),color.blue());
}

void QtGameOfLifeFighterWidget::Blend(
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

std::map<int,QColor> QtGameOfLifeFighterWidget::CreateColorMap() noexcept
{
  std::map<int,QColor> m;
  return m;
}

void QtGameOfLifeFighterWidget::keyPressEvent(QKeyEvent * e)
{
  m_keys_pressed.insert(e->key());
}

void QtGameOfLifeFighterWidget::keyReleaseEvent(QKeyEvent * e)
{
  m_keys_pressed.erase(e->key());
}

void QtGameOfLifeFighterWidget::OnTimer()
{
  m_game.Next();

  for (const auto key: m_keys_pressed)
  {
    switch (key)
    {
      case Qt::Key_A: m_x1 = (m_x1 - 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_D: m_x1 = (m_x1 + 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_W: m_y1 = (m_y1 - 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_S: m_y1 = (m_y1 + 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_Q: m_game.Set(m_x1,m_y1,1); break;

      case Qt::Key_J: m_x2 = (m_x2 - 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_L: m_x2 = (m_x2 + 1 + m_game.GetWidth()) % m_game.GetWidth(); break;
      case Qt::Key_I: m_y2 = (m_y2 - 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_K: m_y2 = (m_y2 + 1 + m_game.GetHeight()) % m_game.GetHeight(); break;
      case Qt::Key_U: m_game.Set(m_x2,m_y2,1); break;
    }
  }

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
    const auto player = hangar.GetPlayer();
    for (int y=0; y!=height; ++y)
    {
      for (int x=0; x!=width; ++x)
      {
        Blend(image,x+left,y+top,ToColor(player));
      }
    }

  }

  //Display players
  Blend(image,m_x1,m_y1,ToColor(Player::player1));
  Blend(image,m_x2,m_y2,ToColor(Player::player2));


  m_pixmap = QPixmap::fromImage(image);
  update(); //Essential
}

void QtGameOfLifeFighterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    m_pixmap
  );
}

QColor QtGameOfLifeFighterWidget::ToColor(const Player player) noexcept
{
  switch (player)
  {
    case Player::player1: return qRgb(255,0,0);
    case Player::player2: return qRgb(0,0,255);
  }
  assert(!"Should not get here");
  return qRgb(0,0,0);
}
