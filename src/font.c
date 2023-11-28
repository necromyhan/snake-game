#include <stdbool.h>
#include <stdlib.h>

#include "font.h"
#include "SDL_render.h"

typedef struct _FONT
{
   TTF_Font*   TtfFont;
   int         Size;
} FONT;

FONT*
CreateFont(
   const char* Name,
   int         Size,
   SDL_Color   Color)
{
   FONT* font = NULL;

   if (TTF_Init())
   {
      SDL_Log("TTF_Init error = %s", SDL_GetError());
      goto exit;
   }

   font = SDL_malloc(sizeof(*font));
   if (NULL == font)
   {
      SDL_Log("SDL_malloc error!");
      goto exit;
   }

   font->TtfFont = TTF_OpenFont(Name, Size);
   if (NULL == font->TtfFont )
   {
      SDL_Log("TTF_OpenFont error = %s", SDL_GetError());
      SDL_free(font);
      font = NULL;
      goto exit;
   }

   font->Size = Size;
   
exit:
   return font;
}

void
DestroyFont(FONT* Font)
{
   if (Font)
   {
      TTF_CloseFont(Font->TtfFont);
      SDL_free(Font);
      TTF_Quit();
   }
}

int
SetFontSize(
   FONT* Font,
   int   Size)
{
   int status = 0;

   if (NULL == Font)
   {
      status = -1;
      goto exit;
   }

   status = TTF_SetFontSize(Font->TtfFont, Size);
   if (status) { goto exit; }

   Font->Size = Size;
   
exit:
   return status;
}

int
GetTextSize(
   const FONT* Font,
   const char* Text,
   int*        Width,
   int*        Height)
{
   int status = 0;

   if (NULL == Font || NULL == Text)
   {
      status = -1;
      goto exit;
   }

   status = TTF_SizeUTF8(Font->TtfFont, Text, Width, Height);

exit:
   return status;
}


int
PrintFontToRenderer(
   const FONT*    Font,
   SDL_Renderer*  Renderer,
   const char*    Text,
   SDL_Color      Color,
   SDL_Point      Position)
{
   int status = 0;

   if (NULL == Font || NULL == Renderer)
   {
      status = -1;
      goto exit;
   }

   SDL_Surface* surface = TTF_RenderText_Solid(Font->TtfFont, Text, Color);
   if (NULL == surface)
   {
      SDL_Log("TTF_RenderText_Solid error = %s", SDL_GetError());
      status = -1;
      goto exit;
   }

   SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer, surface);
   if (NULL == texture)
   {
      SDL_Log("SDL_CreateTextureFromSurface error = %s", SDL_GetError());
      status = -1;
      goto exit;
   }

   int w, h;
   status = SDL_QueryTexture(texture, NULL, NULL, &w, &h);
   if (status)
   {
      SDL_Log("SDL_QueryTexture error = %s", SDL_GetError());
      goto exit;
   }

   // SDL_RenderTextureRotated()

   status = SDL_RenderTexture(
                     Renderer,
                     texture,
                     NULL,
                     &(SDL_FRect){ Position.x, Position.y, w, h });
   if (status)
   {
      SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
      goto exit;
   }

exit:
   SDL_DestroySurface(surface);
   SDL_DestroyTexture(texture);
   return status;
}