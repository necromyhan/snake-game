#include <stdbool.h>
#include <stdlib.h>

#include "font.h"
#include "SDL_render.h"


int
InitFont(
   SDL_Renderer*  Renderer)
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

      TTF_Font* font = TTF_OpenFont("Mork_Dungeon.ttf", 24);
      if (NULL == font)
      {
         SDL_Log("TTF_OpenFont error = %s", SDL_GetError());
      }

      SDL_Color color = { 255, 255, 255 };
      SDL_Surface* surface = TTF_RenderText_Solid(font, "Text Text!", color);
      if (NULL == font)
      {
         SDL_Log("TTF_OpenFont error = %s", SDL_GetError());
      }

      SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
      if (NULL == texture)
      {
         SDL_Log("SDL_CreateTextureFromSurface error = %s", SDL_GetError());
      }


      int w, h;
      res = SDL_QueryTexture(texture, NULL, NULL, &w, &h);
      if (res)
      {
         SDL_Log("SDL_QueryTexture error = %s", SDL_GetError());
         break;
      }

      SDL_FRect rect = { 0, 0, w, h };


      res = SDL_RenderTexture(Renderer, texture, NULL, &rect);
      if (res)
      {
         SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
         break;
      }

      SDL_RenderPresent(Renderer);

   } while (false);
   
   return res;
}
