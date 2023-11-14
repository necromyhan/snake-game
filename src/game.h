#ifndef __SNAKE_GAME_GLOBAL_H__
#define __SNAKE_GAME_GLOBAL_H__

#include <SDL.h>

#include "snake.h"
#include "apple.h"
#include "tileset.h"
#include "font.h"

enum GAME_STATE 
{
   StateMenu,
   StateGameplay,
   StateGameOver
};

typedef struct _GAME_FIELD
{
   int CellSize;
   int Width;
   int Height;
   int WidthInCells;
   int HeightInCells;
} GAME_FIELD;

typedef struct _GAME
{
   SDL_Renderer*  Renderer;
   SDL_Window*    Window;
   SNAKE*         Snake;
   TILESET*       Tileset;
   FONT*          Font;
   GAME_FIELD     Field;
   APPLE          Apple;
   int            State;
} GAME;

int
InitGame(GAME* Game);

void
ExitGame(GAME* Game);

#endif //__SNAKE_GAME_GLOBAL_H__