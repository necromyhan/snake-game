#include <stdbool.h>

#include <SDL.h>
#include "SDL_events.h"
#include "SDL_init.h"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_timer.h"
#include "SDL_video.h"


#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600
#define OBJECT_SIZE     60

SDL_Window*     gWindow = NULL;
SDL_Renderer*   gRenderer = NULL;


int SdlInit()
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_InitSubSystem VIDEO Error!");
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        SDL_Log("SDL_Init Error!");
        return 1;
    }
        
    return 0;
}

int RenderObject(const SDL_Renderer* Renderer)
{
    if (!Renderer)
    {
        return 1;
    }


    // Background
    int status = SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
    if (status) { return status; }

    status = SDL_RenderClear(gRenderer);
    if (status) { return status; }

    // Object
    SDL_FRect object = { 
            (float)((WINDOW_WIDTH - OBJECT_SIZE) / 2),
            (float)((WINDOW_HEIGHT - OBJECT_SIZE) / 2),
            OBJECT_SIZE,
            OBJECT_SIZE };

    status = SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    if (status) { return status; }

    status = SDL_RenderFillRect(gRenderer, &object);
    if (status) { return status; }

    // General rendering
    status = SDL_RenderPresent(gRenderer);
    if (status) { return status; }

    return 0;
}

int main()
{
    if (SdlInit()) { return 1; }

    gWindow = SDL_CreateWindow("New window", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!gWindow)
    {   
        SDL_Log("CreateWindow Error!");
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, NULL, SDL_RENDERER_PRESENTVSYNC);
    if (!gRenderer)
    {
        SDL_Log("CreateRenderer Error!");
        return 1;
    }

    if (RenderObject(gRenderer))
    {
        SDL_Log("Object Render Error!");
        return 1;
    }

    SDL_Event event;
    bool isRunning = true;
    while (isRunning)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                isRunning = false;
            }
        }
    }

    SDL_DestroyWindow(gWindow);
    SDL_Quit();

    return 0;
}