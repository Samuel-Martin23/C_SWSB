#include "../include/SWSB_ent.h"

/*
==============
Player Entity
==============
*/
static void RenderEntityPlayer(Entity *player_ent, SDL_Renderer *renderer);


/*
==============
Bolt Entity
==============
*/
static bool IsBoltOutOfBounds(Entity *bolt_ent);
static void RenderEntityBolt(Entity *bolt_ent, SDL_Renderer *renderer);


/*
==============
Asteroid Entity
==============
*/
static bool IsAsterOutOfBounds(Entity *aster_ent);
static void RenderEntityAster(Entity *aster_ent, SDL_Renderer *renderer);


/*
==============
Power Up Entity
==============
*/
static bool IsPowerUpOutOfBounds(Entity *power_up_ent);
static void SetRGBPowerUp(Entity *power_up_ent);
static void RenderEntityPowerUp(Entity *power_up_ent, SDL_Renderer *renderer);


/*
==============
Explosion Entity
==============
*/
static bool IsExploOutOfBounds(Entity *explo_ent);
static void RenderEntityExplo(Entity *explo_ent, SDL_Renderer *renderer);


/*
==============
Entities
==============
*/
static bool IsEntitiesFull(Entities *ents);


/*
==============
Player Entity
==============
*/
Entity *InitPlayerEntity(SDL_Renderer *renderer)
{
    Entity *player_ent = malloc(sizeof(Entity));

    if (player_ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    char *sprites[] = {MF_IDLE_IMG, MF_FLYING_IMG};

    player_ent->vel = PLAYER_VEL;
    player_ent->damage = PLAYER_DAMAGE;
    player_ent->health = PLAYER_HEALTH;
    player_ent->ent_type = ET_PLAYER;

    player_ent->IsEntOutOfBounds = NULL;
    player_ent->RenderEntity = RenderEntityPlayer;

    player_ent->box.x = PLAYER_X;
    player_ent->box.y = PLAYER_Y;
    player_ent->box.w = PLAYER_W;
    player_ent->box.h = PLAYER_H;

    player_ent->color.r = 0;
    player_ent->color.g = 0;
    player_ent->color.b = 0;
    player_ent->color.a = 255;

    player_ent->sprite_speed = 0;
    player_ent->sprite_frames = MF_SPRITE_NUM - MF_SPRITE_IDLE;
    player_ent->sprite_type = MF_SPRITE_IDLE;
    player_ent->next_sprite = 0;

    memset(player_ent->textures, 0, sizeof(player_ent->textures));

    for (Uint32 i = 0; i < player_ent->sprite_frames; i++)
    {
        player_ent->textures[i] = GetImageTexture(renderer, sprites[i]);
    }

    return player_ent;
}

static void RenderEntityPlayer(Entity *player_ent, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, player_ent->textures[player_ent->sprite_type], NULL, &player_ent->box);
}

void AppendEntityPlayer(Entities *ents, Entity *player_ent)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = player_ent;
    }
}


/*
==============
Bolt Entity
==============
*/
Entity *InitBoltEntity(BoltComponent *bolt, EntityType type)
{
    Entity *bolt_ent = malloc(sizeof(Entity));

    if (bolt_ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    bolt_ent->vel = bolt->vel;
    bolt_ent->damage = bolt->damage;
    bolt_ent->health = 0;
    bolt_ent->ent_type = type;

    bolt_ent->IsEntOutOfBounds = IsBoltOutOfBounds;
    bolt_ent->RenderEntity = RenderEntityBolt;

    bolt_ent->box.x = 0;
    bolt_ent->box.y = 0;
    bolt_ent->box.w = bolt->w;
    bolt_ent->box.h = bolt->h;

    bolt_ent->color.r = bolt->r;
    bolt_ent->color.g = bolt->g;
    bolt_ent->color.b = bolt->b;
    bolt_ent->color.a = bolt->a;

    bolt_ent->sprite_speed = 0;
    bolt_ent->sprite_frames = 0;
    bolt_ent->sprite_type = 0;
    bolt_ent->next_sprite = 0;

    memset(bolt_ent->textures, 0, sizeof(bolt_ent->textures));

    return bolt_ent;
}

static bool IsBoltOutOfBounds(Entity *bolt_ent)
{
    if (bolt_ent->box.y > 0)
    {
        return false;
    }

    return true;
}

static void RenderEntityBolt(Entity *bolt_ent, SDL_Renderer *renderer)
{
    bolt_ent->box.y -= bolt_ent->vel;
    SDL_SetRenderDrawColor(renderer, bolt_ent->color.r, bolt_ent->color.g,
                            bolt_ent->color.b, bolt_ent->color.a);
    SDL_RenderFillRect(renderer, &bolt_ent->box);
}

void AppendEntityBolt(Entities *ents, Entity *bolt_ent, SDL_Rect *player_box)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size] = bolt_ent;
        ents->elems[ents->size]->box.x = player_box->x + (player_box->w / 2);
        ents->elems[ents->size]->box.y = player_box->y - ents->elems[ents->size]->box.h;
        ents->size++;
    }
}

