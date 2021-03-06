#include "../include/SWSB_background.h"

Background InitBackground(SDL_Renderer *renderer)
{
    Background background;

    background.width = SCREEN_WIDTH;
    background.height = SCREEN_HEIGHT;

    SetBackground(&background);

    background.texture = GetImageTexture(renderer, BACKGROUND_IMG);

    return background;
}

void SetBackground(Background *background)
{
    background->curr.x = 0;
    background->curr.y = 0;
    background->curr.w = background->width;
    background->curr.h = background->height;

    background->next.x = 0;
    background->next.y = -background->height;
    background->next.w = background->width;
    background->next.h = background->height;
}

void RenderBackground(Background *background, SDL_Renderer *renderer)
{
    background->curr.y += BACKGROUND_VEL;
    background->next.y += BACKGROUND_VEL;

    if (background->curr.y >= background->height)
    {
        background->curr.y = -background->height;
    }

    if (background->next.y >= background->height)
    {
        background->next.y = -background->height;
    }

    SDL_RenderCopy(renderer, background->texture, NULL, &background->curr);
    SDL_RenderCopy(renderer, background->texture, NULL, &background->next);
}

void FreeBackgroundTexture(Background *background)
{
    SDL_DestroyTexture(background->texture);
}
