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

   Game->Field.CellSize       = 64;
   Game->Field.WidthInCells   = 12;
   Game->Field.HeightInCells  = 9;

   Game->Menu = CreateMenu();
   if (NULL == Game->Menu)
   {
      status = -1;
      goto error;
   }

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

   Game->Font = CreateFont(
                  "kongtext.regular.ttf",
                  Game->Field.CellSize,
                  (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0 });

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
      DestroyMenu(Game->Menu);
      Game->Menu = NULL;
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

int
GameplayHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   int status = 0;
   if (NULL == Event || NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      switch (Event->key.keysym.sym)
      {
         case SDLK_UP:
         {
            if (Game->Snake->Direction != SnakeDirectionUp
                && Game->Snake->Direction != SnakeDirectionDown)
            {
               Game->Snake->Direction = SnakeDirectionUp;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->Snake->Direction != SnakeDirectionUp
                 && Game->Snake->Direction != SnakeDirectionDown)
            {
               Game->Snake->Direction = SnakeDirectionDown;
            }
         }
         break;
         case SDLK_RIGHT:
         {
            if (Game->Snake->Direction != SnakeDirectionLeft
                 && Game->Snake->Direction != SnakeDirectionRight)
            {
               Game->Snake->Direction = SnakeDirectionRight;
            }
         }
         break;
         case SDLK_LEFT:
         {
            if (Game->Snake->Direction != SnakeDirectionRight
                 && Game->Snake->Direction != SnakeDirectionLeft)
            {
               Game->Snake->Direction = SnakeDirectionLeft;
            }
         }
         break;
      }
   }

   exit: 
   return status;
}

static
int
GameplayUpdate(GAME*  Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (IsApple(&Game->Apple, Game->Snake))
   {
      float oldX = Game->Snake->Body[0].x;
      float oldy = Game->Snake->Body[0].y;
      GrowSnake(Game->Snake);
      UpdateApplePosition(
               &Game->Apple,
               Game->Snake,
               Game->Field.CellSize,
               Game->Field.WidthInCells,
               Game->Field.HeightInCells);
   }

   MoveSnake(Game->Snake,
               Game->Field.WidthInCells * Game->Field.CellSize,
               Game->Field.HeightInCells * Game->Field.CellSize);

exit:
   return status;
}

static
int
GameplayRender(GAME*  Game)
{
   int status = 0;
   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   // SDL_SetRenderCliхаpRect(Renderer, &(SDL_Rect){ .x = 0, .y = 0 , .w = 400, .h = 400});
   // SDL_RenderTexture();
   SDL_Rect fieldRect = {
               Game->Field.CellSize,
               Game->Field.CellSize,
               Game->Field.WidthInCells * Game->Field.CellSize,
               Game->Field.HeightInCells * Game->Field.CellSize };

   status = SDL_SetRenderDrawColor(Game->Renderer, 255, 255, 255, 0);
   if (status) { goto exit; }

   SDL_FPoint points[] = { 
         { Game->Field.CellSize - 1, Game->Field.CellSize - 1 },
         { (Game->Field.WidthInCells + 1) * Game->Field.CellSize, Game->Field.CellSize - 1 },
         { (Game->Field.WidthInCells + 1) * Game->Field.CellSize, (Game->Field.HeightInCells + 1) * Game->Field.CellSize},
         { Game->Field.CellSize - 1, (Game->Field.HeightInCells + 1) * Game->Field.CellSize},
         { Game->Field.CellSize - 1, Game->Field.CellSize - 1 } };
   status = SDL_RenderLines(Game->Renderer, points, 5);
   if (status) { goto exit; }

   status = SDL_SetRenderViewport(Game->Renderer, &fieldRect);
   if (status) { goto exit; }

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }

   status = SDL_RenderFillRect(
               Game->Renderer,
               &(SDL_FRect){ 
                  0, 0, 
                  Game->Field.WidthInCells * Game->Field.CellSize,
                  Game->Field.HeightInCells * Game->Field.CellSize});
   if (status) { goto exit; }

   status = RenderApple(Game->Renderer, Game->Tileset, &Game->Apple);
   if (status) { goto exit; }

   status = RenderSnake(Game->Renderer, Game->Tileset, Game->Snake);
   if (status) { goto exit; }

   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}



SCENE gGameplayScene = 
{
   .HandleEvents  = GameplayHandleEvents,
   .Render        = GameplayRender,
   .Update        = GameplayUpdate    
};



int
GameOverHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   return -1;
}

int
GameOverUpdate(GAME* Game)
{
   return -1;
}

int
GameOverRender(GAME* Game)
{
   return -1;
}

SCENE gGameOverScene = 
{
   .HandleEvents  = GameOverHandleEvents,
   .Render        = GameOverRender,
   .Update        = GameOverUpdate    
};