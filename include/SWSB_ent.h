#ifndef SWSB_ENT_H
#define SWSB_ENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SWSB_assets.h"
#include "SWSB_timer.h"

#define PLAYER_ENT      0
#define MAX_ENTS        1024

#define MAX_TEXTURES    10

typedef enum
{
    ET_NONE,
    ET_PLAYER,
    ET_PLAYER_BOLT,
    ET_ASTER,
    ET_POWERUP,
    ET_EXPLO,
    ET_ENEMY_SHIP,
    ET_ENEMY_BOLT,

    ET_NUM_ENTITIES
} EntityType;

typedef enum
{
    MF_SPRITE_IDLE,
    MF_SPRITE_FLYING,

    MF_SPRITE_NUM,


    ASTER_SPRITE_1,
    ASTER_SPRITE_2,
    ASTER_SPRITE_3,
    ASTER_SPRITE_4,

    ASTER_SPRITE_NUM,


    EXPLO_SPRITE_1,
    EXPLO_SPRITE_2,
    EXPLO_SPRITE_3,
    EXPLO_SPRITE_4,
    EXPLO_SPRITE_5,
    EXPLO_SPRITE_6,
    EXPLO_SPRITE_7,
    EXPLO_SPRITE_8,

    EXPLO_SPRITE_NUM,


    TF_SPRITE_IDLE,
    TF_SPRITE_NUM
} SpriteType;

typedef struct Entity
{
    int vel;
    int damage;
    int health;
    Uint32 next_bolt;
    EntityType ent_type;

    bool (*IsEntOutOfBounds)(struct Entity *ent);
    void (*RenderEntity)(struct Entity *ent, SDL_Renderer *renderer);

    SDL_Rect src_rect;
    SDL_Color color;

    SDL_Rect tracer;

    Uint32 sprite_speed;
    Uint32 sprite_frames;
    SpriteType sprite_type;
    Uint32 next_sprite;
    SDL_Texture *textures[MAX_TEXTURES];
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

typedef struct
{
    int sender_index;
    int receiver_index;
    EntityType sender_type;
    EntityType receiver_type;
    bool is_sender_destroyed;
    bool is_receiver_destroyed;
} EntityInteraction;

/*
==============
Player Entity
==============
*/
Entity *InitPlayerEntity(SDL_Renderer *renderer);
void AppendEntityPlayer(Entities *ents, Entity *player_ent);


/*
==============
Bolt Entity
==============
*/
Entity *InitBoltEntity(BoltComponent *bolt, EntityType type);
void AppendEntityBolt(Entities *ents, Entity *bolt_ent, SDL_Rect *ship_box);
void SetBoltComponent(BoltComponent *bolt, int w, int h, int vel,
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
Explosion Entity
==============
*/
Entity *InitExploEntity(SDL_Rect *ent_rect, SDL_Renderer *renderer);
void AppendEntityExplo(Entities *ents, Entity *explo_ent);


/*
==============
Enemy Ship Entity
==============
*/
Entity *InitEnemyShipEntity(SDL_Renderer *renderer);
void AppendEntityEnemyShip(Entities *ents, SDL_Renderer *renderer);


/*
==============
Entities
==============
*/
void RenderEntities(Entities *ents, SDL_Renderer *renderer);
void FreeEntities(Entities *ents);

/*
==============
Entity Interaction
==============
*/
void EntityEnemyDetection(Entities *ents);
bool IsEntityCollision(EntityInteraction *ei, Entities *ents);
void DoEntityDamages(EntityInteraction *ei, Entities *ents, SDL_Renderer *renderer);

#endif /* SWSB_ENT_H */
