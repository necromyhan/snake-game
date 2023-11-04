#ifndef __SNAKE_GAME_FIELD_H__
#define __SNAKE_GAME_FIELD_H__

#include <SDL.h>

typedef struct _GAME_FIELD
{
   int         CellSize;
   int         WidthInCells;
   int         HeightInCells;
} GAME_FIELD;

GAME_FIELD*
CreateGameField(
   int CellSize,
   int WidthInCells,
   int HeightInCells);

#endif //__SNAKE_GAME_FIELD_H__