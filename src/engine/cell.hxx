#include <vector>
#include <map>

#include "SDL2/SDL.h"


#include "basics/point.hxx";
#include "sprite.hxx";

#ifndef CELL_HXX_
#define CELL_HXX_


class Cell
{
public:
  Cell();
  Cell(Point isoCoordinates, Sprite* sprite, SDL_Renderer* renderer, SDL_Window* window);
  ~Cell();

  void setSprite(Sprite* sprite);
  Sprite* getSprite();

  void renderCell(Point cameraOffset, float zoom);

  void setNeighbors(std::vector<Cell*> neighbors);
  void drawSurroundingTiles(Point isoCoordinates);
  void determineTile();

  void increaseHeight(int height);



private:
  Point _coordinates;
  Sprite* _sprite;

  std::vector<Cell*> _neighbors;
  SDL_Renderer* _renderer;
  SDL_Window* _window;

  bool hasElevatedNeighbors();


  int _tileID;

  // Bitmask for neighbor positions
  // [ T B L R ]
  // [ 0 0 0 0 ]

  // Our neighbor tiles look like this
  // 2 5 8  
  // 1 X 7
  // 0 3 6

  enum positions : unsigned int{
       TOP = 0x1,
       BOTTOM = 0x2,
       LEFT = 0x4,
       RIGHT = 0x8,
       TOP_LEFT = 0x10,
       TOP_RIGHT = 0x20,
       BOTTOM_LEFT = 0x40,
       BOTTOM_RIGHT = 0x80,
  };

  unsigned int _position;


  // Map elvated tiles to tile ID.
  // not found means we can't draw a keytile, so elevate this tile!
  std::map<unsigned int, int> tileIdToPosition =
  {
    {TOP, 3},
    {BOTTOM, 5},
    {RIGHT, 1},
    {LEFT, 7},
    {BOTTOM_RIGHT, 2},
    {BOTTOM_LEFT, 8},
    {TOP_RIGHT, 0},
    {TOP_LEFT, 6},

    // correct
    {TOP_LEFT | TOP_RIGHT, 3},
    {(TOP_LEFT | TOP), 3},
    {(TOP_RIGHT | TOP), 3},

    {BOTTOM_LEFT | BOTTOM_RIGHT, 5},
    {BOTTOM_LEFT | BOTTOM, 5},
    {BOTTOM_RIGHT | BOTTOM, 5},
    
    {(BOTTOM_LEFT | TOP_LEFT), 7},
    {(BOTTOM_LEFT | LEFT), 7},
    {(TOP_LEFT | LEFT), 7},

    {BOTTOM_RIGHT | TOP_RIGHT, 1},
    {BOTTOM_RIGHT | RIGHT, 1},
    {TOP_RIGHT | RIGHT, 1},

    {TOP | TOP_LEFT | LEFT, 9},
    {TOP |TOP_RIGHT | RIGHT, 10},
    {BOTTOM |BOTTOM_RIGHT | RIGHT, 2},
    {BOTTOM |BOTTOM_LEFT | LEFT, 11},


  };


};


#endif