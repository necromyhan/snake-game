#ifndef __SNAKE_GAME_MENU_H__
#define __SNAKE_GAME_MENU_H__

#include <SDL.h>

enum START_MENU_ITEM_TYPE
{
   StartMenuStart,
   StartMenuOptions,
   StartMenuExit,
   StartMenuMax
};

enum GAME_OVER_MENU_ITEM_TYPE
{
   GameOverMenuRetry,
   GameOverMenuExit,
   GameOverMenuMax
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
   int         ActiveType;
   MENU_ITEM   Items[];   
} MENU;


MENU*
CreateMenu(
   int            Count,
   const char**   Strings);

void
DestroyMenu(MENU* Menu);

#endif //__SNAKE_GAME_MENU_H__