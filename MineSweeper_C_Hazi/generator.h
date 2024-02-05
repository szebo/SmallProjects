#ifndef GENERATOR_H_INCLUDED
#define GENERATOR_H_INCLUDED

#include <stdbool.h>

typedef struct Field{
    int value;
    bool isMine;
    bool isRevealed;
    bool isFlagged;
}Field;

void generateGameSpace(Field ***board, int width, int height, int mines);

void incrementNorth(Field **field, int x, int y);
void incrementNorthEast(Field **field, int x, int y);
void incrementNorthWest(Field **field, int x, int y);
void incrementEast(Field **field, int x, int y);
void incrementWest(Field **field, int x, int y);
void incrementSouthWest(Field **field, int x, int y);
void incrementSouthEast(Field **field, int x, int y);
void incrementSouth(Field **field, int x, int y);

void processUpperLeft(Field **field, int x, int y);
void processUpperRight(Field **field, int x, int y);
void processUpperRow(Field **field, int x, int y);
void processRightColumn(Field **field, int x, int y);
void processLeftColumn(Field **field, int x, int y);
void processBottomRow(Field **field, int x, int y);
void processBottomLeft(Field **field, int x, int y);
void processBottomRight(Field **field, int x, int y);
void processMiddle(Field **field, int x, int y);

#endif // GENERATOR_H_INCLUDED
