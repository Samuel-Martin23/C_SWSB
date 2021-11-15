#include "../include/SWSB_ent.h"

/*
==============
Player Entity
==============
*/
static void RenderEntityPlayer(Entity *ship, SDL_Renderer *renderer);


/*
==============
Bolt Entity
==============
*/
static bool IsBoltOutOfBounds(Entity *bolt);
static void RenderEntityBolt(Entity *bolt, SDL_Renderer *renderer);


/*
==============
Asteroid Entity
==============
*/
static bool IsAsterOutOfBounds(Entity *aster);
static void RenderEntityAster(Entity *aster, SDL_Renderer *renderer);


/*
==============
Power Up Entity
==============
*/
static bool IsPowerUpOutOfBounds(Entity *power_up);
static void SetRGBPowerUp(Entity *power_up);
static void RenderEntityPowerUp(Entity *power_up, SDL_Renderer *renderer);


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
    Entity *ent = InitTextureEntity(PLAYER_SHIP_X, PLAYER_SHIP_Y, MF_W, MF_H, renderer, MF_IMG);

    ent->vel = PLAYER_VEL;
    ent->damage = PLAYER_DAMAGE;
    ent->health = PLAYER_HEALTH;
    ent->type = ET_PLAYER;

    ent->RenderEntity = RenderEntityPlayer;

    return ent;
}

static void RenderEntityPlayer(Entity *ship, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, ship->texture, NULL, &ship->box);
}

void AppendEntityPlayer(Entities *ents, Entity *player)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size++] = player;
    }
}


/*
==============
Bolt Entity
==============
*/
Entity *InitBoltEntity(BoltComponent *bolt)
{
    Entity *ent = malloc(sizeof(Entity));

    ent->vel = bolt->vel;
    ent->damage = bolt->damage;
    ent->health = 0;
    ent->type = ET_BOLT;

    ent->IsEntOutOfBounds = IsBoltOutOfBounds;
    ent->RenderEntity = RenderEntityBolt;

    ent->box.x = 0;
    ent->box.y = 0;
    ent->box.w = bolt->w;
    ent->box.h = bolt->h;

    ent->color.r = bolt->r;
    ent->color.g = bolt->g;
    ent->color.b = bolt->b;
    ent->color.a = bolt->a;

    ent->texture = NULL;

    return ent;
}

static bool IsBoltOutOfBounds(Entity *bolt)
{
    if (bolt->box.y > 0)
    {
        return false;
    }

    return true;
}

static void RenderEntityBolt(Entity *bolt, SDL_Renderer *renderer)
{
    bolt->box.y -= bolt->vel;
    SDL_SetRenderDrawColor(renderer, bolt->color.r, bolt->color.g, bolt->color.b, bolt->color.a);
    SDL_RenderFillRect(renderer, &bolt->box);
}

void AppendEntityBolt(Entities *ents, Entity *bolt, SDL_Rect *player_box)
{
    if (!(IsEntitiesFull(ents)))
    {
        ents->elems[ents->size] = bolt;
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
    int x = rand() % (SCREEN_WIDTH - ASTER_SIZE);
    Entity *aster = InitTextureEntity(x, -ASTER_SIZE, ASTER_SIZE, ASTER_SIZE, renderer, ASTER_GRAY_IMG);

    aster->vel = ASTER_VEL;
    aster->damage = ASTER_DAMAGE;
    aster->health = ASTER_HEALTH;
    aster->type = ET_ASTER;

    aster->IsEntOutOfBounds = IsAsterOutOfBounds;
    aster->RenderEntity = RenderEntityAster;

    return aster;
}

static bool IsAsterOutOfBounds(Entity *aster)
{
    if (aster->box.y <= SCREEN_HEIGHT)
    {
        return false;
    }

    return true;
}

static void RenderEntityAster(Entity *aster, SDL_Renderer *renderer)
{
    aster->box.y += aster->vel;
    SDL_RenderCopy(renderer, aster->texture, NULL, &aster->box);
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
    Entity *power_up = malloc(sizeof(Entity));

    power_up->vel = PW_VEL;
    power_up->damage = 0;
    power_up->health = 0;
    power_up->type = ET_POWERUP;

    power_up->IsEntOutOfBounds = IsPowerUpOutOfBounds;
    power_up->RenderEntity = RenderEntityPowerUp;

    power_up->box.x = rand() % (SCREEN_WIDTH - PW_WIDTH);
    power_up->box.y = -PW_HEIGHT;
    power_up->box.w = PW_WIDTH;
    power_up->box.h = PW_HEIGHT;

    SetRGBPowerUp(power_up);
    power_up->color.a = 255;

    power_up->texture = NULL;

    return power_up;
}

static bool IsPowerUpOutOfBounds(Entity *power_up)
{
    if (power_up->box.y <= SCREEN_HEIGHT)
    {
        return false;
    }

    return true;
}

static void SetRGBPowerUp(Entity *power_up)
{
    power_up->color.r = (Uint8)(rand() % 256);
    power_up->color.g = (Uint8)(rand() % 256);
    power_up->color.b = (Uint8)(rand() % 256);
}

static void RenderEntityPowerUp(Entity *power_up, SDL_Renderer *renderer)
{
    power_up->box.y += power_up->vel;

    if ((power_up->box.y % 20) == 0)
    {
        SetRGBPowerUp(power_up);
    }

    SDL_SetRenderDrawColor(renderer, power_up->color.r, power_up->color.g, power_up->color.b, power_up->color.a);
    SDL_RenderFillRect(renderer, &power_up->box);
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
Entity Damages
==============
*/
static void DoEntityDamages(Entities *ents, int sender, int receiver)
{
    Entity *ent_sender = ents->elems[sender];
    Entity *ent_receiver = ents->elems[receiver];

    if ((ent_sender->health - ent_receiver->damage) <= 0)
    {
        // Don't remove the player.
        if (sender == PLAYER_ENT)
        {
            ent_sender->health = 0;
            return;
        }

        RemoveEntityFromArray(ents, sender);
    }
    else
    {
        ent_sender->health -= ent_receiver->damage;
    }
}

static bool DidPlayerSendersHitAster(EntityType sender, EntityType receiver)
{
    return ((sender == ET_BOLT || sender == ET_PLAYER) && receiver == ET_ASTER);
}

static bool DidPlayerHitPowerUp(EntityType sender, EntityType receiver)
{
    return (sender == ET_PLAYER && receiver == ET_POWERUP);
}

EntityType DetectEntityCollision(Entities *ents)
{
    EntityType sender = ET_NONE;
    EntityType receiver = ET_NONE;

    for (int i = PLAYER_ENT; i < ents->size; i++)
    {
        for (int j = 1; j < ents->size; j++)
        {
            if (ents->elems[i] != ents->elems[j]
                && SDL_HasIntersection(&ents->elems[i]->box, &ents->elems[j]->box))
            {
                sender = ents->elems[i]->type;
                receiver = ents->elems[j]->type;
        
                if (!(DidPlayerSendersHitAster(sender, receiver)
                    || DidPlayerHitPowerUp(sender, receiver)))
                {
                    continue;
                }

                DoEntityDamages(ents, i, j);
                DoEntityDamages(ents, j, i);

                return receiver;
            }
        }
    }

    return ET_NONE;
}
