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
Entities
==============
*/
static bool IsEntitiesFull(Entities *ents);


static Entity *InitTextureEntity(int x, int y, int w, int h,
                            SDL_Renderer *renderer, const char *image_path)
{
    Entity *ent = malloc(sizeof(Entity));

    if (ent == NULL)
    {
        printf("Could not allocate memory.\n");
        exit(1);
    }

    ent->vel = 0;
    ent->damage = 0;
    ent->health = 0;
    ent->type = ET_NONE;

    ent->IsEntOutOfBounds = NULL;
    ent->RenderEntity = NULL;

    ent->box.x = x;
    ent->box.y = y;
    ent->box.w = w;
    ent->box.h = h;

    ent->color.r = 0;
    ent->color.g = 0;
    ent->color.b = 0;
    ent->color.a = 255;

    ent->texture = GetImageTexture(renderer, image_path);

    return ent;
}


/*
==============
Player Entity
==============
*/
Entity *InitPlayerEntity(SDL_Renderer *renderer)
{
    Entity *player_ent = InitTextureEntity(PLAYER_X, PLAYER_Y, PLAYER_W, PLAYER_H, renderer, MF_IMG);

    player_ent->vel = PLAYER_VEL;
    player_ent->damage = PLAYER_DAMAGE;
    player_ent->health = PLAYER_HEALTH;
    player_ent->type = ET_PLAYER;

    player_ent->RenderEntity = RenderEntityPlayer;

    return player_ent;
}

static void RenderEntityPlayer(Entity *player_ent, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, player_ent->texture, NULL, &player_ent->box);
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
    bolt_ent->type = type;

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

    bolt_ent->texture = NULL;

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
    int x = (rand() % (SCREEN_WIDTH - (ASTER_SIZE + PLAYER_HALF_W))) + PLAYER_QTR_W;
    const char *aster_img = (rand() % 4) ? ASTER_GRAY_IMG : ASTER_BROWN_IMG;
    Entity *aster_ent = InitTextureEntity(x, -ASTER_SIZE, ASTER_SIZE, ASTER_SIZE, renderer, aster_img);

    aster_ent->vel = ASTER_VEL;
    aster_ent->damage = ASTER_DAMAGE;
    aster_ent->health = ASTER_HEALTH;
    aster_ent->type = ET_ASTER;

    aster_ent->IsEntOutOfBounds = IsAsterOutOfBounds;
    aster_ent->RenderEntity = RenderEntityAster;

    return aster_ent;
}

static bool IsAsterOutOfBounds(Entity *aster_ent)
{
    if ((aster_ent->box.y + (aster_ent->box.h/3)) >= SCREEN_HEIGHT)
    {
        return true;
    }

    return false;
}

static void RenderEntityAster(Entity *aster_ent, SDL_Renderer *renderer)
{
    aster_ent->box.y += aster_ent->vel;
    SDL_RenderCopy(renderer, aster_ent->texture, NULL, &aster_ent->box);
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
    power_up_ent->type = ET_POWERUP;

    power_up_ent->IsEntOutOfBounds = IsPowerUpOutOfBounds;
    power_up_ent->RenderEntity = RenderEntityPowerUp;

    power_up_ent->box.x = rand() % (SCREEN_WIDTH - PW_WIDTH);
    power_up_ent->box.y = -PW_HEIGHT;
    power_up_ent->box.w = PW_WIDTH;
    power_up_ent->box.h = PW_HEIGHT;

    SetRGBPowerUp(power_up_ent);
    power_up_ent->color.a = 255;

    power_up_ent->texture = NULL;

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

static void SetRGBPowerUp(Entity *power_up_ent)
{
    power_up_ent->color.r = (Uint8)(rand() % 256);
    power_up_ent->color.g = (Uint8)(rand() % 256);
    power_up_ent->color.b = (Uint8)(rand() % 256);
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

void AppendEntityPowerUp(Entities *ents)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = InitPowerUpEntity();
    }
}


/*
==============
Entities
==============
*/
static void FreeEntity(Entities *ents, int index)
{
    SDL_DestroyTexture(ents->elems[index]->texture);
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
    int i = 1;
    Entity *curr_ent = ents->elems[PLAYER_ENT];

    // The first index is the player.
    curr_ent->RenderEntity(curr_ent, renderer);

    while (i < ents->size)
    {
        curr_ent = ents->elems[i];

        if (curr_ent->IsEntOutOfBounds(curr_ent))
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

EntityCollision DetectEntityCollision(Entities *ents)
{
    EntityCollision ec = {ET_NONE, ET_NONE, false, false};

    for (int i = PLAYER_ENT; i < ents->size; i++)
    {
        for (int j = 1; j < ents->size; j++)
        {
            if (ents->elems[i] != ents->elems[j]
                && SDL_HasIntersection(&ents->elems[i]->box, &ents->elems[j]->box))
            {
                ec.sender = ents->elems[i]->type;
                ec.receiver = ents->elems[j]->type;
        
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
                    RemoveEntityFromArray(ents, j);
                }

                return ec;
            }
        }
    }

    SetEmptyEntityCollision(&ec);
    return ec;
}
