#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "menu.h"

static SDL_Texture* gStartMenuPic = NULL;

enum
{
   TitleWidth = 256,
   TitleHeight = 128
};

MENU*
CreateMenu(
   int            Count,
   const char**   Strings)
{
   MENU* menu = NULL;

   menu = SDL_malloc(sizeof(MENU) + sizeof(MENU_ITEM) * Count);
   if (NULL == menu)
   {
      goto exit;
   }

   menu->Count = Count;
   for (int i = 0; i < Count; ++i)
   {
      menu->Items[i].State = ItemStateIdle;
      menu->Items[i].Text = Strings[i];
      menu->Items[i].Type = i;
   }

   menu->Items[0].State = ItemStateHover;
   menu->ActiveType = 0;

exit:
   return menu;
}

void
DestroyMenu(MENU* Menu)
{
   if (Menu)
   {
      SDL_free(Menu);
   }
}

int
StartMenuHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   int status = 0;
   if (NULL == Event || NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      switch (Event->key.keysym.sym)
      {
         case SDLK_UP:
         {
            if (Game->StartMenu->ActiveType > StartMenuStart)
            {
               Game->StartMenu->Items[Game->StartMenu->ActiveType--].State = ItemStateIdle;
               Game->StartMenu->Items[Game->StartMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->StartMenu->ActiveType < StartMenuMax - 1)
            {
               Game->StartMenu->Items[Game->StartMenu->ActiveType++].State = ItemStateIdle;
               Game->StartMenu->Items[Game->StartMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_RETURN:
         {
            if (Game->StartMenu->ActiveType == StartMenuStart)
            {
               PushUserEvent(gChangeSceneEventType, StateGameplay);
            }
            else if (Game->StartMenu->ActiveType == StartMenuExit)
            {
               SDL_Event event;
               event.type = SDL_EVENT_QUIT;
               SDL_PushEvent(&event);
            }
         }
      }
   }

   exit:
   return status;
}

int
StartMenuUpdate(GAME* Game)
{
   return 0;
}

int
StartMenuRender(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (!gStartMenuPic)
   {
      gStartMenuPic = CreateTextureFromImage(Game->Renderer, "assets/title.png");
      if (NULL == gStartMenuPic)
      {
         status = -1;
         goto exit;
      }
   }

   SDL_SetRenderViewport(Game->Renderer, NULL);

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }
   
   status = SDL_RenderFillRect(Game->Renderer, NULL);
   if (status) { goto exit; }

   int w, h;
   status = SDL_GetWindowSizeInPixels(Game->Window, &w, &h);
   if (status) { goto exit; }

   int fac = w / TitleWidth;
   SDL_FRect picRect = {
               (w - fac * TitleWidth) / 2,
               Game->Field.CellSize / 2,
               TitleWidth * fac,
               TitleHeight * fac};
   SDL_RenderTexture(Game->Renderer, gStartMenuPic, NULL, &picRect);
   if (status) { goto exit; }

   int textW;
   int textH;
   status = GetTextSize(Game->Font, "start",  Game->Field.CellSize, &textW, NULL);
   if (status) { goto exit; }

   int posX = (w - textW) / 2;
   int posY = Game->Field.CellSize + TitleHeight * fac;
   for (int i = 0; i < Game->StartMenu->Count; ++i)
   {
      status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     Game->StartMenu->Items[i].Text,
                     Game->Field.CellSize,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = posX, .y = posY});
      if (status) { goto exit; }

      if (Game->StartMenu->Items[i].State)
      {
         status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     ">",
                     Game->Field.CellSize,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = posX - Game->Field.CellSize, .y = posY});
         if (status) { goto exit; }
      }

      posY += Game->Field.CellSize;
   }

   char copyrightStr[] = "2023 Necromyhan";
   status = GetTextSize(Game->Font, copyrightStr, Game->Field.CellSize / 2, &textW, &textH);
   if (status) { goto exit; }

   status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     copyrightStr,
                     Game->Field.CellSize / 2,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = (w - textW) / 2, .y = h - 2 * textH});
   if (status) { goto exit; }


   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}

