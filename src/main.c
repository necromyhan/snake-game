#include <stdbool.h>

#include <SDL.h>
#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "SDL_video.h"

#define CELL_SIZE       30
#define WIDTH_IN_CELLS  27
#define HEIGHT_IN_CELLS 20

typedef struct _GAME_FIELD
{
   int         CellSize;
   int         Width;
   int         Height;
   SDL_Color   BgColor;
   SDL_Color   GridColor;
} GAME_FIELD;


typedef enum _SNAKE_DIRECTION
{
   SnakeDirectionRight = 0,
   SnakeDirectionLeft,
   SnakeDirectionUp,
   SnakeDirectionDown
} SNAKE_DIRECTION;

typedef struct _SNAKE
{
   int               Length;
   SNAKE_DIRECTION   Direction;
   SDL_FRect*        Body;
} SNAKE;


SDL_FRect rect = { 0 };

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
DrawFieldGrid(
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

      // Draw grid lines
      res = SDL_SetRenderDrawColor(
                           Renderer, 
                           Field->GridColor.r,
                           Field->GridColor.g,
                           Field->GridColor.b,
                           Field->GridColor.a);
      if (res) { break; }

      // Vertical lines
      for (int x = Field->CellSize; x < Field->Width; x += Field->CellSize)
      {
         res = SDL_RenderLine(Renderer, x, 0, x, Field->Height);
         if (res) { break; }
      }
      if (res) { break; }

      // Vertical lines
      for (int y = Field->CellSize; y < Field->Height; y += Field->CellSize)
      {
         res = SDL_RenderLine(Renderer, 0, y, Field->Width, y);
         if (res) { break; }
      }
      if (res) { break; }

      res = SDL_RenderPresent(Renderer);

   } while (false);
   
   return res;
}

SNAKE*
CreateSnake(const GAME_FIELD* Field)
{
   SNAKE* snake = NULL;

   do
   {
      if (NULL == Field) { break; }

      snake = SDL_malloc(sizeof(*snake));
      if (NULL == snake) { break; }

      int cellNumber = (Field->Height / Field->CellSize) * (Field->Width / Field->CellSize);
      snake->Body = SDL_malloc(cellNumber * sizeof(*snake->Body));
      if (NULL == snake->Body)
      {
         SDL_free(snake);
         snake = NULL;
         break;
      }

      SDL_Log("CcellNumber = %d", cellNumber);
      SDL_Log("sizeof(*snake->Body) = %d", (int)sizeof(*snake->Body));
   
      snake->Length = 1;
      snake->Direction = SnakeDirectionRight;

      snake->Body[0].h = Field->CellSize;
      snake->Body[0].w = Field->CellSize;
      snake->Body[0].x = 0;
      snake->Body[0].y = 0;

   } while (false);

   return snake;
}

void
DestroySnake(SNAKE* Snake)
{
   SDL_free(Snake->Body);
   SDL_free(Snake);
}

int
RenderSnake(
   SDL_Renderer*  Renderer,
   const SNAKE*   Snake)
{
   int res = 0;

   do
   {
      if (NULL == Snake) { res = 1; break; }

      res = SDL_SetRenderDrawColor(Renderer, 100, 100, 100, 0);
      if (res) { break; }

      res = SDL_RenderFillRects(Renderer, Snake->Body, Snake->Length);
      if (res) { break; }

      res = SDL_RenderPresent(Renderer);
   } while (false);
   
   return res;
}

int
MoveSnake(SNAKE* Snake)
{
   int res = 0;

   do
   {
      if (NULL == Snake) { res = 1; break; }

      int deltaX, deltaY;
      switch (Snake->Direction)
      {
         case SnakeDirectionUp:
         {
            deltaX = 0;
            deltaY = Snake->Body[0].h;
         }
         break;
         case SnakeDirectionDown:
         {
            deltaX = 0;
            deltaY = 0 - Snake->Body[0].h;
         }
         break;
         case SnakeDirectionRight:
         {
            deltaX = Snake->Body[0].w;
            deltaY = 0;
         }
         break;
         case SnakeDirectionLeft:
         {
            deltaX = 0 - Snake->Body[0].w;
            deltaY = 0;
         }
         break;

         default:
         return 1;
      }

      Snake->Body[0].x += deltaX;
      Snake->Body[0].y += deltaY;

      for (int i = 1; i < Snake->Length; ++i)
      {
         Snake->Body[i].x = Snake->Body[i - 1].x;
         Snake->Body[i].y = Snake->Body[i - 1].y;
      }

   } while (false);
   
   return res;
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
      { 20,  20, 20, 0}};           // GridColor
   
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

   if (DrawFieldGrid(gRenderer, &field))
   {
      SDL_Log("DrawFieldGrid Error!");
      return 1;
   }

   SNAKE* snake = CreateSnake(&field);
   if (NULL == snake) { return 1; }

   if (RenderSnake(gRenderer, snake))
   {
      SDL_Log("RenderSnake Error!");
      return 1;
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

      // WaitTime(300, lastFrameTime);
      if (IsPeriodPassed(250, lastFrameTime))
      {
         MoveSnake(snake);
         RenderSnake(gRenderer, snake);
         lastFrameTime = SDL_GetTicks();
      }

   }

   DestroySnake(snake);
   SDL_DestroyRenderer(gRenderer);
   SDL_DestroyWindow(gWindow);
   SDL_Quit();

   return 0;
}