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

   Apple->Body.x = xCell * CellSize;
   Apple->Body.y = yCell * CellSize;

   Apple->Body.h = CellSize;
   Apple->Body.w = CellSize;
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
   TILESET*       Tileset,
   const APPLE*   Apple)
{
   int res = 0;

   if (NULL == Apple) { res = -1; goto exit; }

   res = RenderTile(Renderer, Tileset, AppleTile, &Apple->Body);

exit:
   return res;
}