#include <stdbool.h>
#include <stdlib.h>

#include <SDL.h>
#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_timer.h"
#include "SDL_video.h"

#include "snake.h"
#include "apple.h"
#include "font.h"
#include "game.h"

bool gInputHandled = false;

static SCENE*  gScenes[] = 
{  
   &gMenuScene,
   &gGameplayScene,
   &gGameOverScene
};
GAME_STATE gCurrentScene = StateMenu;

static
int
SdlInit(void)
{
   if (SDL_InitSubSystem(SDL_INIT_VIDEO))
   {
      SDL_Log("SDL_InitSubSystem VIDEO Error!");
      SDL_Log("%s", SDL_GetError());
      return 1;
   }

   if (SDL_Init(SDL_INIT_EVERYTHING))
   {
      SDL_Log("SDL_Init Error!");
      SDL_Log("%s", SDL_GetError());
      return 1;
   }

   return 0;
}

static
bool
IsPeriodPassed(
   int  Period,
   int  LastUpdateTime)
{
   int currentTime = SDL_GetTicks();
   if ((currentTime - LastUpdateTime) >= Period)
   {
      return true;
   }

   return false;
}

int main()
{
   if (SdlInit())
   {
      goto exit;
   }

   GAME game;
   if (InitGame(&game))
   {
      goto exit;
   }

   bool isRunning = true;
   int lastFrameTime = 0;
   SDL_Event event;
   while (isRunning)
   {
      if (SDL_PollEvent(&event))
      {
         if (event.type == SDL_EVENT_QUIT)
         {
            isRunning = false;
         }
      }

      gScenes[gCurrentScene]->HandleEvents(&game, &event);

      // WaitTime(300, lastFrameTime);
      if (IsPeriodPassed(200, lastFrameTime))
      {
         gScenes[gCurrentScene]->Update(&game);
         gScenes[gCurrentScene]->Render(&game);
         gInputHandled = false;
         lastFrameTime = SDL_GetTicks();
         if (IsSnakeIntersection(game.Snake))
         {
            gCurrentScene = StateGameOver;
            SDL_Log("GAME OVER!");
         }
      }
   }

exit:
   ExitGame(&game);
   SDL_Quit();

   return 0;
}