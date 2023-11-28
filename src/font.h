#ifndef __SNAKE_GAME_FONT_H__
#define __SNAKE_GAME_FONT_H__

#include <SDL3_ttf/SDL_ttf.h>

typedef struct _FONT FONT;

FONT*
CreateFont(
   const char* Name,
   int         Size,
   SDL_Color   Color);

void
DestroyFont(FONT* Font);

int
SetFontSize(
   FONT* Font,
   int   Size);

int
GetTextSize(
   const FONT* Font,
   const char* Text,
   int*        Width,
   int*        Height);

int
PrintFontToRenderer(
   const FONT*    Font,
   SDL_Renderer*  Renderer,
   const char*    Text,
   SDL_Color      Color,
   SDL_Point      Position);

#endif //__SNAKE_GAME_FONT_H__