void SetBoltComponent(BoltComponent *bolt, int w, int h, int vel,
                        int damage, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    bolt->w = w;
    bolt->h = h;
    bolt->vel = vel;
    bolt->damage = damage;
    bolt->r = r;
    bolt->g = g;
    bolt->b = b;
    bolt->a = a;
}


/*
==============
Asteroid Entity
==============
*/
Entity *InitAsteroidEntity(SDL_Renderer *renderer)
{
    Entity *aster_ent = malloc(sizeof(Entity));

    if (aster_ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    bool is_aster_gray = (bool)rand_int(0, 3);
    char *sprites_gray[] = {ASTER_GRAY_IMG_1, ASTER_GRAY_IMG_2, ASTER_GRAY_IMG_3, ASTER_GRAY_IMG_4};
    char *sprites_brown[] = {ASTER_BROWN_IMG_1, ASTER_BROWN_IMG_2, ASTER_BROWN_IMG_3, ASTER_BROWN_IMG_4};
    const char *current_sprite = "";

    aster_ent->vel = ASTER_VEL;
    aster_ent->damage = ASTER_DAMAGE;
    aster_ent->health = ASTER_HEALTH;
    aster_ent->ent_type = ET_ASTER;

    aster_ent->IsEntOutOfBounds = IsAsterOutOfBounds;
    aster_ent->RenderEntity = RenderEntityAster;

    aster_ent->box.w = rand_int(ASTER_MIN_SIZE, ASTER_MAX_SIZE);
    aster_ent->box.h = aster_ent->box.w;
    aster_ent->box.x = rand_int(0, (SCREEN_WIDTH - (aster_ent->box.w + PLAYER_HALF_W))) + PLAYER_QTR_W;
    aster_ent->box.y = -aster_ent->box.h;

    aster_ent->color.r = 0;
    aster_ent->color.g = 0;
    aster_ent->color.b = 0;
    aster_ent->color.a = 255;

    aster_ent->sprite_speed = (Uint32)(rand_int(0, 10) + ASTER_SS);
    aster_ent->sprite_frames = (ASTER_SPRITE_NUM - ASTER_SPRITE_1);
    aster_ent->sprite_type = (SpriteType)rand_int(ASTER_SPRITE_1, ASTER_SPRITE_4);
    aster_ent->next_sprite = 0;

    memset(aster_ent->textures, 0, sizeof(aster_ent->textures));

    for (Uint32 i = 0; i < aster_ent->sprite_frames; i++)
    {
        current_sprite = (is_aster_gray) ? sprites_gray[i] : sprites_brown[i];
        aster_ent->textures[i] = GetImageTexture(renderer, current_sprite);
    } 

    return aster_ent;
}

static bool IsAsterOutOfBounds(Entity *aster_ent)
{
    if ((aster_ent->box.y + (aster_ent->box.h / 3)) >= SCREEN_HEIGHT)
    {
        return true;
    }

    return false;
}

static SpriteType GetAsterSprite(Entity *aster_ent)
{
    if (aster_ent->sprite_type == ASTER_SPRITE_NUM)
    {
        aster_ent->sprite_type = ASTER_SPRITE_1;
    }

    SpriteType curr_sprite = aster_ent->sprite_type;

    if (GetStopWatchTicks() > aster_ent->next_sprite)
    {
        aster_ent->sprite_type++;
        aster_ent->next_sprite = aster_ent->sprite_speed + GetStopWatchTicks();
    }

    return (curr_sprite - ASTER_SPRITE_1);
}

static void RenderEntityAster(Entity *aster_ent, SDL_Renderer *renderer)
{
    aster_ent->box.y += aster_ent->vel;
    SDL_RenderCopy(renderer, aster_ent->textures[GetAsterSprite(aster_ent)], NULL, &aster_ent->box);
}

void AppendEntityAster(Entities *ents, SDL_Renderer *renderer)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = InitAsteroidEntity(renderer);
    }
}


