#include <QApplication>
#include "qtgameoflifefighterwidget.h"
#include "gameoflifefightertrace.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  START_TRACE();
  golf::QtGameOfLifeFighterWidget w;
  w.show();
  return a.exec();
}
