#include <stdbool.h>

#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "SDL_video.h"
#include <SDL.h>


SDL_Window* gWindow = NULL;

int main()
{

    if (SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        SDL_Log("Init VIDEO Error!");
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("Init Error!");
        return 1;
    }


    gWindow = SDL_CreateWindow("New window", 800, 600, 0);

    if (!gWindow)
    {   
        SDL_Log("CreateWindow Error!");
        return 1;
    }


    bool isRunning = true;


    while (isRunning)
    {
        SDL_Event event;
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }
    }

    SDL_DestroyWindow(gWindow);


    return 0;
}