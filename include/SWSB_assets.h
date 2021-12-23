#ifndef SWSB_ASSETS_H
#define SWSB_ASSETS_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef ASSETS_PATH
#define ASSETS_PATH "../assets/"
#endif

#define GAME_NAME                   "Star Wars: Starship Battles"
#define SCREEN_WIDTH                800
#define SCREEN_HEIGHT               600

#define MAX_BUFFER                  512

#define GAME_FONT                   (ASSETS_PATH"pixel_font.ttf")
#define SCORE_FONT_SIZE             20
#define SCORE_MAX_BUFFER            10
#define SCORE_MAX_SIZE              (SCORE_MAX_BUFFER - 1)
#define POINTS_PER_FRAME            1

#define BACKGROUND_IMG              (ASSETS_PATH"star_background.png")
#define BACKGROUND_VEL              2

#define MF_IDLE_IMG                 (ASSETS_PATH"mf_idle.png")
#define MF_FLYING_IMG               (ASSETS_PATH"mf_flying.png")

#define PLAYER_W                    100
#define PLAYER_H                    100
#define PLAYER_X                    ((SCREEN_WIDTH / 2) - (PLAYER_W / 2))
#define PLAYER_Y                    ((SCREEN_HEIGHT - PLAYER_H) - (PLAYER_H / 10))
#define PLAYER_HALF_W               (PLAYER_W / 2)   
#define PLAYER_QTR_W                (PLAYER_W / 4)
#define PLAYER_VEL                  (6 + BACKGROUND_VEL)
#define PLAYER_DAMAGE               20
#define PLAYER_HEALTH               100
#define PLAYER_NEXT_SPAWN           1500

#define PLAYER_BOLT_VEL             (8 + BACKGROUND_VEL)
#define PLAYER_BOLT_DAMAGE          10
#define PLAYER_BOLT_FT              225
#define PLAYER_BOLT_IFT             500

#define ASTER_GRAY_IMG_1            (ASSETS_PATH"asteroid_gray_1.png")
#define ASTER_GRAY_IMG_2            (ASSETS_PATH"asteroid_gray_2.png")
#define ASTER_GRAY_IMG_3            (ASSETS_PATH"asteroid_gray_3.png")
#define ASTER_GRAY_IMG_4            (ASSETS_PATH"asteroid_gray_4.png")
#define ASTER_BROWN_IMG_1           (ASSETS_PATH"asteroid_brown_1.png")
#define ASTER_BROWN_IMG_2           (ASSETS_PATH"asteroid_brown_2.png")
#define ASTER_BROWN_IMG_3           (ASSETS_PATH"asteroid_brown_3.png")
#define ASTER_BROWN_IMG_4           (ASSETS_PATH"asteroid_brown_4.png")
#define ASTER_MIN_SIZE              40
#define ASTER_MAX_SIZE              45
#define ASTER_VEL                   (1 + BACKGROUND_VEL)
#define ASTER_DAMAGE                100
#define ASTER_HEALTH                30
#define ASTER_IST                   5000
#define ASTER_CST                   500
#define ASTER_POINTS                1000
#define ASTER_SS                    120

#define PW_WIDTH                    50
#define PW_HEIGHT                   50
#define PW_VEL                      (BACKGROUND_VEL)
#define PW_IST                      5000
#define PW_CST                      (PW_IST * 10)

#define PW_FAST                     10000

#define EXPLO_IMG_1                 (ASSETS_PATH"explo_1.png")
#define EXPLO_IMG_2                 (ASSETS_PATH"explo_2.png")
#define EXPLO_IMG_3                 (ASSETS_PATH"explo_3.png")
#define EXPLO_IMG_4                 (ASSETS_PATH"explo_4.png")
#define EXPLO_IMG_5                 (ASSETS_PATH"explo_5.png")
#define EXPLO_IMG_6                 (ASSETS_PATH"explo_6.png")
#define EXPLO_IMG_7                 (ASSETS_PATH"explo_7.png")
#define EXPLO_IMG_8                 (ASSETS_PATH"explo_8.png")
#define EXPLO_VEL                   (BACKGROUND_VEL)
#define EXPLO_SS                    75

#define TF_IDLE_IMG                 (ASSETS_PATH"tf_idle.png")
#define TF_W                        60
#define TF_H                        60
#define TF_VEL                      (2 + BACKGROUND_VEL)
#define TF_DAMAGE                   20
#define TF_HEALTH                   20
#define TF_IST                      10000
#define TF_CST                      3000
#define TF_POINTS                   2000
#define TF_PLAYER_HIT_POINTS        4000

#define TF_BOLT_VEL                 (3 + TF_VEL)
#define TF_BOLT_DAMAGE              5
#define TF_BOLT_FT                  425

int rand_int(int min, int max);
SDL_Texture *GetImageTexture(SDL_Renderer *renderer, const char *image_path);

#endif /* SWSB_ASSETS_H */
