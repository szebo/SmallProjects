#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "generator.h"
#include "debugmalloc.h"

/**Ez a függvény előre generálja a pályát minden alkalommal, amikor új játkot indítanak el*/
void generateGameSpace(Field ***target, int width, int height, int mines){

    /**Lefoglalja a pályának a szükséges memória területet*/
    Field **board = (Field**)malloc(sizeof(Field*)*height);
    for(int i = 0; i < height; i++){
        board[i] = (Field*)malloc(sizeof(Field)*width);
    }

    /**Random generátor inicializálása*/
    srand(time(NULL));
    /**Lerakott aknák számlálója*/
    int minesplaced = 0;

    /**A pálya elõkészítése a generáláshoz*/
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            board[i][j].value = 0;
            board[i][j].isMine = false;
            board[i][j].isRevealed = false;
            board[i][j].isFlagged = false;
        }
    }
    /**Az aknák elhelyezése random pozíciókba, még valamiért nem minden esetben mûködik. Addig fut a ciklus amíg el nem éri a meghatározott mennyiséget.*/
    while(minesplaced < mines){
        int i = (rand())%height;
        int j = (rand())%width;

        /**Ha a random generált koordinátákon még nincs akna, akkor lerak egyet és növeli a számlálót*/
        if(!board[i][j].isMine){
            board[i][j].isMine = true;
            board[i][j].value = ((int)'*');
            minesplaced++;
        }
    }

    /**Ez a ciklus az egész pályán megkeresi az aknákat és növeli a körülöttük levõ nem akna mezõk értékét*/
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(board[i][j].isMine){
                if(i == 0 && j == 0)
                    processUpperLeft(board, i, j);
                else if(i == 0 && j == width-1)
                    processUpperRight(board, i, j);
                else if(i == height-1 && j == 0)
                    processBottomLeft(board, i, j);
                else if(i == height-1 && j == width-1)
                    processBottomRight(board, i, j);
                else if(i == 0)
                    processUpperRow(board, i, j);
                else if(i == height-1)
                    processBottomRow(board, i, j);
                else if(j == 0)
                    processLeftColumn(board, i, j);
                else if(j == width-1)
                    processRightColumn(board, i, j);
                else
                    processMiddle(board, i, j);
            }
        }
    }
    *target = board;
}

/**A lenti függvényeknek mind a nevükben van, hogy mit csinálnak, az x a ciklusok i-je az y a ciklusok j-je.*/
void incrementNorth(Field **field, int x, int y){

    if(field[x-1][y].isMine)
        return;
    else
        field[x-1][y].value += 1;
}

void incrementNorthEast(Field **field, int x, int y){

    if(field[x-1][y+1].isMine)
        return;
    else
        field[x-1][y+1].value += 1;
}

void incrementNorthWest(Field **field, int x, int y){

    if(field[x-1][y-1].isMine)
        return;
    else
        field[x-1][y-1].value += 1;
}

void incrementEast(Field **field, int x, int y){

    if(field[x][y+1].isMine)
        return;
    else
        field[x][y+1].value += 1;
}

void incrementWest(Field **field, int x, int y){

    if(field[x][y-1].isMine)
        return;
    else
        field[x][y-1].value += 1;
}

void incrementSouthEast(Field **field, int x, int y){

    if(field[x+1][y+1].isMine)
        return;
    else
        field[x+1][y+1].value += 1;
}

void incrementSouthWest(Field **field, int x, int y){

    if(field[x+1][y-1].isMine)
        return;
    else
        field[x+1][y-1].value += 1;
}

void incrementSouth(Field **field, int x, int y){

    if(field[x+1][y].isMine)
        return;
    else
        field[x+1][y].value += 1;
}

void processUpperLeft(Field **field, int x, int y){
    incrementSouth(field, x, y);
    incrementSouthEast(field, x, y);
    incrementEast(field, x, y);
}

void processUpperRight(Field **field, int x, int y){
    incrementWest(field, x, y);
    incrementSouthWest(field, x, y);
    incrementSouth(field, x, y);
}

void processUpperRow(Field **field, int x, int y){
    incrementSouthWest(field, x, y);
    incrementSouth(field, x, y);
    incrementSouthEast(field, x, y);
    incrementEast(field, x, y);
    incrementWest(field, x, y);
}

void processRightColumn(Field **field, int x, int y){
    incrementNorthWest(field, x, y);
    incrementWest(field, x, y);
    incrementSouthWest(field, x, y);
    incrementNorth(field, x, y);
    incrementSouth(field, x, y);
}

void processLeftColumn(Field **field, int x, int y){
    incrementNorth(field, x, y);
    incrementSouth(field, x, y);
    incrementNorthEast(field, x, y);
    incrementEast(field, x, y);
    incrementSouthEast(field, x, y);
}

void processBottomLeft(Field **field, int x, int y){
    incrementNorth(field, x, y);
    incrementNorthEast(field, x, y);
    incrementEast(field, x, y);
}

void processBottomRight(Field **field, int x, int y){
    incrementNorth(field, x, y);
    incrementNorthWest(field, x, y);
    incrementWest(field, x, y);
}

void processBottomRow(Field **field, int x, int y){
    incrementEast(field, x, y);
    incrementNorthEast(field, x, y);
    incrementNorth(field, x, y);
    incrementNorthWest(field, x, y);
    incrementWest(field, x, y);
}

void processMiddle(Field **field, int x, int y){
    incrementEast(field, x, y);
    incrementNorth(field, x, y);
    incrementNorthEast(field, x, y);
    incrementNorthWest(field, x, y);
    incrementSouth(field, x, y);
    incrementSouthEast(field, x, y);
    incrementSouthWest(field, x, y);
    incrementWest(field, x, y);
}
