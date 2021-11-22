#ifndef SWSB_BACKGROUND_H
#define SWSB_BACKGROUND_H

#include <SDL2/SDL.h>
#include "SWSB_assets.h"

typedef struct
{
    int width;
    int height;
    SDL_Rect curr;
    SDL_Rect next;
    SDL_Texture *texture;
} Background;

Background InitBackground(SDL_Renderer *renderer);
void SetBackground(Background *background);
void RenderBackground(Background *background, SDL_Renderer *renderer);
void FreeBackgroundTexture(Background *background);

#endif /* SWSB_BACKGROUND_H */
