#include "input.h"
#include "render.h"
#include "debugmalloc.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

void gameCycle(){
    /**Ez a játékablak, és a hozzá tartozó renderer*/
    SDL_Window *window;
    SDL_Renderer *renderer;

    /**A játék tér beállításai*/
    int width, height, mines, time, currentTime;
    bool timedgame;

    /**A legutóbbi beállítások betöltése*/
    readOptions(&width, &height, &mines, &time, &timedgame);

    /**A pálya*/
    Field **board;
    /**A pálya generálása, részletek a generator.c fileban*/
    generateGameSpace(&board, width, height, mines);

    /**SDL könyvtár inicializálása, részletek a render.c fileban*/
    sdl_init("MineSweeper", width*49+12, height*49+106, &window, &renderer);

    /**TTF inicializálása és az Arial font megnyitása*/
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 50);

    /**E a ciklus debug célokból kiírja consolera a pályát, a végleges programban nem lesz benne*/
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%2d ", board[i][j].value);
        }
        printf("\n");
    }
    /**Timer indítása a játék indításakor*/
    SDL_TimerID id = SDL_AddTimer(1000, increseTimer, NULL);

    /**Néhány változó a játék vezérléséhez*/
    SDL_Event ev;
    int quit = 0;
    int mouseX = 0;
    int mouseY = 0;
    int revealed = 0;
    int mineCounter = 0;

    /**Kirajzolja a hátteret*/
    boxRGBA(renderer, 0, 0, width*50, height*50+100, 0, 0, 0, 255);
    /**Kirajzolja a mezők négyzethálóját. Részletek a render.c fileban*/
    renderBoard(renderer, width, height);
    /**Kirajzolja a pálya fölötti részt*/
    renderTopBarButtons(renderer, font);
    /**Az időzítő kirajzolás beállłtása beállítása*/
    if(timedgame) {
        renderTimer(renderer, font, time);
        currentTime = time;
    }
    else {
        renderTimer(renderer, font, 0);
        currentTime = 0;
    }

    /**Ez a játék fõciklusa, várja az inputot, és amikor megkapja, akkor végrehajtja a hozzárendelt utasítást, amik lejebb vannak részletezve*/
    while(quit == 0){
        /**Az akna számláló renderelése*/
        renderMineCounter(renderer, mines-mineCounter, font);
        /**A renderer által összerakott kép megjelenítése*/
        SDL_RenderPresent(renderer);
        /**A következő event-re várakozás*/
        SDL_WaitEvent(&ev);

        switch(ev.type){
            /**Az egér koodinátáit gyûjti be minden alkalommal amikor megmozdítjuk*/
            case SDL_MOUSEMOTION:
                mouseX = ev.motion.x;
                mouseY = ev.motion.y;
                break;

            /**Egérkattintásra két dolog történhet*/
            case SDL_MOUSEBUTTONDOWN:
                /**Bal egérgombbal felfedjük a mezõt ami fölött van a kurzor, új játékot indítunk vagy belépünk a menübe*/
                if(ev.button.button == SDL_BUTTON_LEFT){
                    /**Azért nézem, hogy 100-nál nagyobb-e a koordináta, mert ha nem, akkor nem a pálya fölött van az egér, vagyis nem kell mezõkkel foglalkozni
                    *Ez majd akkor lesz lényeges amikor a Menü és az Új Játék gombokat is elhelyezem a pálya fölötti, jelenleg üres helyre*/
                    if(mouseY>100){
                        int x, y;
                        getFieldCoordinatesFromMouse(mouseX, mouseY-100, &x, &y);
                        /**Abban az esetben ha akna, akkor vesztettünk*/
                        if(board[y][x].isMine){
                            printf("You lost.\n");
                            revealFullMap(renderer, board, font, width, height);
                            SDL_RemoveTimer(id);
                        }
                        /**Ha a mező értéke 0, akkor felfedi az érték néküli mezőket és az azt határoló értéket. Akna biztos nincs ennek a teületnek a szélén, mivel az aknákat nem 0 számok veszik körbe*/
                        else if(board[y][x].value == 0){
                            renderEmptySpace(renderer, board, x, y, width, height, font, &revealed);
                        }
                        /**Egyébként kiírja a mezõ értékét*/
                        else{
                            revealSquare(renderer, board, font, y, x);
                            revealed += 1;
                            /**Ha ez volt az utolsó mező amit még fel kellett fedni és nem akna, akkor nyert a játékos*/
                            if((width*height)-revealed == mines){
                                /**Felfedi az egész pályát*/
                                revealFullMap(renderer, board, font, width, height);
                                printf("You won!\n");
                                /**Leállítja az időzítőt*/
                                SDL_RemoveTimer(id);
                            }
                            /**Ha zászlós, akkor levesszük róla a zászlót, és csökkentjük a minesCounter-t*/
                            if(board[y][x].isFlagged){
                                board[y][x].isFlagged = false;
                                mineCounter -= 1;
                            }
                        }
                    /**Ha nem a pályán van az egér, akkor biztos fölötte*/
                    }else{
                        /**Ez az új játék gomb vezérlése*/
                        if(mouseX >= 114 && mouseX <= 264 && mouseY >= 6 && mouseY <= 94){
                            /**Törli a pályát*/
                            clearBoardMemory(board, height);
                            /**Újra generálja*/
                            generateGameSpace(&board, width, height, mines);
                            /**Kirajzolja az alapállpotot*/
                            renderBoard(renderer, width, height);

                            /**A beállítások szerint időzített-e a játék, ha igen, akkor az időzítőt a megadott értékre állítja, különben 0-ra*/
                            if(timedgame) currentTime = time;
                            else currentTime = 0;

                            /**A vezérlési véltozók alap állapotba állítása*/
                            revealed = 0;
                            mineCounter = 0;
                            /**Időzítő kirajzolása*/
                            renderTimer(renderer, font, currentTime);
                            /**Időzítő indítása*/
                            id = SDL_AddTimer(1000, increseTimer, NULL);
                        }
                        /**A menű gomb vezérlése*/
                        if(mouseX >= 6 && mouseX <= 106 && mouseY >= 6 && mouseY <= 94){
                            /**Be kell zárni a játék ablakot*/
                            SDL_DestroyWindow(window);
                            /**Törölni kell a pályát*/
                            clearBoardMemory(board, height);
                            /**Le kell állítani az iőzítőt*/
                            SDL_RemoveTimer(id);
                            /**Átvált a menü kezelésre*/
                            menuHandling(font, &width, &height, &mines, &time, &timedgame);

                            /**A játék ablak újboli megnyitása*/
                            sdl_init("MineSweeper", width*49+12, height*49+106, &window, &renderer);

                            /**Ez innentől ugyanaz mint a "New Game" gomb esetében, azzal a bővítéssel, hogy itt a menüsort is újra ki kell rajzolni felülre*/
                            if(timedgame) currentTime = time;
                            else currentTime = 0;

                            revealed = 0;
                            mineCounter = 0;
                            generateGameSpace(&board, width, height, mines);
                            renderBoard(renderer, width, height);
                            renderTopBarButtons(renderer, font);
                            renderTimer(renderer, font, currentTime);
                            id = SDL_AddTimer(1000, increseTimer, NULL);
                        }
                    }
                }
                /**Jobb egérgombbal zászlót helyezünk le*/
                if(ev.button.button == SDL_BUTTON_RIGHT){
                    /**Egérkoordinátákból meg kell keresni hogy melyik mezőn van*/
                    int x, y;
                    getFieldCoordinatesFromMouse(mouseX, mouseY-100, &x, &y);
                    /**Ha még nincs felfedve és már van rajta zászló, akkor azt törli róla, ha még nincs rajta zászló, akkor kirajzol egyet, és megjelöli a zászlósnak a mezőt*/
                    if(!board[y][x].isRevealed){
                        if(board[y][x].isFlagged){
                            boxRGBA(renderer, x*49+6, y*49+106, x*49+6+45, y*49+106+45, 100, 100, 100, 255);
                            board[y][x].isFlagged = false;
                            mineCounter -= 1;
                        }
                        else{
                            board[y][x].isFlagged = true;
                            renderFlag(renderer, x, y);
                            mineCounter += 1;
                        }
                    }
                }
                break;

            /**A ciklusból való kilépést és a játék leállását inditja el ez a rész.*/
            case SDL_QUIT:
                quit = 1;
                break;
            /**Ez a case felel az időzítő müködéséért, másodpercenként ideugrik a program*/
            case SDL_USEREVENT:
                /**Akkor nyerünk, ha a felfedett mezők számát kivonva a pálya méretéből megkapjuk az aknák számát, vagyis ha az egész pályát ismeri más a játékos*/
                if((width*height)-revealed == mines){
                    revealFullMap(renderer, board, font, width, height);
                    printf("You won!");
                    SDL_RemoveTimer(id);
                }
                /**Ha lejár az idő és még nem nyertünk, akkor vesztettünk*/
                if(currentTime == 1 && timedgame){
                    SDL_RemoveTimer(id);
                    revealFullMap(renderer, board, font, width, height);
                    printf("You lost!");
                }
                /**Ez az else if felel az időzítőért, ha úgy állítottuk be a menüben, akkor visszaszámol, amúgy meg előre*/
                if(timedgame){
                    currentTime -= 1;
                    renderTimer(renderer, font, currentTime);
                }
                else{
                    currentTime += 1;
                    renderTimer(renderer, font, currentTime);
                }
                break;

            default:
                break;
        }
    }

    /**A játék bezárásakor fel kell szabadítani a memóriát, és megszüntetni a timer-t*/
    clearBoardMemory(board, height);
    SDL_RemoveTimer(id);
    /**A program bezárása és a foglalt memória területek felszabadítása*/
    TTF_CloseFont(font);
    /**Végül a program elmenti a legutóbbi beállításokat a menüből*/
    saveOptions(width, height, mines, time, timedgame);
}
/**Ez a fügvény felel a menü kezeléséért*/
void menuHandling(TTF_Font *font, int *width, int *height, int *mines, int *time, bool *timedgame){
    /**Létrehoz egy új renderert és ablakot*/
    SDL_Renderer *menuRenderer;
    SDL_Window *menuWindow;
    sdl_init("Menu", 500, 400, &menuWindow, &menuRenderer);

    /**Vezérlési változók*/
    bool gamestarted = false;
    int mouseX = 0, mouseY = 0;
    SDL_Event ev;

    /**Menü kirajzolása*/
    renderMenu(menuRenderer, font, width, height, mines, time, timedgame);
    SDL_RenderPresent(menuRenderer);

    /**Ez a részlet határozza meg, hogy mi lehet a maximum pálya méret az adott monitor felbontása alapján.*/
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    int max_Width = dm.w/50;
    int max_Height = (dm.h-200)/50;

    /**While ciklusban figyeli, hogy új input event történt-e, egész addig, amíg el nem indítják a játékot*/
    while(!gamestarted){
        /**A következő eventre várakozás*/
        SDL_WaitEvent(&ev);

        switch(ev.type){
            /**Mozgás esetén az egér koordináták lekérdezése*/
            case SDL_MOUSEMOTION:
                mouseX = ev.motion.x;
                mouseY = ev.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                /**Bal egér gomb esetén a megfelelő gomb megnyomásának érzékelése*/
                if(ev.button.button == SDL_BUTTON_LEFT){
                    /**A pálya szélességének növelése és csökkentése, 0 és max_Width között*/
                    if(mouseX > 50 && mouseX < 100 && mouseY > 35 && mouseY < 75){
                        if(*width <= max_Width) *width += 1;
                    }
                    if(mouseX > 50 && mouseX < 100 && mouseY > 129 && mouseY < 169){
                        if(*width-1 >= 10) *width -= 1;
                    }
                    /**A pálya magasságának növelése és csökkentése, 0 és max_Height között*/
                    if(mouseX > 200 && mouseX < 250 && mouseY > 35 && mouseY < 75){
                        if(*height <= max_Height) *height += 1;
                    }
                    if(mouseX > 200 && mouseX < 250 && mouseY > 129 && mouseY < 169){
                        if(*height-1 >= 10) *height -= 1;
                    }
                    /**Az aknák számának növelése és csökkentése*/
                    if(mouseX > 350 && mouseX < 400 && mouseY > 35 && mouseY < 75){
                        /**Az aknák számának maximuma kisebb kell legyen mint a pálya mérete-2*/
                        if(*mines+2 > (*width)*(*height)){
                            printf("You can't put more mines\n");
                        }
                        /**Az aknák számát nem ajánlott a pálya méreténél nagyobbra állítani*/
                        else if(*mines > (*width)*(*height)/2){
                            *mines += 1;
                            printf("Thats a lot of mines, you should consider not continuing to increase it.\n");
                        }
                        else{
                            *mines += 1;
                        }
                    }
                    /**Az aknák számát nem lehet 0 alá vinni*/
                    if(mouseX > 350 && mouseX < 400 && mouseY > 129 && mouseY < 169){
                        if(*mines > 0) *mines -= 1;
                    }
                    /**Az időzítő növelése és csökkentése*/
                    if(mouseX > 50 && mouseX < 100 && mouseY > 235 && mouseY < 275){
                        *time += 5;
                    }
                    /**Az időzítő nem lehet kisebb 0-nál*/
                    if(mouseX > 50 && mouseX < 100 && mouseY > 329 && mouseY < 369){
                        if(*time >= 0) *time -= 5;
                    }
                    /**Az időzítő ki-be kapcsolása*/
                    if(mouseX > 175 && mouseX < 225 && mouseY > 227 && mouseY < 277){
                        *timedgame = !(*timedgame);
                    }
                    /**A játék indítása*/
                    if(mouseX > 175 && mouseX < 275 && mouseY > 300 && mouseY < 350){
                        gamestarted = true;
                    }
                }
                /**Újra rajzolja a menüt az új értékekkel*/
                renderMenu(menuRenderer, font, width, height, mines, time, timedgame);
                SDL_RenderPresent(menuRenderer);
                break;

            /**Kilép a menüből és elindítja a játékot, akkor is ha bezárja az ablakot a felhasználó*/
            case SDL_QUIT:
                gamestarted = true;
                break;
            default:
                break;
        }
    }
    /**Törli a menü ablakot*/
    SDL_DestroyWindow(menuWindow);
}

