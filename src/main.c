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

#define CELL_SIZE       40
#define WIDTH_IN_CELLS  20
#define HEIGHT_IN_CELLS 15

typedef struct _GAME_FIELD
{
   int         CellSize;
   int         Width;
   int         Height;
   SDL_Color   BgColor;
   SDL_Color   GridColor;
} GAME_FIELD;


SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

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


int 
CreatFieldGrid(
   SDL_Renderer*     Renderer,
   const GAME_FIELD* Field)
{
   int res = 0;

   do
   {
      if (NULL == Field)
      {
         res = 1;
         break;
      }

      // Draw grid background
      res = SDL_SetRenderDrawColor(
                           Renderer, 
                           Field->BgColor.r,
                           Field->BgColor.g,
                           Field->BgColor.b,
                           Field->BgColor.a);
      if (res) { break; }
      res = SDL_RenderClear(Renderer);
      if (res) { break; }

      // // Draw grid lines
      res = SDL_SetRenderDrawColor(
                           Renderer, 
                           Field->GridColor.r,
                           Field->GridColor.g,
                           Field->GridColor.b,
                           Field->GridColor.a);
      if (res) { break; }

      // // Vertical lines
      for (int x = Field->CellSize; x < Field->Width; x += Field->CellSize)
      {
         // res = SDL_RenderLine(Renderer, x + 2, 0, x + 2, Field->Height - 1);
         // if (res) { break; }

         // res = SDL_RenderLine(Renderer, x + 1, 0, x + 1, Field->Height - 1);
         // if (res) { break; }

         res = SDL_RenderLine(Renderer, x, 0, x, Field->Height - 1);
         if (res) { break; }
      }
      if (res) { break; }

      // // Vertical lines
      for (int y = Field->CellSize; y < Field->Height; y += Field->CellSize)
      {
         // res = SDL_RenderLine(Renderer, 0, y + 2, Field->Width - 1, y + 2);
         // if (res) { break; }

         // res = SDL_RenderLine(Renderer, 0, y + 1, Field->Width - 1, y + 1);
         // if (res) { break; }
         
         res = SDL_RenderLine(Renderer, 0, y, Field->Width - 1, y);
         if (res) { break; }
      }
      if (res) { break; }

   } while (false);
   
   return res;
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
      UpdateApplePosition(Apple, Snake, Field->CellSize, WIDTH_IN_CELLS, HEIGHT_IN_CELLS);
   }
   MoveSnake(Snake, (int)Field->Width, (int)Field->Height);
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
   const GAME_FIELD* Field,
   const SNAKE*      Snake,
   const APPLE*      Apple)
{
   int res = 0;

   do
   {
      res = CreatFieldGrid(Renderer, Field);
      if (res) { break; }
      
      res = RenderApple(Renderer, Apple);
      if (res) { break; }

      res = RenderSnake(Renderer, Snake);
      if (res) { break; }

      res = SDL_RenderPresent(Renderer);
   } while (false);
   
   return res;
}

int main()
{
   if (SdlInit())
   {
      return 1;
   }

   // Game field init
   GAME_FIELD field = { 
      CELL_SIZE,                    // Cell Size
      CELL_SIZE * WIDTH_IN_CELLS,   // Width
      CELL_SIZE * HEIGHT_IN_CELLS,  // Height
      {  0, 160,  0, 0},            // BgColor
      { 20,  20, 20, 0} };          // GridColor
   
   gWindow = SDL_CreateWindow("New window", field.Width, field.Height, 0);
   if (!gWindow)
   {
      SDL_Log("CreateWindow Error!");
      return 1;
   }

   //  SDL render faster than window creation by OS
   SDL_Delay(50);

   gRenderer = SDL_CreateRenderer(gWindow, NULL, SDL_RENDERER_PRESENTVSYNC);
   if (!gRenderer)
   {
      SDL_Log("CreateRenderer Error!");
      return 1;
   }

   SNAKE* snake = CreateSnake(
                     field.CellSize,
                     WIDTH_IN_CELLS * HEIGHT_IN_CELLS,
                     5,
                     5);
   if (NULL == snake) { return 1; }

   APPLE apple;
   InitApple(&apple, snake, field.CellSize, WIDTH_IN_CELLS, HEIGHT_IN_CELLS);

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
         Update(snake, &apple, &field);
         Render(gRenderer, &field, snake, &apple);
         inputHandled = false;
         lastFrameTime = SDL_GetTicks();
         if (IsSnakeIntersection(snake))
         {
            isRunning = false;
            SDL_Log("GAME OVER!");
         }
      }

   }

   DestroySnake(snake);
   SDL_DestroyRenderer(gRenderer);
   SDL_DestroyWindow(gWindow);
   SDL_Quit();

   return 0;
}