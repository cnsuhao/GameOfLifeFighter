#include "sfmlgameoflifefighterspritesimpl.h"

#include <cassert>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "sfmlgameoflifefighterplayerindex.h"
#include "sfmlgameoflifefighterhelper.h"
#include "gameoflifefighterhelper.h"
#include "gameoflifefightercellstate.h"

golf::SfmlSpritesImpl::SfmlSpritesImpl()
  :
    m_cell_type_textures{},
    m_hangar_of_textures{},
    m_heart_of_textures{},
    m_selected_by_textures{}
{
  #ifndef NDEBUG
  Test();
  #endif
  m_cell_type_textures.Load(CellType::empty,"../GameOfLifeFighter/Resources/Sprites/Empty.png");
  m_cell_type_textures.Load(CellType::alive,"../GameOfLifeFighter/Resources/Sprites/Alive.png");

  m_hangar_of_textures.Load(HangarOf::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_hangar_of_textures.Load(HangarOf::player1,"../GameOfLifeFighter/Resources/Sprites/HangarBlue.png");
  m_hangar_of_textures.Load(HangarOf::player2,"../GameOfLifeFighter/Resources/Sprites/HangarRed.png");

  m_heart_of_textures.Load(HeartOf::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_heart_of_textures.Load(HeartOf::player1,"../GameOfLifeFighter/Resources/Sprites/HeartBlue.png");
  m_heart_of_textures.Load(HeartOf::player2,"../GameOfLifeFighter/Resources/Sprites/HeartRed.png");

  m_selected_by_textures.Load(SelectedBy::none,"../GameOfLifeFighter/Resources/Sprites/Transparent.png");
  m_selected_by_textures.Load(SelectedBy::player1,"../GameOfLifeFighter/Resources/Sprites/SelectedByBlue.png");
  m_selected_by_textures.Load(SelectedBy::player2,"../GameOfLifeFighter/Resources/Sprites/SelectedByRed.png");
}

/*
QImage golf::SfmlSprite::Create(
  const int selected_by,   //0: no-one, 1: player1, 2: player2
  const int hangar_of,     //0: no-one, 1: player1, 2: player2
  const int heart_of,      //0: no-one, 1: player1, 2: player2
  const bool is_building,  //Has the player built something on this square?
  const CellType cell_type
) const
{
  const int width{GetWidth()};
  const int height{GetHeight()};
  QImage image(width,height,QImage::Format_ARGB32);
  assert(height == 6);
  assert(width  == 6);

  //Is alive
  {
    QColor qtcolor = Qt::black;
    if (cell_type == CellType::alive)
    {
      qtcolor = Qt::white;
      switch (heart_of)
      {
        case 0: break;
        case 1: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player1),qtcolor); break;
        case 2: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player2),qtcolor); break;
      }
    }
    else
    {
      qtcolor = Qt::black;
      switch (heart_of)
      {
        case 0: break;
        case 1: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player1),qtcolor); break;
        case 2: qtcolor = QtHelper().Blend(ToColor(PlayerIndex::player2),qtcolor); break;
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
    QColor qtcolor = Qt::black;
    switch (hangar_of)
    {
      case 0: qtcolor = Qt::black; break;
      case 1:
        qtcolor = QtHelper().Blend(qRgb(0,0,0),ToColor(PlayerIndex::player1));
        qtcolor = QtHelper().Blend(qRgb(0,0,0),qtcolor);
      break;
      //case 1: qtcolor = ToColor(PlayerIndex::player1); break;
      case 2:
        qtcolor = QtHelper().Blend(qRgb(0,0,128),ToColor(PlayerIndex::player2));
        qtcolor = QtHelper().Blend(qRgb(0,0,128),qtcolor);
        break;
      //case 2: qtcolor = ToColor(PlayerIndex::player2); break; //Blue is always less bright
    }
    for (int x=0; x!=width; ++x)
    {
      image.setPixel(x,0,qtcolor.rgb());
      //image.setPixel(x,height - 1,qtcolor.rgb());
    }
    for (int y=0; y!=height; ++y)
    {
      image.setPixel(0,y,qtcolor.rgb());
      //image.setPixel(width - 1,y,qtcolor.rgb());
    }
  }

  //Draw player
  if (selected_by != 0)
  {
    QColor qtcolor = Qt::black;
    switch (selected_by)
    {
      case 1: qtcolor = QtHelper().Blend(Qt::white,ToColor(PlayerIndex::player1)); break;
      case 2: qtcolor = QtHelper().Blend(Qt::white,ToColor(PlayerIndex::player2)); break;
      default: assert(!"Should not get here");
    }
    image.setPixel((width / 2)-0,(height / 2)-0,qtcolor.rgb());
    image.setPixel((width / 2)-0,(height / 2)-1,qtcolor.rgb());
    image.setPixel((width / 2)-1,(height / 2)-0,qtcolor.rgb());
    image.setPixel((width / 2)-1,(height / 2)-1,qtcolor.rgb());
  }

  //Draw mono-color of building
  if (is_building)
  {
    QColor qtcolor = Qt::white;
    switch (hangar_of)
    {
      case 0: qtcolor = Qt::darkGray; break;
      case 1: qtcolor = QtHelper().Blend(qtcolor,ToColor(PlayerIndex::player1)); break;
      //case 1: qtcolor = ToColor(PlayerIndex::player1); break;
      case 2: qtcolor = QtHelper().Blend(qtcolor,ToColor(PlayerIndex::player2)); break;
      //case 2: qtcolor = ToColor(PlayerIndex::player2); break;
    }
    const int offset = 0;
    for (int x=offset; x!=width-1-offset; ++x)
    {
      image.setPixel(x,offset,qtcolor.rgb());
      image.setPixel(x,height-1-offset,qtcolor.rgb());
    }
    for (int y=offset; y!=height-1-offset; ++y)
    {
      image.setPixel(offset,y,qtcolor.rgb());
      image.setPixel(width-offset-1,y,qtcolor.rgb());
    }
  }
  return image;
}
*/

sf::Texture& golf::SfmlSpritesImpl::Get(const CellType& cell_type) noexcept
{
  return m_cell_type_textures.Get(cell_type);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const HangarOf& hangar_of) noexcept
{
  return m_hangar_of_textures.Get(hangar_of);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const HeartOf& heart_of) noexcept
{
  return m_heart_of_textures.Get(heart_of);
}

sf::Texture& golf::SfmlSpritesImpl::Get(const SelectedBy& selected_by) noexcept
{
  return m_selected_by_textures.Get(selected_by);
}

#ifndef NDEBUG
void golf::SfmlSpritesImpl::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  {
    Helper();
  }
  //Create one pixmap with all pictures
  {
    SfmlSpritesImpl s;
    for (const auto cell_type: GetAllCellTypes())
    {
      assert(s.Get(cell_type).getSize().x == 6);
      assert(s.Get(cell_type).getSize().y == 6);
    }
    for (const auto hangar_of: GetAllHangarOfs())
    {
      assert(s.Get(hangar_of).getSize().x == 6);
      assert(s.Get(hangar_of).getSize().y == 6);
    }
    for (const auto heart_of: GetAllHeartOfs())
    {
      assert(s.Get(heart_of).getSize().x == 6);
      assert(s.Get(heart_of).getSize().y == 6);
    }
    for (const auto selected_by: GetAllSelectedBys())
    {
      assert(s.Get(selected_by).getSize().x == 6);
      assert(s.Get(selected_by).getSize().y == 6);
    }
  }
}
#endif
