#ifndef QTGAMEOFLIFEWIDGET_H
#define QTGAMEOFLIFEWIDGET_H

#include <map>
#include <QWidget>
#include <QPixmap>
#include "grid.h"

namespace Ui {
  class QtGameOfLifeFighterWidget;
}

struct QImage;

class QtGameOfLifeFighterWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QtGameOfLifeFighterWidget(
    const int width = 100,
    const int height = 100,
    QWidget *parent = 0
  );
  QtGameOfLifeFighterWidget(const QtGameOfLifeFighterWidget&) = delete;
  QtGameOfLifeFighterWidget& operator=(const QtGameOfLifeFighterWidget&) = delete;
  ~QtGameOfLifeFighterWidget();

protected:
  void paintEvent(QPaintEvent *);
private:
  Ui::QtGameOfLifeFighterWidget *ui;
  QPixmap m_pixmap;
  Grid m_grid;
  std::map<int,QColor> m_color_map;

private slots:
  void OnTimer();

  static std::map<int,QColor> CreateColorMap() noexcept;

};

#endif // QTGAMEOFLIFEWIDGET_H
