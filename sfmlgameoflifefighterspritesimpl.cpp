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
  : m_sprites{},
    m_texture_alive{},
    m_texture_empty{},
    m_texture_hangar_blue{},
    m_texture_hangar_red{}
{
  #ifndef NDEBUG
  Test();
  #endif

  //Create textures
  const std::string alive_file{"../GameOfLifeFighter/Resources/Sprites/Alive.png"};
  const std::string empty_file{"../GameOfLifeFighter/Resources/Sprites/Empty.png"};
  const std::string hangar_blue_file{"../GameOfLifeFighter/Resources/Sprites/HangarBlue.png"};
  const std::string hangar_red_file{"../GameOfLifeFighter/Resources/Sprites/HangarRed.png"};

  assert(Helper().IsRegularFile(alive_file));
  assert(Helper().IsRegularFile(empty_file));
  assert(Helper().IsRegularFile(hangar_blue_file));
  assert(Helper().IsRegularFile(hangar_red_file));

  m_texture_alive.loadFromFile(alive_file,sf::IntRect(0,0,6,6));
  m_texture_empty.loadFromFile(empty_file,sf::IntRect(0,0,6,6));
  m_texture_hangar_blue.loadFromFile(hangar_blue_file,sf::IntRect(0,0,6,6));
  m_texture_hangar_red.loadFromFile(hangar_red_file,sf::IntRect(0,0,6,6));

  //Create textures
  for (const auto cell_state: GetAllCellStates())
  {
    sf::Texture texture;

    const sf::Texture& cell_type_texture =
      ( cell_state.GetCellType() == CellType::empty ? m_texture_empty : m_texture_alive);

    texture.
    //Add hangar color
    if (cell_state.GetHangarOf() == 1)
    {
      const sf::Texture& hangar_texture = m_texture_hangar_red;
      sf::Sprite hangar_sprite(hangar_texture);
      hangar_sprite.draw(*sprite.getTexture(),sf::RenderStates());
    }
    else if (cell_state.GetHangarOf() == 2)
    {
      const sf::Texture& hangar_texture = m_texture_hangar_blue;
      sf::Sprite hangar_sprite(hangar_texture);
      hangar_sprite.draw(sprite,sf::RenderStates::Default);
    }


    //const int hash{cell_state.GetHash()};
    //sprite.setColor(sf::Color(255-hash,255-hash,255-hash,255));
    m_textures.insert(std::make_pair(cell_state,texture));
  }


  //Create sprites
  for (const auto cell_state: GetAllCellStates())
  {
    //All cells are either empty or alive
    const sf::Texture& cell_type_texture =
      ( cell_state.GetCellType() == CellType::empty ? m_texture_empty : m_texture_alive);
    sf::Sprite sprite(cell_type_texture);

    //Add hangar color
    if (cell_state.GetHangarOf() == 1)
    {
      const sf::Texture& hangar_texture = m_texture_hangar_red;
      sf::Sprite hangar_sprite(hangar_texture);
      hangar_sprite.draw(*sprite.getTexture(),sf::RenderStates());
    }
    else if (cell_state.GetHangarOf() == 2)
    {
      const sf::Texture& hangar_texture = m_texture_hangar_blue;
      sf::Sprite hangar_sprite(hangar_texture);
      hangar_sprite.draw(sprite,sf::RenderStates::Default);
    }


    //const int hash{cell_state.GetHash()};
    //sprite.setColor(sf::Color(255-hash,255-hash,255-hash,255));
    m_sprites.insert(std::make_pair(cell_state,sprite));
  }
}

sf::Sprite& golf::SfmlSpritesImpl::Get(const CellState& state) noexcept
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
    for (const auto cell_state: GetAllCellStates())
    {
      assert(s.Get(cell_state).getTexture());
    }

  }
}
#endif
