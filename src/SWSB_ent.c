#include "../include/SWSB_ent.h"

Entity InitEntityTexture(int x, int y, int w, int h,
                    int vel, SDL_Renderer *renderer,
                    const char *image_path)
{
    Entity ent;

    ent.box.x = x;
    ent.box.y = y;
    ent.box.w = w;
    ent.box.h = h;
    ent.vel = vel;

    ent.texture = GetImageTexture(renderer, image_path);

    return ent;
}

void RenderEntityTexture(Entity *ent, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, ent->texture, NULL, &ent->box);
}

void FreeEntityTexture(Entity *ent)
{
    SDL_DestroyTexture(ent->texture);
}

/* Bolt Stuff */
static void RemoveBolt(Bolts *bq)
{
    for (int i = 0; i < (bq->size - 1); i++)
    {
        bq->shots[i] = bq->shots[i + 1];
    }

    bq->size--;
}

static bool IsBoltRendered(SDL_Renderer *renderer, Entity *bolt)
{
    if (bolt->box.y > 0)
    {
        bolt->box.y -= bolt->vel;
        SDL_SetRenderDrawColor(renderer, bolt->color.r, bolt->color.g, bolt->color.b, bolt->color.a);
        SDL_RenderFillRect(renderer, &bolt->box);

        return true;
    }

    return false;
}

Entity InitEntityBolt(int w, int h, int vel, Uint32 firetime,
                Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Entity bolt;

    bolt.box.x = 0;
    bolt.box.y = 0;
    bolt.box.w = w;
    bolt.box.h = h;

    bolt.vel = vel;
    bolt.firetime = firetime;

    bolt.color.r = r;
    bolt.color.g = g;
    bolt.color.b = b;
    bolt.color.a = a;

    bolt.texture = NULL;

    return bolt;
}

Bolts InitBolts(int capacity)
{
    Bolts bolts;
    bolts.capacity = capacity;
    bolts.size = 0;
    bolts.next_shot = 0;
    bolts.shots = malloc(sizeof(Entity) * (size_t)capacity);

    return bolts;
}

void AppendBolts(Bolts *bolts, Entity *shot, SDL_Rect *ent_box)
{
    bolts->shots[bolts->size] = *shot;
    bolts->shots[bolts->size].box.x = ent_box->x + (ent_box->w / 2);
    bolts->shots[bolts->size].box.y = ent_box->y - bolts->shots[bolts->size].box.h;
    bolts->size++;
}

bool IsBoltsFull(Bolts *bolts)
{
    if (bolts->size == bolts->capacity)
    {
        return true;
    }

    return false;
}

void RenderBolts(Bolts *bolts, SDL_Renderer *renderer)
{
    int i = 0;

    while (i < bolts->size)
    {
        if (!(IsBoltRendered(renderer, &bolts->shots[i])))
        {
            RemoveBolt(bolts);
            continue;
        }

        i++;
    }
}

void FreeBolts(Bolts *bolts)
{
    free(bolts->shots);
}
