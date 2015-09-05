#include <QApplication>
#include "qtgameoflifefighterwidget.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  golf::QtGameOfLifeFighterWidget w;
  w.show();
  return a.exec();
}
