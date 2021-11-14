#ifndef SWSB_ASSETS_H
#define SWSB_ASSETS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GAME_NAME           "Star Wars: Starship Battles"
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600

#define BACKGROUND_IMG      "../assets/star_background.png"

#define MF_IMG              "../assets/millennium_falcon.png"
#define MF_W                100
#define MF_H                100
#define PLAYER_SHIP_X       (SCREEN_WIDTH / 2) - (MF_W / 2)
#define PLAYER_SHIP_Y       SCREEN_HEIGHT - MF_H
#define PLAYER_VEL          8
#define PLAYER_DAMAGE       5
#define PLAYER_HEALTH       100
#define PLAYER_FT           225

#define ASTER_GRAY_IMG      "../assets/asteroid_gray.png"
#define ASTER_SIZE          50
#define ASTER_VEL           3
#define ASTER_DAMAGE        100
#define ASTER_HEALTH        30
#define ASTER_IST           5000
#define ASTER_CST           4000

#define SHOT_VEL            10
#define SHOT_DAMAGE         10

#define PW_WIDTH            50
#define PW_HEIGHT           50
#define PW_VEL              2
#define PW_IST              5000
#define PW_CST              (PW_IST * 10)

SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path);

#endif /* SWSB_ASSETS_H */
