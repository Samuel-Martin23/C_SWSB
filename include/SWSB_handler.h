#ifndef SWSB_HANDLER_H
#define SWSB_HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *wrenderer;
    const Uint8 *keyboard;
    bool is_key_pressed[SDL_NUM_SCANCODES];
} Handler;

Handler InitHandler(const char *title, int x, int y, int w, int h, Uint32 flags);
void CheckGamePaused(Handler *handler, bool *is_paused);
void FreeHandler(Handler *handler);

#endif /* SWSB_HANDLER_H */
