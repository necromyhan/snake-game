#ifndef __SNAKE_TILES_H__
#define __SNAKE_TILES_H__

#include <SDL.h>
#include <SDL3_image/SDL_image.h>

typedef struct _TILESET TILESET;

enum TILE_NAME
{
   SnakeBody1,
   SnakeBody2,
   SnakeBody3,
   SnakeHead,
   Wall1,
   Wall2,
   FullWall1,
   FullWall2,
   Grass,
   Stone,
   Apple,
   Chicken
};

TILESET *
CreateTileset(
   SDL_Renderer*  Renderer,
   const char*    Path);

void DestroyTileset(
   TILESET*       Tileset);

int
RenderTile(
   SDL_Renderer*  Renderer,
   TILESET*       Tileset,
   int            TileName,
   SDL_FRect*     Dest);

#endif //__SNAKE_TILES_H__