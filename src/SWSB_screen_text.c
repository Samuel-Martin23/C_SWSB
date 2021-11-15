#include "../include/SWSB_screen_text.h"

static void itostr(int value, char *str)
{
    snprintf(str, MAX_BUFFER, "%d", value);
}

static void SetScreenTextTexture(SDL_Renderer *renderer, ScreenText *st)
{
    SDL_Surface *text_surface = TTF_RenderText_Solid(st->font, st->text_buffer, st->color);

    st->texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    st->rect.w = text_surface->w;
    st->rect.h = text_surface->h;

    SDL_FreeSurface(text_surface);
}

ScreenText InitScreenTextScore(SDL_Renderer *renderer, const char *text)
{
    if (TTF_Init() == -1)
    {
        printf("Could not initialize ttf.\n");
        exit(1);
    }

    ScreenText st;

    memcpy(st.text_buffer, text, MAX_BUFFER);

    st.font = TTF_OpenFont(GAME_FONT, SCORE_FONT_SIZE);
    st.color.r = 255;
    st.color.g = 255;
    st.color.b = 255;
    st.color.a = 255;

    SetScreenTextTexture(renderer, &st);

    st.rect.x = SCREEN_WIDTH - st.rect.w;
    st.rect.y = 0;

    st.text_num = 0;

    return st;
}

void SetScoreScreenText(ScreenText *st, int score, SDL_Renderer *renderer)
{
    int j = 0;
    int score_len = 0;
    char score_text[SCORE_MAX_BUFFER] = {0};

    st->text_num += score;
    itostr(st->text_num, score_text);
    score_len = (int)strlen(score_text);

    for (int i = (SCORE_MAX_SIZE - score_len); i < SCORE_MAX_SIZE; i++)
    {
        st->text_buffer[i] = score_text[j];
        j++;
    }

    SDL_DestroyTexture(st->texture);
    SetScreenTextTexture(renderer, st);
}

void RenderScreenText(ScreenText *st, SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, st->texture, NULL, &st->rect);
}

void FreeScreenText(ScreenText *st)
{
    SDL_DestroyTexture(st->texture);
}
