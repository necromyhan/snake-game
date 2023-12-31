#ifndef __SNAKE_GAME_GLOBAL_H__
#define __SNAKE_GAME_GLOBAL_H__

#include <SDL.h>

#include "snake.h"
#include "apple.h"
#include "menu.h"
#include "tileset.h"
#include "font.h"

extern Uint32 gChangeSceneEventType;

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

typedef struct _RESOLUTION
{
   int Width;
   int Height;
} RESOLUTION;

typedef struct _GAME
{
   SDL_Renderer*  Renderer;
   SDL_Window*    Window;
   SNAKE*         Snake;
   TILESET*       Tileset;
   FONT*          Font;
   MENU*          StartMenu;
   MENU*          GameOverMenu;
   GAME_FIELD     Field;
   APPLE          Apple;
   int            PreviousScore;
} GAME;


int
InitGame(
   GAME*             Game,
   const RESOLUTION* Resolution);

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

int
PushUserEvent(
   Uint32   Type,
   Sint32   Code);

#endif //__SNAKE_GAME_GLOBAL_H__