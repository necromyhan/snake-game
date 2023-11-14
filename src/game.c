#include <stdbool.h>
#include <stdlib.h>

#include "game.h"

int
InitGame(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   Game->State = StateMenu;

   Game->Field.CellSize       = 64;
   Game->Field.WidthInCells   = 12;
   Game->Field.HeightInCells  = 9;

   Game->Snake = CreateSnake(
                  Game->Field.CellSize,
                  Game->Field.WidthInCells * Game->Field.HeightInCells,
                  2,
                  2);
   if (NULL == Game->Snake)
   {
      status = -1;
      goto error;
   }

   InitApple(
      &Game->Apple, 
      Game->Snake,
      Game->Field.CellSize,
      Game->Field.WidthInCells,
      Game->Field.HeightInCells);

   Game->Window = SDL_CreateWindow(
                  "SNAKE",
                  Game->Field.CellSize * Game->Field.WidthInCells + 2 * Game->Field.CellSize,
                  Game->Field.CellSize * Game->Field.HeightInCells + 4 * Game->Field.CellSize,
                  0);
   if (NULL == Game->Window)
   {
      status = -1;
      SDL_Log("CreateWindow Error = %s", SDL_GetError());
      goto error;
   }

   //  SDL render faster than window creation by OS
   SDL_Delay(50);

   Game->Renderer = SDL_CreateRenderer(Game->Window, NULL, SDL_RENDERER_ACCELERATED);
   if (NULL == Game->Renderer)
   {
      status = -1;
      SDL_Log("CreateRenderer Error = %s", SDL_GetError());
      goto error;
   }

   // Game->Font = CreateFont();
   Game->Tileset = CreateTileset(Game->Renderer, "snake_tile_sheet.png");
   if (NULL == Game->Renderer)
   {
      status = -1;
      SDL_Log("CreateTileset Error!");
      goto error;
   }

   goto exit;

error:
   ExitGame(Game);

exit:
   return status;
}

void
ExitGame(GAME* Game)
{
   if (Game)
   {
      // DestroyFont(Game->Font);
      // Game->Font = NULL;
      Game->Apple = (APPLE){ 0 };
      Game->Field = (GAME_FIELD){ 0 };
      DestroySnake(Game->Snake);
      Game->Snake = NULL;
      DestroyTileset(Game->Tileset);
      Game->Tileset = NULL;
      SDL_DestroyRenderer(Game->Renderer);
      Game->Renderer = NULL;
      SDL_DestroyWindow(Game->Window);
      Game->Window = NULL;
   }
}