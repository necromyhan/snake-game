#include <stdbool.h>
#include <stdlib.h>

#include "game.h"
#include "menu.h"

static char* gMenuString[] =
{
   "Start",
   "Options",
   "Exit"
};

MENU*
CreateMenu(void)
{
   MENU* menu = NULL;

   menu = SDL_malloc(sizeof(MENU) + sizeof(MENU_ITEM) * MenuItemMax);
   if (NULL == menu)
   {
      goto exit;
   }

   menu->Count = MenuItemMax;
   for (int i = 0; i < MenuItemMax; ++i)
   {
      menu->Items[i].State = ItemStateIdle;
      menu->Items[i].Text = gMenuString[i];
      menu->Items[i].Type = i;
   }

   menu->Items[0].State = ItemStateHover;
   menu->CurrentType = 0;

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

extern GAME_STATE gCurrentScene;

int
MenuHandleEvents(
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
            if (Game->Menu->CurrentType > MenuItemStart)
            {
               Game->Menu->Items[Game->Menu->CurrentType--].State = ItemStateIdle;
               Game->Menu->Items[Game->Menu->CurrentType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_DOWN:
         {
            if (Game->Menu->CurrentType < MenuItemMax - 1)
            {
               Game->Menu->Items[Game->Menu->CurrentType++].State = ItemStateIdle;
               Game->Menu->Items[Game->Menu->CurrentType].State = ItemStateHover;
            }
         }
         break;
         case SDLK_RETURN:
         {
            SDL_Log("Enter");
            if (Game->Menu->CurrentType == MenuItemStart)
            {
               gCurrentScene = StateGameplay;
            }
         }
      }
   }

   exit:
   return status;
}

int
MenuUpdate(GAME* Game)
{
   return 0;
}

int
MenuRender(GAME* Game)
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

   int posX = Game->Field.CellSize;
   int posY = Game->Field.CellSize;
   for (int i = 0; i < Game->Menu->Count; ++i)
   {
      status = PrintFontToRenderer(
            Game->Font,
            Game->Renderer,
            Game->Menu->Items[i].Text,
            (Game->Menu->Items[i].State) ? (SDL_Color){.b = 200, .g = 0, .r = 0, .a = 0} : (SDL_Color){.b = 255, .g = 255, .r = 255, .a = 0},
            (SDL_Point){.x = posX, .y = posY});
      if (status)
      {
         goto exit;
      }

      // TODO: get screen width from window
      posY += Game->Field.CellSize;
   }
   
   status = SDL_RenderPresent(Game->Renderer);

exit:
   return status;
}

SCENE gMenuScene = 
{
   .HandleEvents  = MenuHandleEvents,
   .Render        = MenuRender,
   .Update        = MenuUpdate    
};


