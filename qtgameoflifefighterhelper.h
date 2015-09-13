#ifndef QTGAMEOFLIFEFIGHTERHELPER_H
#define QTGAMEOFLIFEFIGHTERHELPER_H

#include <QImage>

namespace golf {

struct QtHelper
{
  QtHelper();


  ///Mix the color of a QImage its pixel with a new RGB set
  void Blend(
    QImage& image,
    const int x, const int y,
    const int r, const int g, const int b
  ) const noexcept;

  ///Mix the color of a QImage its pixel with a QColor
  void Blend(
    QImage& image,
    const int x, const int y,
    const QColor color
  ) const noexcept;

  ///Mix two colors
  QColor Blend(
    const QColor a,
    const QColor b
  ) const noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // QTGAMEOFLIFEFIGHTERHELPER_H
