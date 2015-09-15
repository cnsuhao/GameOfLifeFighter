#ifndef QTGAMEOFLIFEFIGHTERHELPER_H
#define QTGAMEOFLIFEFIGHTERHELPER_H

namespace golf {

struct QtHelper
{
  QtHelper();

/*

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

  ///Create a pretty QImage for testing purposes
  QImage CreateImage(const int width, const int height, const int z = 0) const noexcept;

  ///Draw a QImage on another QImage
  ///Scanline with std::copy
  void DrawImage(QImage& target, const QImage& source,  const int left, const int top) const noexcept;

  std::string GetVersion() const noexcept;
  std::vector<std::string> GetVersionHistory() const noexcept;
*/
private:
/*
  ///Draw a QImage on another QImage
  ///Scanline with indexed copy
  void DrawImageSlow(QImage& target, const QImage& source,  const int left, const int top) const noexcept;

  ///Draw a QImage on another QImage
  ///Pixel-by-pixel
  void DrawImageSlowest(QImage& target, const QImage& source,  const int left, const int top) const noexcept;
*/
  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace golf

#endif // QTGAMEOFLIFEFIGHTERHELPER_H
