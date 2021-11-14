#include "../include/SWSB_background.h"

Background InitBackground(SDL_Renderer *renderer)
{
    Background background;

    background.width = SCREEN_WIDTH;
    background.height = SCREEN_HEIGHT;

    background.curr.x = 0;
    background.curr.y = 0;
    background.curr.w = background.width;
    background.curr.h = background.height;

    background.next.x = 0;
    background.next.y = -background.height;
    background.next.w = background.width;
    background.next.h = background.height;

    background.texture = GetImageTexture(renderer, BACKGROUND_IMG);

    return background;
}

void RenderBackground(Background *background, SDL_Renderer *renderer)
{
    background->curr.y += 2;
    background->next.y += 2;

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
