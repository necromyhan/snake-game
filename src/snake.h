#ifndef __SNAKE_GAME_SNAKE_H__
#define __SNAKE_GAME_SNAKE_H__

#include "SDL_rect.h"
#include "SDL_render.h"

#include "tileset.h"

/// enum SNAKE_DIRECTION
/// Snake movement directions
typedef enum _SNAKE_DIRECTION
{
   SnakeDirectionRight, ///< Right
   SnakeDirectionLeft,  ///< Left
   SnakeDirectionUp,    ///< Up
   SnakeDirectionDown   ///< Down
} SNAKE_DIRECTION;

/// struct SNAKE
typedef struct _SNAKE
{
   int               Length;    ///< Snake body length
   SNAKE_DIRECTION   Direction; ///< Current movement direction
   SDL_FRect*        Body;      ///< Array of body rects
   bool              InputHandled;
} SNAKE;


SNAKE*
CreateSnake(
   int   CellSize,
   int   CellCount,
   int   StartCellX,
   int   StartCellY);

void
DestroySnake(
   SNAKE *Snake);

void
MoveSnake(
   SNAKE*   Snake,
   int      FieldWidth,
   int      FieldHeight);

void
GrowSnake(
   SNAKE*   Snake);

void
ReinitSnake(
   SNAKE*   Snake);

bool
IsSnakeIntersection(
   const SNAKE*   Snake);

int
RenderSnake(
   SDL_Renderer*  Renderer,
   TILESET*       Tileset,
   const SNAKE*   Snake);

#endif //__SNAKE_GAME_SNAKE_H__