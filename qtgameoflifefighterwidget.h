#ifndef QTGAMEOFLIFEWIDGET_H
#define QTGAMEOFLIFEWIDGET_H

#include <set>
#include <map>
#include <QWidget>
#include <QPixmap>
#include "game.h"

namespace Ui {
  class QtGameOfLifeFighterWidget;
}

struct QImage;

class QtGameOfLifeFighterWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QtGameOfLifeFighterWidget(
    const int width = 60,
    const int height = 200,
    QWidget *parent = 0
  );
  QtGameOfLifeFighterWidget(const QtGameOfLifeFighterWidget&) = delete;
  QtGameOfLifeFighterWidget& operator=(const QtGameOfLifeFighterWidget&) = delete;
  ~QtGameOfLifeFighterWidget();

protected:
  void paintEvent(QPaintEvent *);
  void keyPressEvent(QKeyEvent *);
  void keyReleaseEvent(QKeyEvent *);
private:
  Ui::QtGameOfLifeFighterWidget *ui;
  QPixmap m_pixmap;
  Game m_game;
  std::map<int,QColor> m_color_map;
  std::set<int> m_keys_pressed;
  int m_x1;
  int m_y1;
  int m_x2;
  int m_y2;
private slots:
  void OnTimer();

  static std::map<int,QColor> CreateColorMap() noexcept;

};

#endif // QTGAMEOFLIFEWIDGET_H
