#pragma once
#include "SDL_rect.h"

#include "snake.h"

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
   const APPLE*   Apple);