/*
==============
Power Up Entity
==============
*/
Entity *InitPowerUpEntity(void)
{
    Entity *power_up_ent = malloc(sizeof(Entity));

    if (power_up_ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    power_up_ent->vel = PW_VEL;
    power_up_ent->damage = 0;
    power_up_ent->health = 0;
    power_up_ent->ent_type = ET_POWERUP;

    power_up_ent->IsEntOutOfBounds = IsPowerUpOutOfBounds;
    power_up_ent->RenderEntity = RenderEntityPowerUp;

    power_up_ent->box.x = rand_int(0, SCREEN_WIDTH - PW_WIDTH);
    power_up_ent->box.y = -PW_HEIGHT;
    power_up_ent->box.w = PW_WIDTH;
    power_up_ent->box.h = PW_HEIGHT;

    SetRGBPowerUp(power_up_ent);
    power_up_ent->color.a = 255;

    power_up_ent->sprite_speed = 0;
    power_up_ent->sprite_frames = 0;
    power_up_ent->sprite_type = 0;
    power_up_ent->next_sprite = 0;

    memset(power_up_ent->textures, 0, sizeof(power_up_ent->textures));

    return power_up_ent;
}

static bool IsPowerUpOutOfBounds(Entity *power_up_ent)
{
    if (power_up_ent->box.y <= SCREEN_HEIGHT)
    {
        return false;
    }

    return true;
}

static void RenderEntityPowerUp(Entity *power_up_ent, SDL_Renderer *renderer)
{
    power_up_ent->box.y += power_up_ent->vel;

    if ((power_up_ent->box.y % 20) == 0)
    {
        SetRGBPowerUp(power_up_ent);
    }

    SDL_SetRenderDrawColor(renderer, power_up_ent->color.r, power_up_ent->color.g,
                            power_up_ent->color.b, power_up_ent->color.a);
    SDL_RenderFillRect(renderer, &power_up_ent->box);
}

static void SetRGBPowerUp(Entity *power_up_ent)
{
    power_up_ent->color.r = (Uint8)(rand_int(10, 255));
    power_up_ent->color.g = (Uint8)(rand_int(10, 255));
    power_up_ent->color.b = (Uint8)(rand_int(10, 255));
}

void AppendEntityPowerUp(Entities *ents)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = InitPowerUpEntity();
    }
}


/*
==============
Explosion Entity
==============
*/
Entity *InitExploEntity(SDL_Rect *ent_rect, SDL_Renderer *renderer)
{
    Entity *explo_ent = malloc(sizeof(Entity));

    if (explo_ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    char *sprites[] = {EXPLO_IMG_1, EXPLO_IMG_2, EXPLO_IMG_3, EXPLO_IMG_4,
                        EXPLO_IMG_5, EXPLO_IMG_6, EXPLO_IMG_7, EXPLO_IMG_8};

    explo_ent->vel = EXPLO_VEL;
    explo_ent->damage = 0;
    explo_ent->health = 0;
    explo_ent->ent_type = ET_EXPLO;

    explo_ent->IsEntOutOfBounds = IsExploOutOfBounds;
    explo_ent->RenderEntity = RenderEntityExplo;

    explo_ent->box = *ent_rect;

    explo_ent->color.r = 0;
    explo_ent->color.g = 0;
    explo_ent->color.b = 0;
    explo_ent->color.a = 255;

    explo_ent->sprite_speed = EXPLO_SS;
    explo_ent->sprite_frames = (EXPLO_SPRITE_NUM - EXPLO_SPRITE_1);
    explo_ent->sprite_type = EXPLO_SPRITE_1;
    explo_ent->next_sprite = explo_ent->sprite_speed + GetStopWatchTicks();

    memset(explo_ent->textures, 0, sizeof(explo_ent->textures));

    for (Uint32 i = 0; i < explo_ent->sprite_frames; i++)
    {
        explo_ent->textures[i] = GetImageTexture(renderer, sprites[i]);
    }

    return explo_ent;
}

static bool IsExploOutOfBounds(Entity *explo_ent)
{
    if (explo_ent->sprite_type == EXPLO_SPRITE_NUM)
    {
        return true;
    }

    return false;
}

static SpriteType GetExploSprite(Entity *explo_ent)
{
    SpriteType curr_sprite = explo_ent->sprite_type;

    if (GetStopWatchTicks() > explo_ent->next_sprite)
    {
        explo_ent->sprite_type++;
        explo_ent->next_sprite = explo_ent->sprite_speed + GetStopWatchTicks();
    }

    return (curr_sprite - EXPLO_SPRITE_1);
}

static void RenderEntityExplo(Entity *explo_ent, SDL_Renderer *renderer)
{
    explo_ent->box.y += explo_ent->vel;
    SDL_RenderCopy(renderer, explo_ent->textures[GetExploSprite(explo_ent)], NULL, &explo_ent->box);
}

void AppendEntityExplo(Entities *ents, Entity *explo_ent)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = explo_ent;
    }
}


