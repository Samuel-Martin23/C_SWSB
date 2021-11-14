#ifndef SWSB_ENT_H
#define SWSB_ENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SWSB_assets.h"

#define PLAYER_ENT      0
#define MAX_ENTS        1024

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

typedef struct
{
    int w;
    int h;
    int vel;
    int damage;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} BoltComponent;

/*
==============
Player Entity
==============
*/
Entity *InitPlayerEntity(SDL_Renderer *renderer);
void AppendEntityPlayer(Entities *ents, Entity *player);


/*
==============
Bolt Entity
==============
*/
Entity *InitBoltEntity(BoltComponent *bolt);
void AppendEntityBolt(Entities *ents, Entity *bolt, SDL_Rect *player_box);
void SetBoltComp(BoltComponent *bolt, int w, int h, int vel,
                    int damage, Uint8 r, Uint8 g, Uint8 b, Uint8 a);


/*
==============
Asteroid Entity
==============
*/
Entity *InitAsteroidEntity(SDL_Renderer *renderer);
void AppendEntityAster(Entities *ents, SDL_Renderer *renderer);


/*
==============
Power Up Entity
==============
*/
Entity *InitPowerUpEntity(void);
void AppendEntityPowerUp(Entities *ents);


/*
==============
Entities
==============
*/
void RenderEntities(Entities *ents, SDL_Renderer *renderer);
void FreeEntities(Entities *ents);

/*
==============
Entity Damages
==============
*/
EntityType DetectEntityCollision(Entities *ents);

#endif /* SWSB_ENT_H */
