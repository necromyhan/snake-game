#include <stdbool.h>
#include <stdlib.h>

#include "field.h"


GAME_FIELD*
CreateGameField(
   int CellSize,
   int WidthInCells,
   int HeightInCells)
{
   GAME_FIELD* field = NULL;

   field = SDL_malloc(sizeof(GAME_FIELD));
   if (NULL == field)
   {
      goto exit;
   }

   field->CellSize = CellSize;
   field->WidthInCells = WidthInCells;
   field->HeightInCells = HeightInCells;

exit:

   return field;
}