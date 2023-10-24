#pragma once

#include "SDL_rect.h"

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