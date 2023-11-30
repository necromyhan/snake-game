#include <stdbool.h>
#include <stdlib.h>

#include "game.h"

enum {
   MinumumCellSize = 16,
   FieldWidthInCells = 14,
   FieldHeightInCells = 10
};

Uint32 gChangeSceneEventType = (Uint32)-1;

static const char* gStartMenuString[] =
{
   "start",
   "exit"
};

static const char* gGameOverMenuString[] =
{
   "restart",
   "menu"
};

int
InitGame(
   GAME*             Game,
   const RESOLUTION* Resolution)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   int temp = Resolution->Width / (FieldWidthInCells + 1);
   temp /= MinumumCellSize;
   Game->Field.CellSize = temp * MinumumCellSize; 

   Game->Field.WidthInCells   = FieldWidthInCells;
   Game->Field.HeightInCells  = FieldHeightInCells;

   // Start menu
   Game->StartMenu = CreateMenu(2, gStartMenuString);
   if (NULL == Game->StartMenu)
   {
      status = -1;
      goto error;
   }

   // Game over menu
   Game->GameOverMenu = CreateMenu(2, gGameOverMenuString);
   if (NULL == Game->GameOverMenu)
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
                     "Snake Game",
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

   Game->Renderer = SDL_CreateRenderer(Game->Window, NULL, 0);
   if (NULL == Game->Renderer)
   {
      status = -1;
      SDL_Log("CreateRenderer Error = %s", SDL_GetError());
      goto error;
   }

   // Game->Font = CreateFont();
   Game->Tileset = CreateTileset(Game->Renderer, "assets/tileset.png");
   if (NULL == Game->Renderer)
   {
      status = -1;
      SDL_Log("CreateTileset Error!");
      goto error;
   }

   Game->Font = CreateFont(
                  "assets/font.ttf",
                  Game->Field.CellSize,
                  (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0 });

   Uint32 changeSceneEventType = SDL_RegisterEvents(1);
   if (((Uint32)-1) == changeSceneEventType)
   {
      goto error;
      status = -1;
   }
   gChangeSceneEventType = changeSceneEventType;

   Game->PreviousScore = 0;

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
      DestroyFont(Game->Font);
      Game->Font = NULL;
      Game->Apple = (APPLE){ 0 };
      Game->Field = (GAME_FIELD){ 0 };
      DestroyMenu(Game->StartMenu);
      Game->StartMenu = NULL;
      DestroyMenu(Game->GameOverMenu);
      Game->StartMenu = NULL;
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

   if (Game->Snake->InputHandled)
   {
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
         Game->Snake->InputHandled = false;
      }
   }

   exit: 
   return status;
}

int
PushUserEvent(
   Uint32   Type,
   Sint32   Code)
{
   int status = 0;

   SDL_Event myEvent;
   SDL_memset(&myEvent, 0, sizeof(myEvent));

   myEvent.type = Type;
   myEvent.user.code = Code;

   status = SDL_PushEvent(&myEvent);
   if (status != 1)
   {
      status = -1;
   }
   else
   {
      status = 0;
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

   if (IsSnakeIntersection(Game->Snake))
   {
      status = PushUserEvent(gChangeSceneEventType, StateGameOver);
      if (status)
      {
         goto exit;
      }
      Game->PreviousScore = Game->Snake->Length - 2;
      ReinitSnake(Game->Snake);
      SDL_Delay(1500);

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
   
   Game->Snake->InputHandled = true;

exit:
   return status;
}

int
RenderScore(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   int w, h;
   status = SDL_GetWindowSizeInPixels(Game->Window, &w, &h);
   if (status) { goto exit; }

   int posX = (w - Game->Field.WidthInCells * Game->Field.CellSize) / 2;
   int posY = posX + Game->Field.HeightInCells * Game->Field.CellSize + Game->Field.CellSize / 2;

   char scoreStr[16];
   int symNumber = SDL_snprintf(&scoreStr[0], 16, "Score: %d", Game->Snake->Length - 2);

   status = PrintFontToRenderer(
                  Game->Font,
                  Game->Renderer,
                  scoreStr,
                  Game->Field.CellSize / 2,
                  (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                  (SDL_Point){.x = posX, .y = posY});
   if (status) { goto exit; }

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

   int w, h;
   status = SDL_GetWindowSizeInPixels(Game->Window, &w, &h);
   if (status) { goto exit; }

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }

   status = SDL_RenderFillRect(Game->Renderer, NULL);
   if (status) { goto exit; }

   SDL_Rect fieldRect = {
               (w - Game->Field.WidthInCells * Game->Field.CellSize) / 2,
               (w - Game->Field.WidthInCells * Game->Field.CellSize) / 2,
               Game->Field.WidthInCells * Game->Field.CellSize,
               Game->Field.HeightInCells * Game->Field.CellSize };

   status = RenderFieldOutline(
                  Game->Renderer,
                  Game->Tileset,
                  &fieldRect,
                  Game->Field.CellSize);
   if (status) { goto exit; }

   status = SDL_SetRenderViewport(Game->Renderer, &fieldRect);
   if (status) { goto exit; }   

   status = RenderApple(Game->Renderer, Game->Tileset, &Game->Apple);
   if (status) { goto exit; }

   status = RenderSnake(Game->Renderer, Game->Tileset, Game->Snake);
   if (status) { goto exit; }

   status = SDL_SetRenderViewport(Game->Renderer, NULL);
   if (status) { goto exit; }

   status = RenderScore(Game);
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
