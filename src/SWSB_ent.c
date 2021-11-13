#include "../include/SWSB_ent.h"

static void FreeEntity(Entity *ent)
{
    SDL_DestroyTexture(ent->texture);
    free(ent);
}

static void RemoveEnt(Entities *ents, int index)
{
    FreeEntity(ents->elems[index]);

    for (int i = index; i < (ents->size - 1); i++)
    {
        ents->elems[i] = ents->elems[i + 1];
    }

    ents->size--;
}

static bool IsBoltOutOfBounds(Entity *bolt)
{
    if ((bolt->box.y + bolt->box.h) >= 0)
    {
        return false;
    }

    return true;
}

static bool IsAsterOutOfBounds(Entity *aster)
{
    if (aster->box.y <= SCREEN_HEIGHT)
    {
        return false;
    }

    return true;
}

static void RenderEntityPlayer(Entity *ship, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, ship->texture, NULL, &ship->box);
}

static void RenderEntityBolt(Entity *bolt, SDL_Renderer *renderer)
{
    bolt->box.y -= bolt->vel;
    SDL_SetRenderDrawColor(renderer, bolt->color.r, bolt->color.g, bolt->color.b, bolt->color.a);
    SDL_RenderFillRect(renderer, &bolt->box);
}

static void RenderEntityAster(Entity *aster, SDL_Renderer *renderer)
{
    aster->box.y += aster->vel;
    SDL_RenderCopy(renderer, aster->texture, NULL, &aster->box);
}

Entity *InitTextureEntity(int x, int y, int w, int h,
                            SDL_Renderer *renderer, const char *image_path)
{
    Entity *ent = malloc(sizeof(Entity));

    ent->box.x = x;
    ent->box.y = y;
    ent->box.w = w;
    ent->box.h = h;

    ent->vel = 0;
    ent->firetime = 0;

    ent->color.r = 0;
    ent->color.g = 0;
    ent->color.b = 0;
    ent->color.a = 0;

    ent->texture = GetImageTexture(renderer, image_path);

    ent->IsEntOutOfBounds = NULL;
    ent->RenderEntity = NULL;

    return ent;
}

Entity *InitPlayerEntity(int x, int y, int w, int h,
                        int vel, Uint32 firetime, SDL_Renderer *renderer,
                        const char *image_path)
{
    Entity *ent = InitTextureEntity(x, y, w, h, renderer, image_path);
    ent->vel = vel;
    ent->firetime = firetime;

    ent->RenderEntity = RenderEntityPlayer;

    return ent;
}

Entity *InitBoltEntity(int w, int h, int vel, 
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Entity *ent = malloc(sizeof(Entity));

    ent->box.x = 0;
    ent->box.y = 0;
    ent->box.w = w;
    ent->box.h = h;

    ent->vel = vel;
    ent->firetime = 0;

    ent->color.r = r;
    ent->color.g = g;
    ent->color.b = b;
    ent->color.a = a;

    ent->texture = NULL;

    ent->IsEntOutOfBounds = IsBoltOutOfBounds;
    ent->RenderEntity = RenderEntityBolt;

    return ent;
}

Entity *InitAsteroidEntity(int x, int y, int w, int h,
                            int vel, SDL_Renderer *renderer,
                            const char *image_path)
{
    Entity *aster = InitTextureEntity(x, y, w, h, renderer, image_path);

    aster->vel = vel;
    aster->firetime = 0;

    aster->IsEntOutOfBounds = IsAsterOutOfBounds;
    aster->RenderEntity = RenderEntityAster;

    return aster;
}

void AppenedEntityPlayer(Entities *ents, Entity *player)
{
    ents->elems[ents->size] = player;
    ents->size++;
}

void AppendEntityBolt(Entities *ents, Entity *bolt, SDL_Rect *player_box)
{
    ents->elems[ents->size] = bolt;
    ents->elems[ents->size]->box.x = player_box->x + (player_box->w / 2);
    ents->elems[ents->size]->box.y = player_box->y - ents->elems[ents->size]->box.h;
    ents->size++;
}

void AppendEntityAster(Entities *ents, SDL_Renderer *renderer)
{
    int x = rand() % (SCREEN_WIDTH - ASTER_SIZE);
    ents->elems[ents->size] = InitAsteroidEntity(x, -ASTER_SIZE, ASTER_SIZE, ASTER_SIZE,
                                                    4, renderer, ASTER_GRAY_IMG);
    ents->size++;
}

bool IsEntitiesFull(Entities *ents)
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
    Entity *curr_ent = ents->elems[0];

    // The first index is the player.
    curr_ent->RenderEntity(ents->elems[0], renderer);

    while (i < ents->size)
    {
        curr_ent = ents->elems[i];

        if (curr_ent->IsEntOutOfBounds(curr_ent))
        {
            RemoveEnt(ents, i);
            continue;
        }

        curr_ent->RenderEntity(curr_ent, renderer);
        i++;
    }
}

void FreeEntities(Entities *ents)
{
    for (int i = 0; i < ents->size; i++)
    {
       FreeEntity(ents->elems[i]);
    }
}
