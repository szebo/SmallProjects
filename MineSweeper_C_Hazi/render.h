#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>

#include "generator.h"

typedef enum Color{
    red = 0,
    green = 1,
    blue = 2,
    yellow = 3,
    brown = 4,
    purple = 5,
    cyan = 6,
    orange = 7,
    grey = 8,
    light_grey = 9
}Color;

SDL_Color setColor(Color c);

void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer);

void renderFlag(SDL_Renderer *renderer, int x, int y);

void renderText(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font, Color color, char *text);

void renderBoard(SDL_Renderer *renderer, int width, int height);

void renderEmptySpace(SDL_Renderer *renderer, Field **board, int x, int y, int width, int height, TTF_Font *font, int *revealed);

void revealSquare(SDL_Renderer *renderer, Field **board, TTF_Font *font, int i, int j);

void revealFullMap(SDL_Renderer *renderer, Field **board, TTF_Font *font, int width, int height);

void renderTopBarButtons(SDL_Renderer *renderer, TTF_Font *font);

void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int *width, int *height, int *mines, int *time, bool *timedgame);

void convertIntToString(int a, char **target);

void renderTimer(SDL_Renderer *renderer, TTF_Font *font, int time);

void renderImage(SDL_Renderer *renderer, char *img_name, SDL_Rect *pos);

void renderMineCounter(SDL_Renderer *renderer, int mines, TTF_Font *font);

#endif // RENDER_H_INCLUDED
