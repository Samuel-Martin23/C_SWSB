#include "../include/SWSB.h"

void RunSWSB(void)
{
    Handler handler = InitHandler(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Background background = InitBackground(handler.wrenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    Entity *player_ship = InitPlayerEntity(PLAYER_SHIP_X, PLAYER_SHIP_Y, MF_W, MF_H, 5, 250, handler.wrenderer, MF_IMG);
    Entity *player_bolt = NULL;
    Entities total_ents = {0, MAX_ENTS, {}};

    Uint32 player_next_shot = 0;
    Uint32 aster_next_spawn = 0;

    bool render_asteroids = false;

    AppenedEntityPlayer(&total_ents, player_ship);

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

        if (handler.time >= 5000 && !(render_asteroids))
        {
            render_asteroids = true;
        }

        if (handler.keyboard[SDL_SCANCODE_A] && player_ship->box.x > 0)
        {
            player_ship->box.x -= player_ship->vel; 
        }
        if (handler.keyboard[SDL_SCANCODE_D] && (player_ship->box.x + player_ship->box.w) < SCREEN_WIDTH)
        {
            player_ship->box.x += player_ship->vel;
        }

        if (handler.keyboard[SDL_SCANCODE_SPACE]
            && !(IsEntitiesFull(&total_ents))
            && handler.time > player_next_shot)
        {
            player_bolt = InitBoltEntity(2, 20, 10, 255, 0, 0, 255);
            AppendEntityBolt(&total_ents, player_bolt, &player_ship->box);
            player_next_shot = handler.time + player_ship->firetime;
        }

        if (render_asteroids
            && handler.time > aster_next_spawn
            && !(IsEntitiesFull(&total_ents)))
        {
            AppendEntityAster(&total_ents, handler.wrenderer);
            aster_next_spawn = handler.time + 4000;
        }

        RenderBackground(&background, handler.wrenderer);

        RenderEntities(&total_ents, handler.wrenderer);

        SDL_RenderPresent(handler.wrenderer);

        SetFrameRate(&handler);
    }

    FreeEntities(&total_ents);
    FreeBackgroundTexture(&background);
    FreeHandler(&handler);
    SDL_Quit();
}
