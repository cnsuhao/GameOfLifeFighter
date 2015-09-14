#include "qtgameoflifefighterhelper.h"

#include <cassert>

#include <QColor>

#include "gameoflifefightertrace.h"

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
  //Qt4:
  //const auto n_channels = source.pixelFormat().channelCount();

  const auto n_channels = 4;

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
  //Qt4:
  //const auto n_channels = source.pixelFormat().channelCount();

  const auto n_channels = 4;
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
  const bool verbose{false};
  if (verbose) { TRACE("Default-construction of QtHelper"); }
  {
    const QtHelper q;
  }
  //CreateImage
  {
    const QImage a = QtHelper().CreateImage(256,256,64);
    assert(!a.isNull());
  }
  {
    QImage target = QtHelper().CreateImage(256,256,64);
    assert(!target.isNull());
    const QImage source = QtHelper().CreateImage(196,156,196);
    assert(!source.isNull());
    QtHelper().DrawImage(target,source,32,64);
  }
  {
    QImage target_slowest = QtHelper().CreateImage(256,256,64);
    QImage target_slow    = QtHelper().CreateImage(256,256,64);
    QImage target_fast    = QtHelper().CreateImage(256,256,64);
    const QImage source   = QtHelper().CreateImage(196,156,196);
    QtHelper().DrawImage(       target_fast   ,source,32,64);
    QtHelper().DrawImageSlow   (target_slow   ,source,32,64);
    QtHelper().DrawImageSlowest(target_slowest,source,32,64);
    assert(target_fast == target_slow);
    assert(target_fast == target_slowest);
  }
}
#endif
