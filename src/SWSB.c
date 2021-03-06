#include "../include/SWSB.h"

void RunGame(Handler *handler, Background *background)
{
    Entities total_ents = {0, {NULL}};
    ScreenText game_score = InitScreenTextScore(handler->wrenderer, "000000000");
    ScreenText restart_msg = InitScreenTextStart(handler->wrenderer, "PRESS SPACE TO RESTART");

    Entity *player_ship = NULL;
    EntityInteraction ei = {0, 0, ET_NONE, ET_NONE, false, false};
    BoltComponent bolt = {2, 20, PLAYER_BOLT_VEL, PLAYER_BOLT_DAMAGE, 255, 0, 0, 255};
    Uint32 ents_next_spawn[ET_NUM_ENTITIES] = {0, 0, 0, ASTER_IST, PW_IST, 0, TF_IST, 0};

    Uint32 powerup_expired = 0;
    Uint32 current_ticks = 0;

    int current_score = 0;
    bool is_paused = false;

    AppendEntityPlayer(&total_ents, InitPlayerEntity(handler->wrenderer));

    StartStopWatch();

    while (true)
    {
        SetIterationTicks();

        UpdateKeyboard(handler);

        /*
        ==============
        Quit or pause game.
        ==============
        */
        SDL_PollEvent(&handler->event);

        if (handler->event.type == SDL_QUIT)
        {
            handler->game_state = GAME_QUIT;
            break;
        }

        if (GetKeyState(handler, SDL_SCANCODE_ESCAPE) & KEY_PRESSED)
        {
            is_paused = !(is_paused);
        }

        if (is_paused)
        {
            PauseStopWatch();
            continue;
        }

        ResumeStopWatch();

        current_ticks = GetStopWatchTicks();


        /*
        ==============
        Update Player.
        ==============
        */
        SDL_SetRenderDrawColor(handler->wrenderer, 0, 0, 0, 255);
        SDL_RenderClear(handler->wrenderer);

        player_ship = total_ents.elems[PLAYER_ENT];

        if ((GetKeyState(handler, SDL_SCANCODE_A) & (KEY_PRESSED|KEY_HELD_DOWN))
            && player_ship && player_ship->src_rect.x > 0)
        {
            player_ship->src_rect.x -= player_ship->vel; 
            player_ship->sprite_type = MF_SPRITE_FLYING;
        }

        if ((GetKeyState(handler, SDL_SCANCODE_D) & (KEY_PRESSED|KEY_HELD_DOWN))
            && player_ship && (player_ship->src_rect.x + player_ship->src_rect.w) < SCREEN_WIDTH)
        {
            player_ship->src_rect.x += player_ship->vel;
            player_ship->sprite_type = MF_SPRITE_FLYING;
        }

        if ((GetKeyState(handler, SDL_SCANCODE_A) & KEY_UNPRESSED) 
            && (GetKeyState(handler, SDL_SCANCODE_D) & KEY_UNPRESSED) && player_ship)
        {
            player_ship->sprite_type = MF_SPRITE_IDLE;
        }


        /*
        ==============
        Entity Interaction.
        ==============
        */
        EntityEnemyDetection(&total_ents);

        if (IsEntityCollision(&ei, &total_ents))
        {
            DoEntityDamages(&ei, &total_ents, handler->wrenderer);

             // If we got a powerup.
            if (!(ei.is_sender_destroyed) && ei.is_receiver_destroyed
                && ei.sender_type == ET_PLAYER && ei.receiver_type == ET_POWERUP)
            {
                SetBoltComponent(&bolt, 2, 20, PLAYER_BOLT_VEL*2, PLAYER_BOLT_DAMAGE, 255, 255, 0, 255);
                powerup_expired = current_ticks + PW_FAST;
            }
        }

        /*
        ==============
        Add new Entities.
        ==============
        */
        // If our power up is expired, set it back
        // to a default shot.
        if (current_ticks > powerup_expired)
        {
            SetBoltComponent(&bolt, 2, 20, PLAYER_BOLT_VEL, PLAYER_BOLT_DAMAGE, 255, 0, 0, 255);
        }

        // If the user fires, spawn a new bolt.
        if ((GetKeyState(handler, SDL_SCANCODE_SPACE) & (KEY_PRESSED|KEY_HELD_DOWN))
            && player_ship && current_ticks > player_ship->next_bolt)
        {
            AppendEntityBolt(&total_ents, InitBoltEntity(&bolt, ET_PLAYER_BOLT), &player_ship->src_rect);
            player_ship->next_bolt = current_ticks + PLAYER_BOLT_FT;
        }

        // If our time is up, spawn a new asteroid.
        if (current_ticks > ents_next_spawn[ET_ASTER])
        {
            AppendEntityAster(&total_ents, handler->wrenderer);
            ents_next_spawn[ET_ASTER] = current_ticks + ASTER_CST;
        }

        // If our time is up, spawn a new power up.
        if (current_ticks > ents_next_spawn[ET_POWERUP])
        {
            AppendEntityPowerUp(&total_ents);
            ents_next_spawn[ET_POWERUP] = current_ticks + PW_CST;
        }

        // If our time is up, spawn a new power up.
        if (current_ticks > ents_next_spawn[ET_ENEMY_SHIP])
        {
            AppendEntityEnemyShip(&total_ents, handler->wrenderer);
            ents_next_spawn[ET_ENEMY_SHIP] = current_ticks + TF_CST;
        }


        /*
        ==============
        Display game score.
        ==============
        */
        current_score += POINTS_PER_FRAME;

        if (ei.is_sender_destroyed && ei.is_receiver_destroyed
            && ei.sender_type == ET_PLAYER_BOLT && ei.receiver_type == ET_ASTER)
        {
            current_score += ASTER_POINTS;
        }

        if (ei.is_sender_destroyed && ei.is_receiver_destroyed
            && ei.sender_type == ET_PLAYER_BOLT && ei.receiver_type == ET_ENEMY_SHIP)
        {
            current_score += TF_POINTS;
        }

        if (!(ei.is_sender_destroyed) && ei.is_receiver_destroyed
            && ei.sender_type == ET_PLAYER && ei.receiver_type == ET_ENEMY_SHIP)
        {
            current_score += TF_PLAYER_HIT_POINTS;
        }

        if (player_ship)
        {
            SetScoreScreenText(&game_score, current_score, handler->wrenderer);
            ents_next_spawn[ET_PLAYER] = current_ticks + PLAYER_NEXT_SPAWN;
        }


        /*
        ==============
        Render everything.
        ==============
        */
        RenderBackground(background, handler->wrenderer);
        RenderEntities(&total_ents, handler->wrenderer);
        RenderScreenText(&game_score, handler->wrenderer);

        // If the player is dead, when can we spawn again?
        if (!(player_ship) && current_ticks > ents_next_spawn[ET_PLAYER])
        {
            // Need to render some text.
            RenderScreenText(&restart_msg, handler->wrenderer);

            if (GetKeyState(handler, SDL_SCANCODE_SPACE) & KEY_PRESSED)
            {
                handler->game_state = GAME_RESTART;
                break;
            }
        }

        SDL_RenderPresent(handler->wrenderer);

        current_score = 0;

        SetFrameRate();
    }

    FreeScreenText(&restart_msg);
    FreeScreenText(&game_score);
    FreeEntities(&total_ents);
}

