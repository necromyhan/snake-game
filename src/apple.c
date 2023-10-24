#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDL_timer.h"

#include "apple.h"
#include "snake.h"


void
UpdateApplePosition(
   APPLE*   Apple,
   SNAKE*   Snake,
   int      CellSize,
   int      WidthInCells,
   int      HeightInCell)
{
   if (NULL == Apple || NULL == Snake)
   {
      return;
   }

   int time  = SDL_GetTicks();
   srand(time);

   int xCell = rand() % WidthInCells;
   int yCell = rand() % HeightInCell;

   SDL_Log("xCell = %d, yCell = %d", xCell, yCell);

   Apple->Body.x = xCell * CellSize + CellSize / 4;
   Apple->Body.y = yCell * CellSize + CellSize / 4;

   Apple->Body.h = CellSize / 2;
   Apple->Body.w = CellSize / 2;
}

bool
IsApple(
   APPLE*   Apple,
   SNAKE*   Snake)
{
   return SDL_HasRectIntersectionFloat(&Snake->Body[0], &Apple->Body);
}

int
RenderApple(
   SDL_Renderer*  Renderer,
   const APPLE*   Apple)
{
   int res = 0;

   do
   {
      if (NULL == Apple) { res = 1; break; }

      res = SDL_SetRenderDrawColor(Renderer, 200, 0, 0, 0);
      if (res) { break; }

      res = SDL_RenderFillRect(Renderer, &Apple->Body);
   } while (false);
   
   return res;
}