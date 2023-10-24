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
   SDL_FRect   Apple;
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
      // res = SDL_SetRenderDrawColor(
      //                      Renderer, 
      //                      Field->GridColor.r,
      //                      Field->GridColor.g,
      //                      Field->GridColor.b,
      //                      Field->GridColor.a);
      // if (res) { break; }

      // // Vertical lines
      // for (int x = Field->CellSize; x < Field->Width; x += Field->CellSize)
      // {
      //    res = SDL_RenderLine(Renderer, x, 0, x, Field->Height - 1);
      //    if (res) { break; }
      // }
      // if (res) { break; }

      // // Vertical lines
      // for (int y = Field->CellSize; y < Field->Height; y += Field->CellSize)
      // {
      //    res = SDL_RenderLine(Renderer, 0, y, Field->Width - 1, y);
      //    if (res) { break; }
      // }
      // if (res) { break; }

   } while (false);
   
   return res;
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

      res = SDL_RenderFillRects(Renderer, &Snake->Body[1], Snake->Length - 1);
      if (res) { break; }

      res = SDL_SetRenderDrawColor(Renderer, 60, 60, 60, 0);
      if (res) { break; }

      res = SDL_RenderFillRect(Renderer, &Snake->Body[0]);
   } while (false);
   
   return res;
}


bool
IsApple(
   SNAKE*      Snake,
   GAME_FIELD* Field)
{
   return SDL_HasRectIntersectionFloat(&Snake->Body[0], &Field->Apple);
}



void
Update(
   SNAKE*      Snake,
   GAME_FIELD* Field)
{
   if (IsApple(Snake, Field))
   {
      float oldX = Snake->Body[0].x;
      float oldy = Snake->Body[0].y;
      GrowSnake(Snake);
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

void
InitApple(
   GAME_FIELD* Field)
{
   int xCell = rand() % ((Field->Width / Field->CellSize) + 1);
   int yCell = rand() % ((Field->Height / Field->CellSize) + 1);

   Field->Apple.x = xCell * Field->CellSize + Field->CellSize / 4;
   Field->Apple.y = yCell * Field->CellSize + Field->CellSize / 4;

   Field->Apple.h = Field->CellSize / 2;
   Field->Apple.w = Field->CellSize / 2;
}


int Render(
   SDL_Renderer*     Renderer,
   const GAME_FIELD* Field,
   const SNAKE*      Snake)
{
   int res = 0;

   do
   {
      res = SDL_RenderClear(Renderer);
      if (res) { break; }

      res = CreatFieldGrid(Renderer, Field);
      if (res) { break; }

      res = SDL_SetRenderDrawColor(Renderer, 200, 0, 0, 0);
      if (res) { break; }

      res = SDL_RenderFillRect(Renderer, &Field->Apple);

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
      { 20,  20, 20, 0},            // GridColor
      {  0,   0,  0, 0}};           // Apple
   
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

   InitApple(&field);

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
      if (IsPeriodPassed(250, lastFrameTime))
      {
         Update(snake, &field);
         Render(gRenderer, &field, snake);
         inputHandled = false;
         lastFrameTime = SDL_GetTicks();
      }

   }

   DestroySnake(snake);
   SDL_DestroyRenderer(gRenderer);
   SDL_DestroyWindow(gWindow);
   SDL_Quit();

   return 0;
}