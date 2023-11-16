#ifndef __SNAKE_GAME_GLOBAL_H__
#define __SNAKE_GAME_GLOBAL_H__

#include <SDL.h>

#include "snake.h"
#include "apple.h"
#include "tileset.h"
#include "font.h"

typedef enum _GAME_STATE 
{
   StateMenu,
   StateGameplay,
   StateGameOver
} GAME_STATE;

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
} GAME;

int
InitGame(GAME* Game);

void
ExitGame(GAME* Game);

typedef
int
(*SCENE_HANDLE_EVENT)(
   GAME*             Game,
   const SDL_Event*  Event);

typedef
int
(*SCENE_UPDATE)(GAME* Game);

typedef
int
(*SCENE_RENDER)(GAME* Game);

typedef struct _SCENE
{
   SCENE_HANDLE_EVENT   HandleEvents;
   SCENE_UPDATE         Update;
   SCENE_RENDER         Render;
} SCENE;

extern SCENE gGameplayScene;
extern SCENE gMenuScene;
extern SCENE gGameOverScene;
#endif //__SNAKE_GAME_GLOBAL_H__