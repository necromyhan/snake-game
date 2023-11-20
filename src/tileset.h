#ifndef __SNAKE_TILES_H__
#define __SNAKE_TILES_H__

#include <SDL.h>
#include <SDL3_image/SDL_image.h>

typedef struct _TILESET TILESET;

enum TILE_NAME
{
   SnakeBody1Tile,
   SnakeBody2Tile,
   SnakeBody3Tile,
   SnakeHeadTile,
   Wall1Tile,
   Wall2Tile,
   FullWall1Tile,
   FullWall2Tile,
   GrassTile,
   StoneTile,
   AppleTile,
   ChickenTile
};

TILESET*
CreateTileset(
   SDL_Renderer*  Renderer,
   const char*    Path);

void DestroyTileset(
   TILESET*       Tileset);

int
RenderTile(
   SDL_Renderer*     Renderer,
   TILESET*          Tileset,
   int               TileName,
   const SDL_FRect*  Dest);

SDL_Texture*
CreateTextureFromImage(
   SDL_Renderer*  Renderer,
   const char*    Path);

#endif //__SNAKE_TILES_H__