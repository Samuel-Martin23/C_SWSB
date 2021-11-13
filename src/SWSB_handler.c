#include "../include/SWSB_handler.h"

Handler SDL_InitHandler(const char *title, int x, int y, int w, int h, Uint32 flags)
{
    SDL_Init(SDL_INIT_VIDEO);

    Handler handler;

    handler.window = SDL_CreateWindow(title, x, y, w, h, flags);

    if (handler.window == NULL)
    {
        printf("Could not create window: %s\n", SDL_GetError());
        exit(1);
    }

    handler.wrenderer = SDL_CreateRenderer(handler.window, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    if (handler.wrenderer == NULL)
    {
        printf("Could not create window renderer: %s\n", SDL_GetError());
        exit(1);
    }

    handler.keyboard = SDL_GetKeyboardState(NULL);
    handler.time = 0;

    return handler;
}

void SetFrameRate(Handler *handler)
{
    if ((1000 / FPS) > (SDL_GetTicks() - handler->time))
    {
        SDL_Delay((1000 / FPS) - (SDL_GetTicks() - handler->time));
    }
}

void FreeHandler(Handler *handler)
{
    SDL_DestroyRenderer(handler->wrenderer);
    SDL_DestroyWindow(handler->window);
}
