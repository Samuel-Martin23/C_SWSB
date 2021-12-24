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

    handler.game_state = GAME_NONE;

    memset(handler.is_key_pressed, 0, SDL_NUM_SCANCODES);
    memset(handler.key_state, 0, SDL_NUM_SCANCODES);

    srand((unsigned int)time(NULL));

    return handler;
}

void UpdateKeyboard(Handler *handler)
{
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    for (Uint16 i = 0; i < SDL_NUM_SCANCODES; i++)
    {
        if (keyboard[i] && !(handler->is_key_pressed[i]))
        {
            handler->is_key_pressed[i] = !(handler->is_key_pressed[i]);
            handler->key_state[i] = KEY_PRESSED;
            continue;
        }
        else if (keyboard[i] && handler->is_key_pressed[i])
        {
            handler->key_state[i] = KEY_HELD_DOWN;
            continue;
        }
        else if (!(keyboard[i]) && handler->is_key_pressed[i])
        {
            handler->is_key_pressed[i] = !(handler->is_key_pressed[i]);
        }

        handler->key_state[i] = KEY_UNPRESSED;
    }
}

Uint8 GetKeyState(Handler *handler, Uint16 key)
{
    return handler->key_state[key];
}

void FreeHandler(Handler *handler)
{
    SDL_DestroyRenderer(handler->wrenderer);
    SDL_DestroyWindow(handler->window);
}
