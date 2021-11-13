#include "../include/SWSB.h"

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600

#define GAME_NAME       "Star Wars: Starship Battles"

#define BOLT_W         8
#define BOLT_H         26
#define SHIP_SIZE      100

#define PLAYER_SHIP_X   (SCREEN_WIDTH / 2) - (SHIP_SIZE / 2)
#define PLAYER_SHIP_Y   SCREEN_HEIGHT - SHIP_SIZE

void RunSWSB(void)
{
    Bolts player_bolts = InitBolts(5);

    Handler handler = SDL_InitHandler(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Background background = InitBackground(handler.wrenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    Entity player_ship = InitEntityTexture(PLAYER_SHIP_X, PLAYER_SHIP_Y, SHIP_SIZE, SHIP_SIZE, 5, handler.wrenderer, MF_TEXT);
    Entity standard_shot = InitEntityBolt(2, 20, 10, 250, 255, 0, 0, 255);

    while (true)
    {
        handler.time = SDL_GetTicks();

        SDL_SetRenderDrawColor(handler.wrenderer, 0, 0, 0, 255);
        SDL_RenderClear(handler.wrenderer);

        SDL_PollEvent(&handler.event);

        if (handler.event.type == SDL_QUIT)
        {
            break;
        }

        if (handler.keyboard[SDL_SCANCODE_A] && player_ship.box.x > 0)
        {
            player_ship.box.x -= player_ship.vel; 
        }
        if (handler.keyboard[SDL_SCANCODE_D] && (player_ship.box.x + player_ship.box.w) < SCREEN_WIDTH)
        {
            player_ship.box.x += player_ship.vel;
        }

        if (handler.keyboard[SDL_SCANCODE_SPACE]
            && !(IsBoltsFull(&player_bolts))
            && handler.time > player_bolts.next_shot)
        {
            AppendBolts(&player_bolts, &standard_shot, &player_ship.box);
            player_bolts.next_shot = handler.time + standard_shot.firetime;
        }

        RenderBackground(&background, handler.wrenderer);
        RenderEntityTexture(&player_ship, handler.wrenderer);
        RenderBolts(&player_bolts, handler.wrenderer);

        SDL_RenderPresent(handler.wrenderer);

        SetFrameRate(&handler);
    }

    FreeBolts(&player_bolts);
    FreeEntityTexture(&player_ship);
    FreeBackgroundTexture(&background);
    FreeHandler(&handler);
    SDL_Quit();
}
