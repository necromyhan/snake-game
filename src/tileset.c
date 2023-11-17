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