bool GameDidStart(Handler *handler, Background *background)
{
    bool start_game = true;
    Entity *player = InitPlayerEntity(handler->wrenderer);
    ScreenText start_msg = InitScreenTextStart(handler->wrenderer, "PRESS SPACE TO START");

    while (true)
    {
        SetIterationTicks();

        UpdateKeyboard(handler);

        SDL_SetRenderDrawColor(handler->wrenderer, 0, 0, 0, 255);
        SDL_RenderClear(handler->wrenderer);

        SDL_PollEvent(&handler->event);

        if (handler->event.type == SDL_QUIT)
        {
            start_game = false;
            break;
        }

        if (GetKeyState(handler, SDL_SCANCODE_SPACE) & KEY_PRESSED)
        {
            break;
        }

        RenderBackground(background, handler->wrenderer);
        player->RenderEntity(player, handler->wrenderer);
        RenderScreenText(&start_msg, handler->wrenderer);

        SDL_RenderPresent(handler->wrenderer);

        SetFrameRate();
    }

    FreeEntity(player);
    FreeScreenText(&start_msg);

    return start_game;
}

void RunSWSB(void)
{
    Handler handler = InitHandler(GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    Background background = InitBackground(handler.wrenderer);

    if (GameDidStart(&handler, &background))
    {
        while (true)
        {
            RunGame(&handler, &background);

            if (handler.game_state == GAME_QUIT)
            {
                break;
            }

            if (handler.game_state == GAME_RESTART)
            {
                SetBackground(&background);
            }
        }
    }

    FreeBackgroundTexture(&background);
    FreeHandler(&handler);
    SDL_Quit();
}
