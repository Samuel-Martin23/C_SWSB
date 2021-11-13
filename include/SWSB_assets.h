#ifndef SWSB_ASSETS_H
#define SWSB_ASSETS_H

#define MF_TEXT             "../assets/millennium_falcon.png"
#define BACKGROUND_TEXT     "../assets/star_background.png"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path);

#endif /* SWSB_ASSETS_H */
