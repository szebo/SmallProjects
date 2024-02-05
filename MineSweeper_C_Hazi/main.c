#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "render.h"
#include "input.h"
#include "generator.h"
#include "debugmalloc.h"

int main(int argc, char *argv[]) {

    /**Elinditja a főciklust, részletek az input.c fileban*/
    gameCycle();

    /**Az SDL bezárása*/
    SDL_Quit();
    return 0;
}
