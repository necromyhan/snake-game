#include <stdbool.h>
#include <stdlib.h>

#include "tileset.h"

enum
{
   TileSize = 16
};

typedef struct _TILESET
{
   SDL_Texture*   Texture;
   int            Width;
   int            Height;
   SDL_Texture*   Title;    
} TILESET;

TILESET*
CreateTileset(
   SDL_Renderer*  Renderer,
   const char*    Path)
{
   TILESET* tileset = NULL;

   if (NULL == Renderer || NULL == Path) { goto exit; }

   if (IMG_INIT_PNG != IMG_Init(IMG_INIT_PNG))
   {
      SDL_Log("IMG_Init error = %s", SDL_GetError());
      goto exit;
   }

   SDL_Surface* tsSurface = IMG_Load(Path);
   if (NULL == tsSurface) 
   { 
      SDL_Log("IMG_Load error = %s", SDL_GetError());
      goto error;
   }

   SDL_Texture* tsTexture = SDL_CreateTextureFromSurface(Renderer, tsSurface);
   if (NULL == tsTexture) 
   { 
      SDL_Log("SDL_CreateTextureFromSurface error = %s", SDL_GetError());
      goto error;
   }

   int w, h;
   if (SDL_QueryTexture(tsTexture, NULL, NULL, &w, &h))
   {
      SDL_Log("SDL_QueryTexture error = %s", SDL_GetError());
      goto error;
   }

   tileset = SDL_malloc(sizeof(TILESET));
   if (NULL == tileset)
   {
      SDL_Log("SDL_malloc error = %s", SDL_GetError());
      goto error;
   }

   tileset->Texture = tsTexture;
   tileset->Height = h;
   tileset->Width = w;
   goto exit;

error:
   IMG_Quit();
   SDL_DestroyTexture(tsTexture);
   SDL_free(tileset);

exit:
   SDL_DestroySurface(tsSurface);
   return tileset;
}

void
DestroyTileset(
   TILESET* Tileset)
{
   if (Tileset)
   {
      SDL_DestroyTexture(Tileset->Texture);
      SDL_free(Tileset);
      IMG_Quit();
   }
}

int
RenderTile(
   SDL_Renderer*     Renderer,
   TILESET*          Tileset,
   int               TileName,
   const SDL_FRect*  Dest)
{
   int status = 0;

   if (NULL == Renderer || NULL == Tileset)
   {
      status = -1;
      goto exit;
   }

   SDL_FRect tile;
   tile.x = (TileName * TileSize) % Tileset->Width;
   tile.y = ((TileName * TileSize) / Tileset->Width) * TileSize;
   tile.h = tile.w = TileSize;

   status = SDL_RenderTexture(
                     Renderer,
                     Tileset->Texture,
                     &tile,
                     Dest);
   if (status)
   {
      SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
   }


exit:
   return status;
}

SDL_Texture*
CreateTextureFromImage(
   SDL_Renderer*  Renderer,
   const char*    Path)
{
   SDL_Texture* texture = NULL;

   if (NULL == Renderer || NULL == Path)
   {
      goto exit;
   }

   SDL_Surface* tsSurface = IMG_Load(Path);
   if (NULL == tsSurface) 
   { 
      SDL_Log("IMG_Load error = %s", SDL_GetError());
      goto exit;
   }

   texture = SDL_CreateTextureFromSurface(Renderer, tsSurface);
   if (NULL == texture) 
   { 
      SDL_Log("SDL_CreateTextureFromSurface error = %s", SDL_GetError());
      goto exit;
   }
   
exit:
   SDL_DestroySurface(tsSurface);
   return texture;
}

int
RenderFieldOutline(
   SDL_Renderer*  Renderer,
   TILESET*       Tileset,
   SDL_Rect*      Outline,
   int            CellSize)
{
   int status = 0;

   if (NULL == Renderer || NULL == Tileset)
   {
      status = -1;
      goto exit;
   }

   SDL_FRect tile;

   tile.x = 0;
   tile.y = 48;
   tile.w = 1;
   tile.h = 16;

   SDL_FRect dest = { 0 };

   for (int i = 0; i < Outline->h; i += CellSize)
   {
      dest.w = CellSize / 16;
      dest.h = CellSize;
      dest.x = Outline->x - dest.w;
      dest.y = Outline->y + i;
      status = SDL_RenderTexture(
                     Renderer,
                     Tileset->Texture,
                     &tile,
                     &dest);
      if (status)
      {
         SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
         goto exit;
      }
   }

   for (int i = 0; i < Outline->h; i += CellSize)
   {
      dest.w = CellSize / 16;
      dest.h = CellSize;
      dest.x = Outline->x + Outline->w;
      dest.y = Outline->y + i;
      status = SDL_RenderTexture(
                     Renderer,
                     Tileset->Texture,
                     &tile,
                     &dest);
      if (status)
      {
         SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
         goto exit;
      }
   }


   tile.x = 16;
   tile.y = 48;
   tile.w = 16;
   tile.h = 1;

   for (int i = 0; i < Outline->w; i += CellSize)
   {
      dest.h = CellSize / 16;
      dest.w = CellSize;
      dest.x = Outline->x + i;
      dest.y = Outline->y - dest.h;
      status = SDL_RenderTexture(
                     Renderer,
                     Tileset->Texture,
                     &tile,
                     &dest);
      if (status)
      {
         SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
         goto exit;
      }
   }

   for (int i = 0; i < Outline->w; i += CellSize)
   {
      dest.h = CellSize / 16;
      dest.w = CellSize;
      dest.x = Outline->x + i;
      dest.y = Outline->y + Outline->h;
      status = SDL_RenderTexture(
                     Renderer,
                     Tileset->Texture,
                     &tile,
                     &dest);
      if (status)
      {
         SDL_Log("SDL_RenderTexture error = %s", SDL_GetError());
         goto exit;
      }
   }
   

exit:
   return status;
}