/*
==============
Entities
==============
*/
static void FreeEntity(Entities *ents, int index)
{
    int i = 0;

    while (ents->elems[index]->textures[i] != NULL)
    {
        SDL_DestroyTexture(ents->elems[index]->textures[i]);
        i++;
    }

    free(ents->elems[index]);
}

static void RemoveEntityFromArray(Entities *ents, int index)
{
    FreeEntity(ents, index);

    for (int i = index; i < (ents->size - 1); i++)
    {
        ents->elems[i] = ents->elems[i + 1];
    }

    ents->size--;
}

static bool IsEntitiesFull(Entities *ents)
{
    if (ents->size == ents->capacity)
    {
        return true;
    }

    return false;
}

void RenderEntities(Entities *ents, SDL_Renderer *renderer)
{
    int i = PLAYER_ENT;
    Entity *curr_ent = NULL;

    while (i < ents->size)
    {
        curr_ent = ents->elems[i];

        // Some Entities might not have an out of bounds function.
        if (curr_ent->IsEntOutOfBounds && curr_ent->IsEntOutOfBounds(curr_ent))
        {
            RemoveEntityFromArray(ents, i);
            continue;
        }

        curr_ent->RenderEntity(curr_ent, renderer);
        i++;
    }
}

void FreeEntities(Entities *ents)
{
    for (int i = PLAYER_ENT; i < ents->size; i++)
    {
        FreeEntity(ents, i);
    }
}


/*
==============
Entity Collision
==============
*/
static void SetEmptyEntityCollision(EntityCollision *ec)
{
    ec->sender = ET_NONE;
    ec->receiver = ET_NONE;
    ec->is_sender_destroyed = false;
    ec->is_receiver_destroyed = false;
}

static bool DoEntityDamages(Entities *ents, int sender, int receiver)
{
    Entity *ent_sender = ents->elems[sender];
    Entity *ent_receiver = ents->elems[receiver];
    int ent_health = ent_sender->health - ent_receiver->damage;

    if (ent_health <= 0)
    {
        // Don't remove the player.
        if (sender == PLAYER_ENT)
        {
            ent_sender->health = 0;
            return false;
        }

        return true;
    }
    else
    {
        ent_sender->health = ent_health;
    }

    return false;
}

static bool DidPlayerHitPowerUp(EntityType sender, EntityType receiver)
{
    return (sender == ET_PLAYER && receiver == ET_POWERUP);
}

static bool DidPlayerSendersHitAster(EntityType sender, EntityType receiver)
{
    return ((sender == ET_PLAYER || sender == ET_PLAYER_BOLT) && receiver == ET_ASTER);
}

EntityCollision DetectEntityCollision(Entities *ents, SDL_Renderer *renderer)
{
    EntityCollision ec = {ET_NONE, ET_NONE, false, false};

    for (int i = PLAYER_ENT; i < ents->size; i++)
    {
        for (int j = 1; j < ents->size; j++)
        {
            if (ents->elems[i] != ents->elems[j]
                && SDL_HasIntersection(&ents->elems[i]->box, &ents->elems[j]->box))
            {
                ec.sender = ents->elems[i]->ent_type;
                ec.receiver = ents->elems[j]->ent_type;
        
                if (!(DidPlayerSendersHitAster(ec.sender, ec.receiver)
                    || DidPlayerHitPowerUp(ec.sender, ec.receiver)))
                {
                    continue;
                }

                // I need to check both of them before I free
                // either one of them.
                ec.is_sender_destroyed = DoEntityDamages(ents, i, j);
                ec.is_receiver_destroyed = DoEntityDamages(ents, j, i);

                if (ec.is_sender_destroyed)
                {
                    RemoveEntityFromArray(ents, i);
                }

                if (ec.is_receiver_destroyed)
                {
                    // If the receiver was destroyed and
                    // it was an asteroid then play an
                    // explosion effect.
                    if (ec.receiver == ET_ASTER)
                    {
                        AppendEntityExplo(ents, InitExploEntity(&ents->elems[j]->box, renderer));
                    }

                    RemoveEntityFromArray(ents, j);
                }

                return ec;
            }
        }
    }

    SetEmptyEntityCollision(&ec);
    return ec;
}
