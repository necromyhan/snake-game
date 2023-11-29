#include <stdbool.h>

#include "snake.h"


SNAKE*
CreateSnake(
   int   CellSize,
   int   CellCount,
   int   StartCellX,
   int   StartCellY)
{
   SNAKE *snake = NULL;

   snake = SDL_malloc(sizeof(*snake));
   if (NULL == snake) { goto exit; }

   snake->Body = SDL_malloc(CellCount * sizeof(*snake->Body));
   if (NULL == snake->Body)
   {
      SDL_free(snake);
      snake = NULL;
      goto exit;
   }

   SDL_memset(snake->Body, 0x00, sizeof(*snake->Body));

   snake->Body[0].h = CellSize;
   snake->Body[0].w = CellSize;
   snake->Body[0].x = StartCellX * CellSize;
   snake->Body[0].y = StartCellY * CellSize;

   snake->Body[1].h = CellSize;
   snake->Body[1].w = CellSize;
   snake->Body[1].x = (StartCellX - 1) * CellSize;
   snake->Body[1].y = StartCellY * CellSize;

   snake->Length = 2;
   snake->Direction = SnakeDirectionRight;

   snake->InputHandled = true;
exit:
   return snake;
}

void DestroySnake(SNAKE *Snake)
{
   SDL_free(Snake->Body);
   SDL_free(Snake);
}

void
MoveSnake(
   SNAKE*   Snake,
   int      FieldWidth,
   int      FieldHeight)
{
   if (NULL == Snake) { goto exit; }

   int deltaX, deltaY;
   switch (Snake->Direction)
   {
      case SnakeDirectionUp:
      {
         deltaX = 0;
         deltaY = 0 - Snake->Body[0].h;
         break;
      }
      case SnakeDirectionDown:
      {
         deltaX = 0;
         deltaY = Snake->Body[0].h;
         break;
      }
      case SnakeDirectionRight:
      {
         deltaX = Snake->Body[0].w;
         deltaY = 0;
         break;
      }
      case SnakeDirectionLeft:
      {
         deltaX = 0 - Snake->Body[0].w;
         deltaY = 0;
         break;
      }
      default:
      {
         goto exit;
      }
   }

   for (int i = Snake->Length - 1; i > 0; --i)
   {
      Snake->Body[i].x = Snake->Body[i - 1].x;
      Snake->Body[i].y = Snake->Body[i - 1].y;
   }

   Snake->Body[0].x += deltaX;
   Snake->Body[0].y += deltaY;
   Snake->Body[0].x = (int)(Snake->Body[0].x + FieldWidth) % FieldWidth;
   Snake->Body[0].y = (int)(Snake->Body[0].y + FieldHeight) % FieldHeight;

   exit: ;
}

void
GrowSnake(
   SNAKE*   Snake)
{
   Snake->Body[Snake->Length].h = Snake->Body[0].h;
   Snake->Body[Snake->Length].w = Snake->Body[0].w;
   Snake->Body[Snake->Length].x = Snake->Body[Snake->Length - 1].x;
   Snake->Body[Snake->Length].y = Snake->Body[Snake->Length - 1].y;

   Snake->Length += 1;
}

void
ReinitSnake(
   SNAKE*   Snake)
{
   if (NULL == Snake)
   {
      goto exit;
   }

   Snake->Body[0].x = 2 * Snake->Body[0].w;
   Snake->Body[0].y = 2 * Snake->Body[0].h;
   Snake->Body[1].x = 1 * Snake->Body[1].w;
   Snake->Body[1].y = 2 * Snake->Body[1].h;

   Snake->Length = 2;
   Snake->Direction = SnakeDirectionRight;

   exit: ;
}

bool
IsSnakeIntersection(
   const SNAKE*   Snake)
{
   bool intersec = false;

   for (int i = 1; i < Snake->Length; ++i)
   {
      if ((intersec = SDL_HasRectIntersectionFloat(&Snake->Body[0], &Snake->Body[i])))
      {
         break;  
      }
   }

   return intersec;
}

int
RenderSnake(
   SDL_Renderer*  Renderer,
   TILESET*       Tileset,
   const SNAKE*   Snake)
{
   int res = 0;

   if (NULL == Snake) { res = -1; goto exit; }

   for (int i = Snake->Length - 1; i > 0; --i)
   {  
      res = RenderTile(Renderer, Tileset, i % 3, &Snake->Body[i]);   
   }

   res = RenderTile(Renderer, Tileset, SnakeHeadTile, &Snake->Body[0]);

exit:
   return res;
}