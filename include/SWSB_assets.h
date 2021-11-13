#ifndef SWSB_ASSETS_H
#define SWSB_ASSETS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GAME_NAME           "Star Wars: Starship Battles"
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600

#define MF_IMG              "../assets/millennium_falcon.png"
#define MF_W                100
#define MF_H                100
#define PLAYER_SHIP_X       (SCREEN_WIDTH / 2) - (MF_W / 2)
#define PLAYER_SHIP_Y       SCREEN_HEIGHT - MF_H

#define BACKGROUND_IMG      "../assets/star_background.png"
#define ASTER_SIZE          50

#define ASTER_GRAY_IMG      "../assets/asteroid_gray.png"

SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path);

#endif /* SWSB_ASSETS_H */
