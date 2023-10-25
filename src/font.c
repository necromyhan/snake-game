#include <stdbool.h>
#include <stdlib.h>

#include "font.h"


int
InitFont()
{
   int res = 0;

   do
   {
      res = TTF_Init();
      if (res)
      {
         SDL_Log("TTF_Init error!");
         break;
      }
   } while (false);
   
   return res;
}
