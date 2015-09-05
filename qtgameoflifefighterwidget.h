#ifndef QTGAMEOFLIFEWIDGET_H
#define QTGAMEOFLIFEWIDGET_H

#include <set>
#include <map>
#include <QWidget>
#include <QPixmap>
#include "gameoflifefightergame.h"

namespace Ui {
  class QtGameOfLifeFighterWidget;
}

struct QImage;

namespace golf {

class QtGameOfLifeFighterWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QtGameOfLifeFighterWidget(
    const int width = 200,
    const int height = 60,
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
  std::map<int,QColor> m_color_map;
  Game m_game;
  std::set<Key> m_keys;
  QPixmap m_pixmap;

  static void Blend(
    QImage& image,
    const int x, const int y,
    const int r, const int g, const int b
  );
  static void Blend(
    QImage& image,
    const int x, const int y,
    const QColor color
  );
  static QColor ToColor(const Player player) noexcept;

private slots:
  void OnTimer();

  static std::map<int,QColor> CreateColorMap() noexcept;

};

} //~namespace golf

#endif // QTGAMEOFLIFEWIDGET_H
