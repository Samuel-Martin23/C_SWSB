#ifndef SWSB_ENT_H
#define SWSB_ENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SWSB_assets.h"

#define MAX_ENTS 1024

typedef struct Entity
{
    int vel;
    Uint32 firetime;

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

Entity *InitTextureEntity(int x, int y, int w, int h,
                            SDL_Renderer *renderer, const char *image_path);

Entity *InitPlayerEntity(int x, int y, int w, int h,
                        int vel, Uint32 firetime, SDL_Renderer *renderer,
                        const char *image_path);

Entity *InitBoltEntity(int w, int h, int vel, 
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a);

Entity *InitAsteroidEntity(int x, int y, int w, int h,
                            int vel, SDL_Renderer *renderer,
                            const char *image_path);

void AppenedEntityPlayer(Entities *ents, Entity *player);
void AppendEntityBolt(Entities *ents, Entity *bolt, SDL_Rect *player_box);
void AppendEntityAster(Entities *ents, SDL_Renderer *renderer);

bool IsEntitiesFull(Entities *ents);
void RenderEntities(Entities *ents, SDL_Renderer *renderer);
void FreeEntities(Entities *ents);

#endif /* SWSB_ENT_H */
