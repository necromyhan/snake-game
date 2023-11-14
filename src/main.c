#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "SDL_video.h"

#include "snake.h"
#include "apple.h"
#include "font.h"
#include "game.h"

bool gInputHandled = false;

int SdlInit()
{
   if (SDL_InitSubSystem(SDL_INIT_VIDEO))
   {
      SDL_Log("SDL_InitSubSystem VIDEO Error!");
      SDL_Log("%s", SDL_GetError());
      return 1;
   }

   if (SDL_Init(SDL_INIT_EVERYTHING))
   {
      SDL_Log("SDL_Init Error!");
      SDL_Log("%s", SDL_GetError());
      return 1;
   }

   return 0;
}


void
Update(GAME* Game)
{
   if (NULL == Game)
   {
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

exit: ;
}

bool
IsPeriodPassed(
   int  Period,
   int  LastUpdateTime)
{
   int currentTime = SDL_GetTicks();
   if ((currentTime - LastUpdateTime) >= Period)
   {
      return true;
   }

   return false;
}


int Render(GAME* Game)
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

void
HandleGameEvent(
   GAME*             Game,
   const SDL_Event*  Event)
{
   if (NULL == Event || NULL == Game)
   {
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      if (!gInputHandled)
      {
         switch (Event->key.keysym.sym)
         {
            case SDLK_UP:
            {
               if (Game->Snake->Direction != SnakeDirectionUp
                   && Game->Snake->Direction != SnakeDirectionDown)
               {
                  Game->Snake->Direction = SnakeDirectionUp;
                  gInputHandled = true;
               }
            }
            break;
            case SDLK_DOWN:
            {
               if (Game->Snake->Direction != SnakeDirectionUp
                   && Game->Snake->Direction != SnakeDirectionDown)
               {
                  Game->Snake->Direction = SnakeDirectionDown;
                  gInputHandled = true;
               }
            }
            break;
            case SDLK_RIGHT:
            {
               if (Game->Snake->Direction != SnakeDirectionLeft
                   && Game->Snake->Direction != SnakeDirectionRight)
               {
                  Game->Snake->Direction = SnakeDirectionRight;
                  gInputHandled = true;
               }
            }
            break;
            case SDLK_LEFT:
            {
               if (Game->Snake->Direction != SnakeDirectionRight
                     && Game->Snake->Direction != SnakeDirectionLeft)
               {
                  Game->Snake->Direction = SnakeDirectionLeft;
                  gInputHandled = true;
               }
            }
            break;
         }
      }
      gInputHandled = true;
   }

   exit: ;
}


int main()
{
   if (SdlInit())
   {
      goto exit;
   }

   GAME game;
   if (InitGame(&game))
   {
      goto exit;
   }

   bool isRunning = true;
   int lastFrameTime = 0;
   SDL_Event event;
   while (isRunning)
   {
      if (SDL_PollEvent(&event))
      {
         if (event.type == SDL_EVENT_QUIT)
         {
            isRunning = false;
         }
      }
      HandleGameEvent(&game, &event);

      // WaitTime(300, lastFrameTime);
      if (IsPeriodPassed(200, lastFrameTime))
      {
         Update(&game);
         Render(&game);
         gInputHandled = false;
         lastFrameTime = SDL_GetTicks();
         if (IsSnakeIntersection(game.Snake))
         {
            isRunning = false;
            SDL_Log("GAME OVER!");
         }
      }

   }

exit:
   SDL_Quit();

   return 0;
}