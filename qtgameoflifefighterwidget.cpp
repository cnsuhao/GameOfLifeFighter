#include "qtgameoflifefighterwidget.h"

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
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const auto i = m_game.GetGrid(x,y);
      //Q
      image.setPixel(x,y,
        i == 0 ? qRgb(0,0,0) : qRgb(255,255,255)
      );
    }
  }

  const QColor pixel1{image.pixel(m_x1,m_y1)};
  const QColor pixel2{image.pixel(m_x2,m_y2)};

  image.setPixel(m_x1,m_y1,
    qRgb(
      255,
      pixel1.green() / 2,
      pixel1.blue()  / 2
    )
  );
  image.setPixel(
    m_x2,m_y2,
    qRgb(
      pixel2.red()   / 2,
      pixel2.green() / 2,
      255
    )
  );

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
