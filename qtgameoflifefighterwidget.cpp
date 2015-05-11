#include "qtgameoflifefighterwidget.h"

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#include "ui_qtgameoflifefighterwidget.h"

QtGameOfLifeFighterWidget::QtGameOfLifeFighterWidget(
  const int width,
  const int height,
  QWidget *parent
)
  : QWidget(parent),
    ui(new Ui::QtGameOfLifeFighterWidget),
    m_pixmap(width,height),
    m_grid(width,height),
    m_color_map{}
{
  ui->setupUi(this);
  OnTimer();
  //Start a timer
  {
    QTimer * const timer{new QTimer(this)};
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->setInterval(1);
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

void QtGameOfLifeFighterWidget::OnTimer()
{
  m_grid.Next();
  const int height{m_pixmap.height()};
  const int width{m_pixmap.width()};
  QImage image(width,height,QImage::Format_RGB32);
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const auto i = m_grid.Get(x,y);
      image.setPixel(x,y,
        i == 0 ? qRgb(0,0,0) : qRgb(255,255,255)
      );
    }
  }
  m_pixmap = QPixmap::fromImage(image);
  update();
}

void QtGameOfLifeFighterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap(
    this->rect(),
    m_pixmap
  );
}
