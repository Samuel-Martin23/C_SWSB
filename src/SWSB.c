#include "../include/SWSB.h"

void RunSWSB(void)
{
    Handler handler = InitHandler(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Background background = InitBackground(handler.wrenderer);
    Entities total_ents = {0, MAX_ENTS, {NULL}};
    ScreenText game_score = InitScreenTextScore(handler.wrenderer, "000000000");

    Entity *player_ship = InitPlayerEntity(handler.wrenderer);

    EntityCollision ec = {ET_NONE, ET_NONE, false, false};
    BoltComponent bolt = {2, 20, SHOT_VEL, SHOT_DAMAGE, 255, 0, 0, 255};

    Uint32 player_next_shot = 0;
    Uint32 aster_next_spawn = ASTER_IST;
    Uint32 powerup_next_spawn = PW_IST;
    Uint32 powerup_expired = 0;
    Uint32 current_ticks = 0;

    bool is_paused = false;
    int current_score = 0;


    AppendEntityPlayer(&total_ents, player_ship);
    StartStopWatch(&handler.timer);

    while (true)
    {
        SetIterationTicks(&handler.timer);

        /*
        ==============
        Quit or pause game.
        ==============
        */
        SDL_PollEvent(&handler.event);

        if (handler.event.type == SDL_QUIT)
        {
            break;
        }

        CheckGamePaused(&handler, &is_paused);

        if (!(total_ents.elems[PLAYER_ENT]->health) || is_paused)
        {
            PauseStopWatch(&handler.timer);
            continue;
        }

        ResumeStopWatch(&handler.timer);

        current_ticks = GetStopWatchTicks(&handler.timer);

        /*
        ==============
        Update Player.
        ==============
        */
        SDL_SetRenderDrawColor(handler.wrenderer, 0, 0, 0, 255);
        SDL_RenderClear(handler.wrenderer);

        if (handler.keyboard[SDL_SCANCODE_A] && player_ship->box.x > 0)
        {
            player_ship->box.x -= player_ship->vel; 
            player_ship->sprite_type = MF_SPRITE_FLYING;
        }

        if (handler.keyboard[SDL_SCANCODE_D] && (player_ship->box.x + player_ship->box.w) < SCREEN_WIDTH)
        {
            player_ship->box.x += player_ship->vel;
            player_ship->sprite_type = MF_SPRITE_FLYING;
        }

        if (!(handler.keyboard[SDL_SCANCODE_A]) && !(handler.keyboard[SDL_SCANCODE_D]))
        {
            player_ship->sprite_type = MF_SPRITE_IDLE;
        }


        /*
        ==============
        Add new Entities.
        ==============
        */
        ec = DetectEntityCollision(&total_ents);

        if (!(ec.is_sender_destroyed) && ec.is_receiver_destroyed
            && ec.sender == ET_PLAYER && ec.receiver == ET_POWERUP)
        {
            SetBoltComponent(&bolt, 2, 20, SHOT_VEL*2, SHOT_DAMAGE, 0, 255, 0, 255);
            powerup_expired = current_ticks + PW_FAST;
        }

        if (current_ticks > powerup_expired)
        {
            SetBoltComponent(&bolt, 2, 20, SHOT_VEL, SHOT_DAMAGE, 255, 0, 0, 255);
        }

        if (handler.keyboard[SDL_SCANCODE_SPACE]
            && current_ticks > player_next_shot)
        {
            AppendEntityBolt(&total_ents, InitBoltEntity(&bolt, ET_PLAYER_BOLT), &player_ship->box);
            player_next_shot = current_ticks + PLAYER_FT;
        }

        if (current_ticks > aster_next_spawn)
        {
            AppendEntityAster(&total_ents, handler.wrenderer);
            aster_next_spawn = current_ticks + ASTER_CST;
        }

        if (current_ticks > powerup_next_spawn)
        {
            AppendEntityPowerUp(&total_ents);
            powerup_next_spawn = current_ticks + PW_CST;
        }


        /*
        ==============
        Display game score.
        ==============
        */
        current_score += POINTS_PER_FRAME;

        if (ec.is_sender_destroyed && ec.is_receiver_destroyed
            && ec.sender == ET_PLAYER_BOLT && ec.receiver == ET_ASTER)
        {
            current_score += ASTER_POINTS;
        }

        SetScoreScreenText(&game_score, current_score, handler.wrenderer);


        /*
        ==============
        Render everything.
        ==============
        */
        RenderBackground(&background, handler.wrenderer);
        RenderEntities(&total_ents, handler.wrenderer);
        RenderScreenText(&game_score, handler.wrenderer);

        SDL_RenderPresent(handler.wrenderer);

        current_score = 0;

        SetFrameRate(&handler.timer);
    }

    FreeScreenText(&game_score);
    FreeEntities(&total_ents);
    FreeBackgroundTexture(&background);
    FreeHandler(&handler);
    SDL_Quit();
}
