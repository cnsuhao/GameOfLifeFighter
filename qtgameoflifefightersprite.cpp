#include "qtgameoflifefightersprite.h"

#include <cassert>
#include <iostream>
#include <QImage>

#include "qtgameoflifefighterplayerindex.h"
#include "qtgameoflifefighterhelper.h"

golf::QtSprite::QtSprite()
{
  #ifndef NDEBUG
  Test();
  #endif
}

QImage golf::QtSprite::Create(const CellState& state) const noexcept
{
  return Create(
    state.GetSelectedBy(),
    state.GetHangarOf(),
    state.GetHeartOf(),
    state.GetIsBuilding(),
    state.GetCellType()
  );
}

QImage golf::QtSprite::Create(
  const int selected_by,   //0: no-one, 1: player1, 2: player2
  const int hangar_of,     //0: no-one, 1: player1, 2: player2
  const int heart_of,      //0: no-one, 1: player1, 2: player2
  const bool is_building,  //Has the player built something on this square?
  const CellType cell_type
) const
{
  const int width{GetWidth()};
  const int height{GetHeight()};
  QImage image(width,height,QImage::Format_RGB32);
  assert(height == 6);
  assert(width  == 6);

  //Is alive
  {
    QColor qtcolor = Qt::white;
    if (cell_type == CellType::alive)
    {
      switch (heart_of)
      {
        case 0: qtcolor = Qt::white; break;
        case 1: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player1),Qt::white); break;
        case 2: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player2),Qt::white); break;
      }
    }
    else
    {
      switch (heart_of)
      {
        case 0: qtcolor = Qt::black; break;
        case 1: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player1),Qt::black); break;
        case 2: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player2),Qt::black); break;
      }
    }
    for (int y=1;y!=6; ++y)
    {
      for (int x=1;x!=6; ++x)
      {
        image.setPixel(x,y,qtcolor.rgb());
      }
    }
  }


  //Draw mono-color selection border
  {
    QColor qtcolor1 = Qt::black;
    switch (selected_by)
    {
      case 0: qtcolor1 = Qt::black; break;
      case 1: qtcolor1 = ToColor(PlayerIndex::player1); break;
      case 2: qtcolor1 = ToColor(PlayerIndex::player2); break;
    }
    QColor qtcolor2 = Qt::black;
    switch (hangar_of)
    {
      case 0: qtcolor2 = Qt::black; break;
      case 1: qtcolor2 = Qt::lightGray; break;
      case 2: qtcolor2 = Qt::gray; break;
    }
    QColor qtcolor = QtHelper().Blend(qtcolor1,qtcolor2);
    for (int x=0; x!=width; ++x)
    {
      image.setPixel(x,0,qtcolor.rgb());
      image.setPixel(x,1,qtcolor.rgb());
      image.setPixel(x,height - 1,qtcolor.rgb());
    }
    for (int y=0; y!=height; ++y)
    {
      image.setPixel(0,y,qtcolor.rgb());
      image.setPixel(1,y,qtcolor.rgb());
      image.setPixel(width - 1,y,qtcolor.rgb());
    }
  }

  //Draw mono-color of building
  if (is_building)
  {
    QColor qtcolor = Qt::white;
    switch (selected_by)
    {
      case 0: qtcolor = Qt::darkGray; break;
      case 1: qtcolor = ToColor(PlayerIndex::player1); break;
      case 2: qtcolor = ToColor(PlayerIndex::player2); break;
    }
    image.setPixel(1,4,qtcolor.rgb());
    image.setPixel(2,2,qtcolor.rgb());
    image.setPixel(3,3,qtcolor.rgb());
    image.setPixel(4,1,qtcolor.rgb());
  }
  return image;
}


#ifndef NDEBUG
void golf::QtSprite::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }

  //Check if all pictures are different
  std::vector<QImage> v;
  for (int selected_by = 0; selected_by <= 2; ++selected_by)
    for (int hangar_of = 0; hangar_of <= 2; ++hangar_of)
      for (int heart_of = 0; heart_of <= 2; ++heart_of)
        for (bool is_building: { true, false} )
          for (CellType cell_type: { CellType::empty, CellType::alive} )
            v.push_back(QtSprite().Create(selected_by,hangar_of,heart_of,is_building,cell_type));


  const int sz{static_cast<int>(v.size())};
  for (int i=0; i!=sz; ++i)
  {
    for (int j=0; j!=sz; ++j)
    {
      if (i==j) continue;
      if (v[i] == v[j])
      {
        std::clog << i << "," << j << std::endl;
      }
      assert(v[i] != v[j]);
    }
  }
}
#endif
