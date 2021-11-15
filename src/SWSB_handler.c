#include "../include/SWSB_handler.h"

Handler InitHandler(const char *title, int x, int y, int w, int h, Uint32 flags)
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

    memset(handler.is_key_pressed, 0, 512);

    return handler;
}

void CheckGamePaused(Handler *handler, bool *is_paused)
{
    if (handler->keyboard[SDL_SCANCODE_ESCAPE] && !(handler->is_key_pressed[SDL_SCANCODE_ESCAPE]))
    {
        *is_paused = !(*is_paused);
        handler->is_key_pressed[SDL_SCANCODE_ESCAPE] = !(handler->is_key_pressed[SDL_SCANCODE_ESCAPE]);
    }
    else if (!(handler->keyboard[SDL_SCANCODE_ESCAPE]) && handler->is_key_pressed[SDL_SCANCODE_ESCAPE])
    {
        handler->is_key_pressed[SDL_SCANCODE_ESCAPE] = !(handler->is_key_pressed[SDL_SCANCODE_ESCAPE]);
    }
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
