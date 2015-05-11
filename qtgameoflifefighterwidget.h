#ifndef QTPAPERROCKSCISSORSWIDGET_H
#define QTPAPERROCKSCISSORSWIDGET_H

#include <QWidget>
#include <QPixmap>

struct Grid
{
  Grid(int,int) {}
  void Next() {}
  int Get(int,int) { return 0; }
};

namespace Ui {
  class QtGameOfLifeFighterWidget;
}

struct QImage;

class QtGameOfLifeFighterWidget : public QWidget
{
  Q_OBJECT

public:
  explicit QtGameOfLifeFighterWidget(
    const int width = 600,
    const int height = 400,
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

private slots:
  void OnTimer();
};

#endif // QTPAPERROCKSCISSORSWIDGET_H
