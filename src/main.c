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
#include "field.h"


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
Update(
   SNAKE*      Snake,
   APPLE*      Apple,
   GAME_FIELD* Field)
{
   if (IsApple(Apple, Snake))
   {
      float oldX = Snake->Body[0].x;
      float oldy = Snake->Body[0].y;
      GrowSnake(Snake);
      UpdateApplePosition(Apple, Snake, Field->CellSize, Field->WidthInCells, Field->HeightInCells);
   }
   MoveSnake(Snake, Field->WidthInCells * Field->CellSize, Field->HeightInCells * Field->CellSize);
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


int Render(
   SDL_Renderer*     Renderer,
   TILESET*          Tileset,
   const GAME_FIELD* Field,
   const SNAKE*      Snake,
   const APPLE*      Apple)
{
   int res = 0;

   // SDL_SetRenderCliхаpRect(Renderer, &(SDL_Rect){ .x = 0, .y = 0 , .w = 400, .h = 400});
   // SDL_RenderTexture();
   SDL_Rect fieldRect = {
               Field->CellSize,
               Field->CellSize,
               Field->WidthInCells * Field->CellSize,
               Field->HeightInCells * Field->CellSize };

   res = SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 0);
   if (res) { goto exit; }

   SDL_FPoint points[] = { 
         { Field->CellSize - 1, Field->CellSize - 1 },
         { (Field->WidthInCells + 1) * Field->CellSize, Field->CellSize - 1 },
         { (Field->WidthInCells + 1) * Field->CellSize, (Field->HeightInCells + 1) * Field->CellSize},
         { Field->CellSize - 1, (Field->HeightInCells + 1) * Field->CellSize},
         { Field->CellSize - 1, Field->CellSize - 1 } };
   res = SDL_RenderLines(Renderer, points, 5);
   if (res) { goto exit; }

   res = SDL_SetRenderViewport(Renderer, &fieldRect);
   if (res) { goto exit; }

   res = SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
   if (res) { goto exit; }

   res = SDL_RenderFillRect(
               Renderer,
               &(SDL_FRect){ 
                  0, 0, 
                  Field->WidthInCells * Field->CellSize,
                  Field->HeightInCells * Field->CellSize});
   if (res) { goto exit; }

   res = RenderApple(Renderer, Tileset, Apple);
   if (res) { goto exit; }

   res = RenderSnake(Renderer, Tileset, Snake);
   if (res) { goto exit; }

   res = SDL_RenderPresent(Renderer);
   
exit:
   return res;
}

int main()
{
   if (SdlInit())
   {
      goto exit;
   }

   // Game field init
   GAME_FIELD* field = CreateGameField(40, 16, 12);
   if (NULL == field) { goto exit; }
   

   SDL_Window* window = SDL_CreateWindow(
                     "SNAKE",
                     field->CellSize * field->WidthInCells + 2 * field->CellSize,
                     field->CellSize * field->HeightInCells + 4 * field->CellSize,
                     0);
   if (!window)
   {
      SDL_Log("CreateWindow Error!");
      goto exit;
   }

   //  SDL render faster than window creation by OS
   SDL_Delay(50);

   SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_PRESENTVSYNC);
   if (!renderer)
   {
      SDL_Log("CreateRenderer Error!");
      goto exit;
   }


   TILESET* tileset = CreateTileset(renderer, "snake_tile_sheet.png");
   if (NULL == tileset)
   {
      SDL_Log("CreateTileset Error!");
      goto exit;
   }

   SNAKE* snake = CreateSnake(
                     field->CellSize,
                     field->WidthInCells * field->HeightInCells,
                     5,
                     5);
   if (NULL == snake) { return 1; }


   APPLE apple;
   InitApple(&apple, snake, field->CellSize, field->WidthInCells, field->HeightInCells);

   bool isRunning = true;
   bool inputHandled = false;
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
         else if (event.type == SDL_EVENT_KEY_DOWN)
         {
            if (!inputHandled)
            {
               switch (event.key.keysym.sym)
               {
                  case SDLK_UP:
                  {
                     if (snake->Direction != SnakeDirectionUp
                        && snake->Direction != SnakeDirectionDown)
                     {
                        snake->Direction = SnakeDirectionUp;
                        inputHandled = true;
                     }
                  }
                  break;
                  case SDLK_DOWN:
                  {
                     if (snake->Direction != SnakeDirectionUp
                        && snake->Direction != SnakeDirectionDown)
                     {
                        snake->Direction = SnakeDirectionDown;
                        inputHandled = true;
                     }
                  }
                  break;
                  case SDLK_RIGHT:
                  {
                     if (snake->Direction != SnakeDirectionLeft
                        && snake->Direction != SnakeDirectionRight)
                     {
                        snake->Direction = SnakeDirectionRight;
                        inputHandled = true;
                     }
                  }
                  break;
                  case SDLK_LEFT:
                  {
                     if (snake->Direction != SnakeDirectionRight
                        && snake->Direction != SnakeDirectionLeft)
                     {
                        snake->Direction = SnakeDirectionLeft;
                        inputHandled = true;
                     }
                  }
                  break;
               }
            }
            inputHandled = true;
         }
      }

      // WaitTime(300, lastFrameTime);
      if (IsPeriodPassed(200, lastFrameTime))
      {
         Update(snake, &apple, field);
         Render(renderer, tileset, field, snake, &apple);
         inputHandled = false;
         lastFrameTime = SDL_GetTicks();
         if (IsSnakeIntersection(snake))
         {
            isRunning = false;
            SDL_Log("GAME OVER!");
         }
      }

   }


exit:
   DestroySnake(snake);
   DestroyTileset(tileset);
   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}