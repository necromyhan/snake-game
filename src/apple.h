#ifndef __SNAKE_GAME_APPLE_H__
#define __SNAKE_GAME_APPLE_H__

#include "SDL_rect.h"

#include "snake.h"
#include "tileset.h"

typedef struct _APPLE
{
   SDL_FRect   Body;
} APPLE;


void
UpdateApplePosition(
   APPLE*   Apple,
   SNAKE*   Snake,
   int      CellSize,
   int      WidthInCells,
   int      HeightInCell);


#define InitApple(Apple, Snake, CellSize, WidthInCells, HeightInCells) \
         UpdateApplePosition((Apple), (Snake), (CellSize), (WidthInCells), (HeightInCells))


bool
IsApple(
   APPLE*   Apple,
   SNAKE*   Snake);


int
RenderApple(
   SDL_Renderer*  Renderer,
   TILESET*       Tileset,
   const APPLE*   Apple);

#endif //__SNAKE_GAME_APPLE_H__