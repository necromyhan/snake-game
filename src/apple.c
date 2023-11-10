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
      goto exit;
   }

   int time  = SDL_GetTicks();
   srand(time);

   int xCell = rand() % WidthInCells;
   int yCell = rand() % HeightInCell;

   Apple->Body.x = xCell * CellSize + CellSize / 4;
   Apple->Body.y = yCell * CellSize + CellSize / 4;

   Apple->Body.h = CellSize / 2;
   Apple->Body.w = CellSize / 2;
exit: ;
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

   if (NULL == Apple) { res = -1; goto exit; }

   res = SDL_SetRenderDrawColor(Renderer, 200, 0, 0, 0);
   if (res) { goto exit; }

   res = SDL_RenderFillRect(Renderer, &Apple->Body);
   
exit:
   return res;
}