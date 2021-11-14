#ifndef SWSB_ENT_H
#define SWSB_ENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SWSB_assets.h"

#define MAX_ENTS 1024

typedef enum
{
    ET_NONE,
    ET_PLAYER,
    ET_BOLT,
    ET_ASTER,
    ET_POWERUP
} EntityType;

typedef struct Entity
{
    int vel;
    int damage;
    int health;
    EntityType type;

    bool (*IsEntOutOfBounds)(struct Entity *ent);
    void (*RenderEntity)(struct Entity *ent, SDL_Renderer *renderer);

    SDL_Rect box;
    SDL_Color color;
    SDL_Texture *texture;
} Entity;

typedef struct Entities
{
    int size;
    int capacity;
    Entity *elems[MAX_ENTS];
} Entities;

Entity *InitPlayerEntity(SDL_Renderer *renderer);
Entity *InitBoltEntity(int w, int h, int vel, int damage,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Entity *InitAsteroidEntity(SDL_Renderer *renderer);
Entity *InitPowerUpEntity(void);

void AppendEntityPlayer(Entities *ents, Entity *player);
void AppendEntityBolt(Entities *ents, Entity *bolt, SDL_Rect *player_box);
void AppendEntityAster(Entities *ents, SDL_Renderer *renderer);
void AppendEntityPowerUp(Entities *ents);

EntityType DetectEntityCollision(Entities *ents);

bool IsEntitiesFull(Entities *ents);
void RenderEntities(Entities *ents, SDL_Renderer *renderer);
void FreeEntities(Entities *ents);

#endif /* SWSB_ENT_H */
