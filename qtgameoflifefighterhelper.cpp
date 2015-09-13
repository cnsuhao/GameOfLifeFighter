#include "qtgameoflifefighterhelper.h"

#include <cassert>

#include <QColor>

golf::QtHelper::QtHelper()
{
  #ifndef NDEBUG
  Test();
  #endif
}


QColor golf::QtHelper::Blend(
  const QColor a,
  const QColor b
) const noexcept
{
  return qRgb(
    (a.red() + b.red()) / 2,
    (a.green() + b.green()) / 2,
    (a.blue() + b.blue()) / 2
  );
}


void golf::QtHelper::Blend(
  QImage& image,
  const int x, const int y,
  const QColor color
) const noexcept
{
  Blend(image,x,y,color.red(),color.green(),color.blue());
}

void golf::QtHelper::Blend(
  QImage& image,
  const int x, const int y,
  const int r, const int g, const int b
) const noexcept
{
  const QColor here{image.pixel(x,y)};
  image.setPixel(
    x,y,
    qRgb(
      (here.red()   + r) / 2,
      (here.green() + g) / 2,
      (here.blue()  + b) / 2
    )
  );
}

#ifndef NDEBUG
void golf::QtHelper::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
}
#endif
