#include <SDL.h>
#include <stdlib.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>

#include "render.h"
#include "input.h"
#include "debugmalloc.h"

/**Ez a projetk létrehozásakor automatikusan generált függvény.*/
void sdl_init(char const *felirat, int szeles, int magas, SDL_Window **pwindow, SDL_Renderer **prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window *window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/**Zászló rajzoló függvény, beolvassa a flag.png-t és kirajzolja a paraméterként kapott helyre*/
void renderFlag(SDL_Renderer *renderer, int x, int y){
    SDL_Texture *flag = IMG_LoadTexture(renderer, "flag.png");
    if (flag == NULL) {
        SDL_Log("Can't open image: %s", IMG_GetError());
        exit(1);
    }

    /**Az x és y koordináták számolása a következõ képlettel történik: (Az x és y a mezõk koordinátája a pályából, ezért azt át kell konvertálnom az SDL koordináta rendszerébe)
    A konevrtálásnál a szorzó a mezõ méretét adja meg, a +6 pedig a játékablak szélén levõ vastagabb margó. AZ y-nál a +100 ahhoz kell, hogy letoljam a tényleges mezõre, mivel az mindig 100
    képponttal van lejebb a ablak tetejétõl.*/
    SDL_Rect destRect = {x*49+6, y*49+6+100, 45, 45};
    /**A zászlõ mögé egy világosabb mezõt rajzolók, hogy jobban látszódjon, hogy itt történt valami, bár ez fõleg a szöveg rajzolásnál lesz lényeges.
    A számok itt sorban: bal felsõ x és y (ugyanaz mint az elõbb), a jobb alsó x és y (A +50-es eltolás mezõk közötti elválasztõ vonal miatt van), és a szín RGBA formátumban*/
    boxRGBA(renderer, x*49+6, y*49+6+100, x*49+50, y*49+100+50, 130, 130, 130, 255);
    SDL_RenderCopy(renderer, flag, NULL, &destRect);
}

/**Ez a függvény rajzolja ki a szövegeket*/
void renderText(SDL_Renderer *renderer, SDL_Rect rect, TTF_Font *font, Color color, char *text){
    /**Létrehoz egy SDL_Surface pointert, amire ráírja a paraméterként kapott szöveget a betûtípussal és aszínnel ellátva*/
    SDL_Surface *text_surface = TTF_RenderUTF8_Shaded(font, text, setColor(color), setColor(9));
    /**Az SDL_Surface-bõl egy SDL_Texture-t csinál*/
    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    /**Az SDL_Texture-t felhasználva kirajzolja a szöveget*/
    SDL_RenderCopy(renderer, text_texture, NULL, &rect);
    /**Memória felszabadítása*/
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}

/**Ez a függvény rajzolja ki a pályát, még egy kicsit finomítani kell a jobb szél igazítását.
*A +6 itt is a margót adja, a mezõk 49 képpont szélesek*/
void renderBoard(SDL_Renderer *renderer, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            boxRGBA(renderer, 6+(j*49), 106+(i*49), 6+(j*49+45), 106+(i*49+45), 100, 100, 100, 255);
        }
    }
}
/**Ez a függvény rekurzívan felfedi az üres területet és a szélén lévő mezőket, ha 0 értékű mezőre kattintottunk. Még dolgozom rajta, jelenlegi állapotában szerintem nem működik még jól.*/
void renderEmptySpace(SDL_Renderer *renderer, Field **board, int x, int y, int width, int height, TTF_Font *font, int *revealed){

    /**Bázis feltételek a következők:
    *Ha pályán kívül vannak a koordináták*/
    if(x < 0 || y < 0 || y >= height || x >= width){
        return;
    }
    /**Ha már fel van fedve*/
    if(board[y][x].isRevealed){
        return;
    }
    /**Ha zászló van a mezőn*/
    if(board[y][x].isFlagged){
        return;
    }
    /**Ha nem 0 értékű mezőnél van*/
    if(board[y][x].value > 0){
        revealSquare(renderer, board, font, y, x);
        *revealed += 1;
        return;
    }

    /**Ha bázisfeltételek nem tértek vissza, akkor kirajzol egy üres mezőt a megfelelő koordinátákra, és beállítja a mezőt felfedettre.*/
    board[y][x].isRevealed = true;
    *revealed += 1;
    boxRGBA(renderer, x*49+6, y*49+6+100, x*49+50, y*49+100+50, 130, 130, 130, 255);
    SDL_RenderPresent(renderer);
    /**Tovább megy valamelyik szomszédos mezőre.*/
    renderEmptySpace(renderer, board, x-1, y, width, height, font, revealed);
    renderEmptySpace(renderer, board, x+1, y, width, height, font, revealed);
    renderEmptySpace(renderer, board, x, y-1, width, height, font, revealed);
    renderEmptySpace(renderer, board, x, y+1, width, height, font, revealed);
    renderEmptySpace(renderer, board, x-1, y+1, width, height, font, revealed);
    renderEmptySpace(renderer, board, x-1, y-1, width, height, font, revealed);
    renderEmptySpace(renderer, board, x+1, y+1, width, height, font, revealed);
    renderEmptySpace(renderer, board, x+1, y-1, width, height, font, revealed);
}
/**Ezzel a függvény végzi a nem 0 értékű mezők felfedését*/
void revealSquare(SDL_Renderer *renderer, Field **board, TTF_Font *font, int i, int j){
    /**Az SDL_Rect adja meg, hogy hova kell kirajzolni majd*/
    SDL_Rect rect = { j*49+6, i*49+6+100, 45, 45};
    /**A felfedettség vizsgálatra azért van szükség, hogy nem rendereljük egymásra többször ugyanazt értelmetlenül*/
    if(!board[i][j].isRevealed){
        board[i][j].isRevealed = true;
        /**A megkülönböztetésre, aknás és nem aknás mező között azért van szükség, mert különben nem a megfelelő karakterek jelennének meg, hiszen az egyiknél %d kell a másiknál pedig %c
        *Ha nincs akna a mezőn, akkor kiírja a számot*/
        if(!board[i][j].isMine){
            /**Itt pedig azért kell külön esetként kezelni a 0 értéket, mert ott nem kell kiírni a számot. Ez tulajdonképpen csak a revealFullMap() függvény hívásakor lehetséges
            *mivel, ha 0 értékre kattintunk, annak a főciklusban már van egy megkülönbözetett esete.*/
            if(board[i][j].value == 0){
                boxRGBA(renderer, j*49+6, i*49+6+100, j*49+50, i*49+100+50, 130, 130, 130, 255);
            }
            /**Ha nem 0 az értéke, akkor azt át kell alakítani stringé, hogy aztán ki lehessen írni, majd a stringet fel kell szabadítani*/
            else{
                char *text;
                convertIntToString(board[i][j].value, &text);
                renderText(renderer, rect, font, board[i][j].value, text);
                free(text);
            }
        }
        /**Ha van akna, akkor egy * karaktert ír ki*/
        else{
            char *text = (char*)malloc(sizeof(char)*2);
            sprintf(text, "%c", board[i][j].value);
            renderText(renderer, rect, font, board[i][j].value, text);
            free(text);
        }
    }
}
/**Ez a függvény akkor hívódik meg, amikor az illető egy aknára kattintott, ilyenkor az egész pályára lefut a revealSquare() függvény*/
void revealFullMap(SDL_Renderer *renderer, Field **board, TTF_Font *font, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(!board[i][j].isRevealed)
                revealSquare(renderer, board, font, i, j);
        }
    }
}
/**Ez a függvény jeleníti meg a játéktér fölötti gombokat és számlálókat (a számlálók még nincsenek meg)*/
void renderTopBarButtons(SDL_Renderer *renderer, TTF_Font *font){
    SDL_Rect menu = { 6, 6, 100, 88 };
    SDL_Rect newGame = { 110, 6, 150, 88 };

    renderText(renderer, menu, font, -1, "Menu");
    renderText(renderer, newGame, font, -1, "New Game");
}
/**Ez rajzolja fel az időzítőt, másodpercenként van meghívva a gamaCycle() függvényben*/
void renderTimer(SDL_Renderer *renderer, TTF_Font *font, int time){
    SDL_Rect timer = {266, 6, 100, 88};
    char *text;
    convertIntToString(time, &text);
    renderText(renderer, timer, font, -1, text);
    free(text);
}
/**Ez a függvény felelős a menü megjelenítésért, minden kattintás után meghívódik*/
void renderMenu(SDL_Renderer *renderer, TTF_Font *font, int *width, int *height, int *mines, int *time, bool *timedgame){
    /**Ez a háttér a menüben*/
    boxRGBA(renderer, 0, 0, 500, 500, 150, 150, 150, 255);
    /**Itt egymás után vannak a különböző menüelemek helyeit meghatározó négyszögek, üres sorokkal elválasztva a különböző beállítások szerint.*/
    SDL_Rect increase_width = {50, 35, 50, 40};
    SDL_Rect decrease_width = {50, 129, 50, 40};
    SDL_Rect width_option = {25, 77, 100, 50};
    SDL_Rect width_option_text = { 35, 6, 80, 25};

    SDL_Rect incraese_height = {200, 35, 50, 40};
    SDL_Rect decrease_height = {200, 129, 50, 40};
    SDL_Rect height_option = {175, 77, 100, 50};
    SDL_Rect height_option_text = { 185, 6, 80, 25};

    SDL_Rect incraese_mines = {350, 35, 50, 40};
    SDL_Rect decrease_mines = {350, 129, 50, 40};
    SDL_Rect mines_option = {325, 77, 100, 50};
    SDL_Rect mines_option_text = { 335, 6, 80, 25};

    SDL_Rect incraese_time = {50, 235, 50, 40};
    SDL_Rect decrease_time = {50, 329, 50, 40};
    SDL_Rect time_option = {25, 277, 100, 50};
    SDL_Rect time_option_text = { 35, 206, 80, 25};

    SDL_Rect timer_tick = {175, 227, 50, 50};
    SDL_Rect timer_tick_text_box = {227, 227, 200, 50};

    SDL_Rect new_game = {175, 300, 150, 50};

    /**Ezek a számból alakuló szövegek*/
    char *width_text;
    convertIntToString(*width, &width_text);
    char *height_text;
    convertIntToString(*height, &height_text);
    char *mines_text;
    convertIntToString(*mines, &mines_text);
    char *time_text;
    convertIntToString(*time, &time_text);

    /**Itt történik a tényleges rajzolás, ismét üres sorokkal tagolva a beállítások szerint*/
    renderImage(renderer, "uparrow.png", &increase_width);
    renderText(renderer, width_option, font, -1, width_text);
    renderImage(renderer, "downarrow.png", &decrease_width);
    renderText(renderer, width_option_text, font, -1, "Width");

    renderImage(renderer, "uparrow.png", &incraese_height);
    renderText(renderer, height_option, font, -1, height_text);
    renderImage(renderer, "downarrow.png", &decrease_height);
    renderText(renderer, height_option_text, font, -1, "Height");

    renderImage(renderer, "uparrow.png", &incraese_mines);
    renderText(renderer, mines_option, font, -1, mines_text);
    renderImage(renderer, "downarrow.png", &decrease_mines);
    renderText(renderer, mines_option_text, font, -1, "Mines");

    renderImage(renderer, "uparrow.png", &incraese_time);
    renderText(renderer, time_option, font, -1, time_text);
    renderImage(renderer, "downarrow.png", &decrease_time);
    renderText(renderer, time_option_text, font, -1, "Timer");

    renderText(renderer, timer_tick_text_box, font, -1, "Timed game");
    if(*timedgame){
        renderImage(renderer, "tick.png", &timer_tick);
    }
    else{
        renderImage(renderer, "emptytick.png", &timer_tick);
    }

    renderText(renderer, new_game, font, -1, "New Game");

    /**Itt pedig felszabadítom a dinamikusan foglalt memória területek.*/
    free(width_text);
    free(height_text);
    free(mines_text);
    free(time_text);
}