/**Ez a függvény olvassa be a játák indításakor a legutóbb mentett beállításokat, és azokat a címszerint átvett paraméterekbe helyezi*/
void readOptions(int *width, int *height, int *mines, int *time, bool *timedgame){
    /**Megnyit egy file-t*/
    FILE *file = fopen("options.txt", "rt");
    /**Ellenőrzi, hogy megnyílt-e*/
    if(file == NULL){
        perror("Could not open file.");
        return;
    }

    /**Beolvassa az adatokat a megfelelő változókba*/
    fscanf(file, "%d\n%d\n%d\n%d", width, height, mines, time);
    //fscanf(file, "%d", height);
    //fscanf(file, "%d", mines);
    //fscanf(file, "%d", time);
    int temp;
    fscanf(file, "%d", &temp);
    if(temp == 0)
        *timedgame = false;
    else
        *timedgame = true;
    /**Bezárja a file-t*/
    fclose(file);
}

/**Ez a függvény menti el a program bezárásakor a beállításokat*/
void saveOptions(int width, int height, int mines, int time, bool timedgame){
    /**Megnyitja a file-t*/
    FILE *file = fopen("options.txt", "wt");
    /**Ellenőrzi, hogy megnyílt-e*/
    if(file == NULL){
        perror("Could not open file.");
        return;
    }

    /**Kiírja az paraméterként kapott változókat a fileba*/
    fprintf(file, "%d\n%d\n%d\n%d\n%d", width, height, mines, time, timedgame);
    /**Bezátja a file-t*/
    fclose(file);
}

/**Ez a függvény felel az időzítő folytonos működéséert, az infoc-s minta alapján készült*/
Uint32 increseTimer(Uint32 ms, void *param){
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    SDL_PushEvent(&ev);
    return ms;
}

/**Ez a függvény törli a pályát*/
void clearBoardMemory(Field **board, int height){
    for(int i = 0; i < height; i++){
        free(board[i]);
    }
    free(board);
}

/**Rövid függvény arra, hogy az egérkoordinátáiból megkapjam, hogy épp melyik mezőn van a kurzor*/
void getFieldCoordinatesFromMouse(int mouseX, int mouseY, int *x, int *y){
    *x = floor(mouseX/49);
    *y = floor(mouseY/50);
}
