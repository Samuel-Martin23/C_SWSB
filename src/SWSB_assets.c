#include "../include/SWSB_assets.h"

int rand_int(int min, int max)
{
    if (min > max)
    {
        return 0;
    }

    int range = (max - min) + 1;
    return (rand() % range) + min;
}

SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path)
{
    SDL_Texture *image_texuare = NULL;
    SDL_Surface *image_surface = NULL;

    image_surface = IMG_Load(image_path);

    if (image_surface == NULL)
    {
        printf("Could not create image surface: %s\n", SDL_GetError());
        exit(1);
    }

    image_texuare = SDL_CreateTextureFromSurface(renderer, image_surface);
    SDL_FreeSurface(image_surface);

    if (image_texuare == NULL)
    {
        printf("Could not create image texture: %s\n", SDL_GetError());
        exit(1);
    }

    return image_texuare;
}