/**Az akna számláló megjelenítésére szolgáló külön függvény, ez is minden jobb egérgombnál meghívódik a játék során*/
void renderMineCounter(SDL_Renderer *renderer, int mines, TTF_Font *font){
    SDL_Rect pos = {400, 6, 100, 88};

    char *text;
    convertIntToString(mines, &text);

    renderText(renderer, pos, font, -1, text);
    free(text);
}

/**Int-ből stringet csinál, és azt a mutatott helyre másolja*/
void convertIntToString(int a, char **target){
    int length = a/10+2;
    *target = (char*)malloc(sizeof(char)*length);
    sprintf(*target, "%d", a);
}

/**Általános kép rajzoló függvény, azért vettem külön, mert a renderFlag()-ben kicsit máshpgy történik, hogy a háttér is megfeleően változzon*/
void renderImage(SDL_Renderer *renderer, char *img_name, SDL_Rect *pos){
    SDL_Texture *img = IMG_LoadTexture(renderer, img_name);
    if (img == NULL) {
        SDL_Log("Can't open image: %s", IMG_GetError());
        exit(1);
    }

    SDL_RenderCopy(renderer, img, NULL, pos);
}

/**Ezt azért csináltam, hogy a text renderernek elég legyen egy enumként megadni a színt ezzel szerintem áttekinthetőbb a kód.*/
SDL_Color setColor(Color c){
    SDL_Color color;
    switch(c){
    case 0:
        color = (SDL_Color){155, 86, 30}; ///Barna
        break;
    case 1:
        color = (SDL_Color){0, 255, 0}; ///Zöld
        break;
    case 2:
        color = (SDL_Color){255, 0, 0}; ///Piros
        break;
    case 3:
        color = (SDL_Color){235, 231, 28}; ///Sárga
        break;
    case 4:
        color = (SDL_Color){0, 0, 255}; ///Kék
        break;
    case 5:
        color = (SDL_Color){175, 70, 219}; ///Lila
        break;
    case 6:
        color = (SDL_Color){70, 192, 219}; ///Cián-kék
        break;
    case 7:
        color = (SDL_Color){222, 136, 31}; ///Narancs
        break;
    case 8:
        color = (SDL_Color){100, 100, 100}; ///Sötét szürke
        break;
    case 9:
        color = (SDL_Color){130, 130, 130}; ///Világos szürke
        break;
    default:
        color = (SDL_Color){0, 0, 0}; ///Fekete
        break;
    }
    return color;
}
