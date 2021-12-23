#ifndef SWSB_HANDLER_H
#define SWSB_HANDLER_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define KEY_PRESSED         1
#define KEY_HELD_DOWN       2
#define KEY_UNPRESSED       4

typedef enum
{
    GAME_NONE,
    GAME_QUIT,
    GAME_RESTART
} GameState;

typedef struct
{
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *wrenderer;
    GameState game_state;
    bool is_key_pressed[SDL_NUM_SCANCODES];
    Uint8 key_state[SDL_NUM_SCANCODES];
} Handler;

Handler InitHandler(const char *title, int x, int y, int w, int h, Uint32 flags);
void UpdateKeyboard(Handler *handler);
Uint8 GetKeyState(Handler *handler, Uint8 key);
void FreeHandler(Handler *handler);

#endif /* SWSB_HANDLER_H */
