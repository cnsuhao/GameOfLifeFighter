#ifndef QTGAMEOFLIFEWIDGET_H
#define QTGAMEOFLIFEWIDGET_H

#include <set>
#include <map>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QWidget>
#include <QPixmap>
#pragma GCC diagnostic pop

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
  Game m_game;
  std::map<int,Key> m_key_map;
  std::vector<Key> m_keys;
  QPixmap m_pixmap;

  ///The number of ticks the game is running
  int m_tick;

  void AddKey(const Key key);
  static std::map<int,Key> CreateInitialKeyMap() noexcept;
  void RemoveKey(const Key key);

  #ifndef NDEBUG
  void Test() noexcept;
  #endif

private slots:
  void OnJoystickCheck();
  void OnTimer();
};

} //~namespace golf

#endif // QTGAMEOFLIFEWIDGET_H