SCENE gMenuScene = 
{
   .HandleEvents  = StartMenuHandleEvents,
   .Render        = StartMenuRender,
   .Update        = StartMenuUpdate    
};

int
GameOverMenuHandleEvents(
   GAME*             Game,
   const SDL_Event*  Event)
{
   int status = 0;
   if (NULL == Event || NULL == Game)
   {
      status = -1;
      goto exit;
   }

   if (Event->type == SDL_EVENT_KEY_DOWN)
   {
      switch (Event->key.keysym.sym)
      {
         case SDLK_UP:
         {
            if (Game->GameOverMenu->ActiveType > GameOverMenuRetry)
            {
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType--].State = ItemStateIdle;
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->GameOverMenu->ActiveType < GameOverMenuMax - 1)
            {
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType++].State = ItemStateIdle;
               Game->GameOverMenu->Items[Game->GameOverMenu->ActiveType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_RETURN:
         {
            if (Game->GameOverMenu->ActiveType == GameOverMenuExit)
            {
               PushUserEvent(gChangeSceneEventType, StateMenu);
            }
            else if (Game->GameOverMenu->ActiveType == GameOverMenuRetry)
            {
               PushUserEvent(gChangeSceneEventType, StateGameplay);
            }
         }
      }
   }   

exit:
   return status;
}

int
GameOverMenuUpdate(GAME* Game)
{
   return 0;
}

int
GameOverMenuRender(GAME* Game)
{
   int status = 0;

   if (NULL == Game)
   {
      status = -1;
      goto exit;
   }

   status = SDL_SetRenderDrawColor(Game->Renderer, 0, 0, 0, 0);
   if (status) { goto exit; }
   
   status = SDL_RenderFillRect(Game->Renderer, NULL);
   if (status) { goto exit; }

   int w, h;
   status = SDL_GetWindowSizeInPixels(Game->Window, &w, &h);
   if (status) { goto exit; }

   int textW, textH;
   const char goStr[] = "GAME OVER";
   status = GetTextSize(Game->Font, goStr, Game->Field.CellSize * 3 / 2, &textW, &textH);
   if (status) { goto exit; }

   status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     "GAME OVER",
                     Game->Field.CellSize * 3 / 2,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = (w - textW) / 2, .y = h / 3});
   if (status) { goto exit; }

   status = GetTextSize(Game->Font, "Restart", Game->Field.CellSize / 2, &textW, NULL);
   if (status) { goto exit; }

   int posX = (w - textW) / 2;
   int posY = h / 3 + textH +  2 * Game->Field.CellSize;
   for (int i = 0; i < Game->GameOverMenu->Count; ++i)
   {
      status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     Game->GameOverMenu->Items[i].Text,
                     Game->Field.CellSize / 2,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = posX, .y = posY});
      if (status)
      {
         goto exit;
      }

      if (Game->GameOverMenu->Items[i].State)
      {
         status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     ">",
                     Game->Field.CellSize / 2,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = posX - Game->Field.CellSize, .y = posY});
         if (status) { goto exit; }
      }

      // TODO: get screen width from window
      posY += Game->Field.CellSize;
   }
   
   char scoreStr[16];
   int symNumber = SDL_snprintf(&scoreStr[0], 16, "Score: %d", Game->PreviousScore);

   status = GetTextSize(Game->Font, scoreStr, Game->Field.CellSize / 2, &textW, NULL);
   if (status) { goto exit; }

   status = PrintFontToRenderer(
                     Game->Font,
                     Game->Renderer,
                     scoreStr,
                     Game->Field.CellSize / 2,
                     (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
                     (SDL_Point){.x = (w - textW) / 2, .y = h / 3 + textH + Game->Field.CellSize / 4});
         if (status) { goto exit; }

   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}

SCENE gGameOverScene = 
{
   .HandleEvents  = GameOverMenuHandleEvents,
   .Render        = GameOverMenuRender,
   .Update        = GameOverMenuUpdate
};