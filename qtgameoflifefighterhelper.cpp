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


QImage golf::QtHelper::CreateImage(const int width, const int height, const int z) const noexcept
{
  QImage image(width,height,QImage::Format_ARGB32);
  for (int y=0;y!=height;++y)
  {
    for (int x=0;x!=width;++x)
    {
      image.setPixel(
        x,y,
        qRgb((x+z+0)%256,(y+z+0)%256,(x+y+z)%256) //Color
      );
    }
  }
  return image;
}

void golf::QtHelper::DrawImage(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  const auto n_channels = source.pixelFormat().channelCount();
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    const auto line_to = target.scanLine(y + top);
    const auto line_from = source.constScanLine(y);
    std::copy(
      &line_from[0],
      &line_from[width * n_channels],
      &line_to[left * n_channels]
    );
  }
}



void golf::QtHelper::DrawImageSlow(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  const auto n_channels = source.pixelFormat().channelCount();
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    const auto line_to = target.scanLine(y + top);
    const auto line_from = source.constScanLine(y);
    for (int x=0; x!=width; ++x)
    {
      for (int c=0; c!=n_channels; ++c)
      {
        line_to[ ((left + x) * n_channels) + c]
          = line_from[ (x * n_channels) + c]
        ;
      }
    }
  }
}

void golf::QtHelper::DrawImageSlowest(
  QImage& target, const QImage& source,
  const int left, const int top
) const noexcept
{
  const int width = source.width();
  const int height = source.height();
  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      target.setPixel(left+x,top+y,source.pixel(x,y));
    }
  }
}


#ifndef NDEBUG
void golf::QtHelper::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  assert(!"TODO");
  /*
  if (verbose) { TRACE("Default-construction of QtGraphics"); }
  {
    const QtGraphics q;
  }
  //CreateImage
  {
    const QImage a = QtGraphics().CreateImage(256,256,64);
    assert(!a.isNull());
  }
  {
    QImage target = QtGraphics().CreateImage(256,256,64);
    assert(!target.isNull());
    const QImage source = QtGraphics().CreateImage(196,156,196);
    assert(!source.isNull());
    QtGraphics().DrawImage(target,source,32,64);
  }
  {
    QImage target_slowest = QtGraphics().CreateImage(256,256,64);
    QImage target_slow    = QtGraphics().CreateImage(256,256,64);
    QImage target_fast    = QtGraphics().CreateImage(256,256,64);
    const QImage source   = QtGraphics().CreateImage(196,156,196);
    QtGraphics().DrawImage(       target_fast   ,source,32,64);
    QtGraphics().DrawImageSlow   (target_slow   ,source,32,64);
    QtGraphics().DrawImageSlowest(target_slowest,source,32,64);
    assert(target_fast == target_slow);
    assert(target_fast == target_slowest);
  }
  const bool do_timing{false};
  if (do_timing)
  {
    //TRACE 't_fast' line 188 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,032000000000000001'
    //TRACE 't_slow' line 189 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '0,51700000000000002'
    //TRACE 't_slowest' line 190 in file '../../Classes/CppQtGraphics/qtgraphics.cpp': '1,169'
    const int sz{5000};
    QImage target_fast    = QtGraphics().CreateImage(sz,sz);
    QImage target_slow    = QtGraphics().CreateImage(sz,sz);
    QImage target_slowest = QtGraphics().CreateImage(sz,sz);
    const QImage source   = QtGraphics().CreateImage(sz,sz);
    Stopwatch s_fast;
    QtGraphics().DrawImage(target_fast,source,0,0);
    const double t_fast{s_fast.GetElapsedSecs()};
    Stopwatch s_slow;
    QtGraphics().DrawImageSlow(target_slow,source,0,0);
    const double t_slow{s_slow.GetElapsedSecs()};
    Stopwatch s_slowest;
    QtGraphics().DrawImageSlowest(target_slowest,source,0,0);
    const double t_slowest{s_slowest.GetElapsedSecs()};
    TRACE(t_fast);
    TRACE(t_slow);
    TRACE(t_slowest);
    assert(t_fast < t_slow);
    assert(t_slow < t_slowest);
    assert(t_fast * 10.0 < t_slow);
  }
  */
}
#endif
