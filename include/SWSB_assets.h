#ifndef SWSB_ASSETS_H
#define SWSB_ASSETS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GAME_NAME           "Star Wars: Starship Battles"
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       600

#define MAX_BUFFER          512

#define GAME_FONT           "../assets/pixel_font.ttf"
#define SCORE_FONT_SIZE     20
#define SCORE_MAX_BUFFER    10
#define SCORE_MAX_SIZE      (SCORE_MAX_BUFFER - 1)
#define POINTS_PER_FRAME    1

#define BACKGROUND_IMG      "../assets/star_background.png"
#define BACKGROUND_VEL      2

#define MF_IDLE_IMG         "../assets/mf_idle.png"    
#define MF_FLYING_IMG       "../assets/mf_flying.png"

#define PLAYER_W            100
#define PLAYER_H            100
#define PLAYER_X            ((SCREEN_WIDTH / 2) - (PLAYER_W / 2))
#define PLAYER_Y            ((SCREEN_HEIGHT - PLAYER_H) - (PLAYER_H / 10))
#define PLAYER_HALF_W       (PLAYER_W / 2)   
#define PLAYER_QTR_W        (PLAYER_W / 4)
#define PLAYER_VEL          (6 + BACKGROUND_VEL)
#define PLAYER_DAMAGE       5
#define PLAYER_HEALTH       100
#define PLAYER_FT           225

#define ASTER_GRAY_1_IMG    "../assets/asteroid_gray_1.png"
#define ASTER_GRAY_2_IMG    "../assets/asteroid_gray_2.png"
#define ASTER_GRAY_3_IMG    "../assets/asteroid_gray_3.png"
#define ASTER_GRAY_4_IMG    "../assets/asteroid_gray_4.png"
#define ASTER_BROWN_1_IMG   "../assets/asteroid_brown_1.png"
#define ASTER_BROWN_2_IMG   "../assets/asteroid_brown_2.png"
#define ASTER_BROWN_3_IMG   "../assets/asteroid_brown_3.png"
#define ASTER_BROWN_4_IMG   "../assets/asteroid_brown_4.png"

#define ASTER_MIN_SIZE      50
#define ASTER_MAX_SIZE      60
#define ASTER_VEL           (1 + BACKGROUND_VEL)
#define ASTER_DAMAGE        100
#define ASTER_HEALTH        30
#define ASTER_IST           5000
#define ASTER_CST           450
#define ASTER_POINTS        1000

#define SHOT_VEL            (8 + BACKGROUND_VEL)
#define SHOT_DAMAGE         10

#define PW_WIDTH            50
#define PW_HEIGHT           50
#define PW_VEL              (BACKGROUND_VEL)
#define PW_IST              5000
#define PW_CST              (PW_IST * 10)

#define PW_FAST             10000

int rand_int(int min, int max);
SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path);

#endif /* SWSB_ASSETS_H */
