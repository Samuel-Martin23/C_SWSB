#ifndef SWSB_ENT_H
#define SWSB_ENT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SWSB_assets.h"

typedef struct
{
    int vel;
    Uint32 firetime;

    SDL_Rect box;
    SDL_Color color;
    SDL_Texture *texture;
} Entity;

typedef struct
{
    int size;
    int capacity;
    Uint32 next_shot;
    Entity *shots;
} Bolts;

Entity InitEntityTexture(int x, int y, int w, int h,
                    int vel, SDL_Renderer *renderer,
                    const char *image_path);
void RenderEntityTexture(Entity *ent, SDL_Renderer *renderer);
void FreeEntityTexture(Entity *ent);

Entity InitEntityBolt(int w, int h, int vel, Uint32 firetime,
                Uint8 r, Uint8 g, Uint8 b, Uint8 a);
Bolts InitBolts(int capacity);
void AppendBolts(Bolts *bolts, Entity *shot, SDL_Rect *ent_box);
bool IsBoltsFull(Bolts *bolts);
void RenderBolts(Bolts *bolts, SDL_Renderer *renderer);
void FreeBolts(Bolts *bolts);

#endif /* SWSB_ENT_H */
