#ifndef __SNAKE_GAME_MENU_H__
#define __SNAKE_GAME_MENU_H__

#include <SDL.h>

enum MENU_ITEM_TYPE
{
   MenuItemStart,
   MenuItemOptions,
   MenuItemExit,
   MenuItemMax
};

enum MENU_ITEM_STATE
{
   ItemStateIdle,
   ItemStateHover
};

typedef struct _MENU_ITEM
{
   const char* Text;
   int         Type;
   int         State;
} MENU_ITEM;

typedef struct _MENU
{
   int         Count;
   int         CurrentType;
   MENU_ITEM   Items[];   
} MENU;

MENU*
CreateMenu(void);

void
DestroyMenu(MENU* Menu);

#endif //__SNAKE_GAME_MENU_H__