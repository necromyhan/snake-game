#pragma once

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