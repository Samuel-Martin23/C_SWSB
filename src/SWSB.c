#include "../include/SWSB.h"

void RunSWSB(void)
{
    Handler handler = InitHandler(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Background background = InitBackground(handler.wrenderer);

    Entity *player_ship = InitPlayerEntity(handler.wrenderer);
    Entity *player_bolt = NULL;
    Entities total_ents = {0, MAX_ENTS, {}};

    Uint32 player_next_shot = 0;
    Uint32 aster_next_spawn = ASTER_IST;
    Uint32 powerup_next_spawn = PW_IST;
    EntityType type = ET_NONE;

    BoltComponent bolt = {2, 20, SHOT_VEL, SHOT_DAMAGE, 255, 0, 0, 255};
    Uint32 powerup_expired = 0;

    AppendEntityPlayer(&total_ents, player_ship);

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

        if (handler.keyboard[SDL_SCANCODE_A] && player_ship->box.x > 0)
        {
            player_ship->box.x -= player_ship->vel; 
        }
        if (handler.keyboard[SDL_SCANCODE_D] && (player_ship->box.x + player_ship->box.w) < SCREEN_WIDTH)
        {
            player_ship->box.x += player_ship->vel;
        }

        type = DetectEntityCollision(&total_ents);

        if (type == ET_POWERUP)
        {
            SetBoltComp(&bolt, 2, 20, SHOT_VEL*2, SHOT_DAMAGE, 0, 255, 0, 255);
            powerup_expired = handler.time + PW_FAST;
        }

        if (handler.time > powerup_expired)
        {
            SetBoltComp(&bolt, 2, 20, SHOT_VEL, SHOT_DAMAGE, 255, 0, 0, 255);
        }

        if (handler.keyboard[SDL_SCANCODE_SPACE]
            && handler.time > player_next_shot)
        {
            player_bolt = InitBoltEntity(&bolt);
            AppendEntityBolt(&total_ents, player_bolt, &player_ship->box);
            player_next_shot = handler.time + PLAYER_FT;
        }

        if (handler.time > aster_next_spawn)
        {
            AppendEntityAster(&total_ents, handler.wrenderer);
            aster_next_spawn = handler.time + ASTER_CST;
        }

        if (handler.time > powerup_next_spawn)
        {
            AppendEntityPowerUp(&total_ents);
            powerup_next_spawn = handler.time + PW_CST;
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
