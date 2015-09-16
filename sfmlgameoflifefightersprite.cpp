#include "sfmlgameoflifefightersprite.h"

#include <cassert>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "sfmlgameoflifefighterplayerindex.h"
#include "sfmlgameoflifefighterhelper.h"
#include "gameoflifefighterhelper.h"
#include "gameoflifefightercellstate.h"

std::map<golf::CellState,sf::Sprite> golf::SfmlSprite::m_sprites = CreateSprites();

golf::SfmlSprite::SfmlSprite()
{
  #ifndef NDEBUG
  Test();
  #endif
}

std::map<golf::CellState,sf::Sprite> golf::SfmlSprite::CreateSprites() noexcept
{
  std::map<golf::CellState,sf::Sprite> m;

  const std::string alive_file{"../GameOfLifeFighter/Resources/Sprites/Alive.png"};
  const std::string empty_file{"../GameOfLifeFighter/Resources/Sprites/Empty.png"};
  assert(Helper().IsRegularFile(alive_file));
  assert(Helper().IsRegularFile(empty_file));

  sf::Texture texture_alive;
  texture_alive.loadFromFile(alive_file);
  sf::Texture texture_empty;
  texture_empty.loadFromFile(empty_file);

  const sf::IntRect rect(0,0,6,6);

  for (const auto cell_state: GetAllCellStates())
  {
    const auto& texture = cell_state.GetCellType() == CellType::empty ? texture_empty : texture_alive;
    sf::Sprite sprite(texture,rect);
    m.insert(std::make_pair(cell_state,sprite));
  }
  return m;
}

sf::Sprite& golf::SfmlSprite::Get(const CellState& state) noexcept
{
  assert(m_sprites.count(state) == 1);
  return m_sprites[state];
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

#ifndef NDEBUG
void golf::SfmlSprite::Test() noexcept
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
    SfmlSprite s;
    for (const auto cell_state: GetAllCellStates())
    {
      assert(s.Get(cell_state).getTexture());
    }

  }
}
#endif
