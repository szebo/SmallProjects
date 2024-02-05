#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL_ttf.h>
#include <stdbool.h>

#include "generator.h"

void gameCycle();

void getFieldCoordinatesFromMouse(int mouseX, int mouseY, int *x, int *y);

void menuHandling(TTF_Font *font, int *width, int *height, int *mines, int *time, bool *timedgame);

void clearBoardMemory(Field **board, int height);

Uint32 increseTimer(Uint32 ms, void *param);

void readOptions(int *width, int *height, int *mines, int *time, bool *timedgame);

void saveOptions(int width, int height, int mines, int time, bool timedgame);

#endif // INPUT_H_INCLUDED
