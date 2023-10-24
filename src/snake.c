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

   do
   {
      snake = SDL_malloc(sizeof(*snake));
      if (NULL == snake) { break; }

      snake->Body = SDL_malloc(CellCount * sizeof(*snake->Body));
      if (NULL == snake->Body)
      {
         SDL_free(snake);
         snake = NULL;
         break;
      }

      SDL_memset(snake->Body, 0x00, sizeof(*snake->Body));

      snake->Body[0].h = CellSize;
      snake->Body[0].w = CellSize;
      snake->Body[0].x = StartCellX * CellSize;
      snake->Body[0].y = StartCellY * CellSize;

      snake->Length = 1;
      snake->Direction = SnakeDirectionRight;
   } while (false);

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
            return;
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
   } while (false);
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