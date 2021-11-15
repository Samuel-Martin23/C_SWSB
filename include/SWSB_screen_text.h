#ifndef SWSB_SCREEN_TEXT_H
#define SWSB_SCREEN_TEXT_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "SWSB_assets.h"

typedef struct
{
    TTF_Font *font;
    SDL_Color color;
    SDL_Texture *texture;
    SDL_Rect rect;
    int text_num;
    char text_buffer[MAX_BUFFER];
} ScreenText;

ScreenText InitScreenTextScore(SDL_Renderer *renderer, const char *text);
void SetScoreScreenText(ScreenText *st, int score, SDL_Renderer *renderer);
void RenderScreenText(ScreenText *st, SDL_Renderer *renderer);
void FreeScreenText(ScreenText *st);

#endif /* SWSB_SCREEN_TEXT_H */
