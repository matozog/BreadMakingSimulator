#include "Farmer.h"
#include "Bakery.h"
#include "Shop.h"
#include <fstream>
#include <atomic>
#include <unistd.h>
#include <iostream>
#include <mutex>

using namespace std;


mutex mutexConsole;
mutex mutexFarmers;

bool loadMapFromFile();
void drawMap();

Field fieldWithWheat(0);
Field fieldWithRye(1);
Mill mill;
Bakery bakery;
Shop shop;

static string mapFileName = "map";
int **mapArray;
CoordinateField** mapFields;
int widthMap = 0, heightMap = 0;
bool endProgram = false;
int amountOfFarmers = 9;  // max 9

int main(int argc, char **argv ) {

    srand(time(NULL));
    initscr();
    nodelay(stdscr,TRUE);
    start_color();
    curs_set(0);

    // initialize color
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    init_pair(2, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(3, COLOR_WHITE, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_WHITE, COLOR_BLUE);
    init_pair(7, COLOR_WHITE, COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_CYAN);  

    // declare threads and objects

    thread *farmerThreads = new thread[amountOfFarmers];
    vector<Farmer> farmers;
    thread natureFieldWithWheat, natureFieldWithRye, millThread, bakeryThread;
    thread shopThread;

    // load map from file

    if(loadMapFromFile())
        drawMap();
    else cout<< "Brak pliku";

    // run nature threads on the fields
    natureFieldWithRye = thread(&Field::natureThread, &fieldWithRye);
    natureFieldWithWheat = thread(&Field::natureThread, &fieldWithWheat);
    
    // run bakery thread
    bakeryThread = thread(&Bakery::simulatingBakeryLife, &bakery);

    // run mill thread
    millThread = thread(&Mill::makeFlour, &mill);

    //run shop thread
    shopThread = thread(&Shop::simulatingShopLife, &shop);

    // create farmers
    int x_farmer = 4, y_farmer = 31;
    for(int i=0, j=0; i<amountOfFarmers; i++, j+=3)
    {
        Farmer farmer(x_farmer+j,y_farmer,i);
        farmers.push_back(farmer);
        if(i>0 && i%3==2) {
            y_farmer++;
            x_farmer = 4;
            j=-3;
        }
    }

    // run farmers threads
    for(int i=0 ;i<farmers.size(); i++){
        farmerThreads[i] = thread(&Farmer::simulatingLife, farmers.at(i));
    }

    char c = getch();

    while(c!='c'){
        c = getch();
    }

    // join farmers threads
    for(int i=0; i<amountOfFarmers;i++)
    {
        farmerThreads[i].join();
    }
    endwin();

    return 0;
}

// function drawing map from file, read integer and by switch case declaration draw right character

void drawMap()
{
    for(int i = 0; i< heightMap ; i++)
    {
        for(int j=0; j< widthMap; j++) {
           switch(mapArray[i][j])
           {
               case 0:
                   attron(COLOR_PAIR(1));
                   mutexConsole.lock();
                   mvprintw(i,j,"%s"," ");
                   mutexConsole.unlock();
                   attroff(COLOR_PAIR(1));
                   break;
               case 1:
                   move(i,j);
                   addch(ACS_HLINE);
                   break;
               case 2:
                   move(i,j);
                   addch(ACS_VLINE);
                   break;
               case 3:
                   mutexConsole.lock();
                   mvprintw(i,j,"%s"," ");
                   mutexConsole.unlock();
                   break;
               case 4:
                   fieldWithRye.setArea(j,i,4);
                   break;
               case 5:
                   fieldWithWheat.setArea(j,i,4);
                   break;
                case 6:
                    move(i,j);
                    addch(ACS_ULCORNER);
                   break;
                case 7:
                    break;
                case 8: 
                    move(i,j);
                    addch(ACS_URCORNER);
                    break;
                case 9:
                    move(i,j);
                    addch(ACS_LLCORNER);
                    break;
                case 10:
                    move(i,j);
                    addch(ACS_LRCORNER);
                    break;
                case 11:
                    mutexConsole.lock();
                    attron(COLOR_PAIR(8));
                    mvprintw(i,j,"%s"," ");
                    attroff(COLOR_PAIR(9));
                    mutexConsole.unlock();
                    break;
                case 12:
                    mutexConsole.lock();
                    attron(COLOR_PAIR(2));
                    mvprintw(i,j,"%s"," ");
                    attroff(COLOR_PAIR(11));
                    mutexConsole.unlock();
                    break;
           }
            mutexConsole.lock();
            refresh();
            mutexConsole.unlock();
        }
    }
}

// function opening and read data from file to mapArray, return true if operations completed successful

bool loadMapFromFile()
{
    fstream file;
    file.open(mapFileName);

    if( !file.good() )
        return false;

    file >> heightMap >> widthMap;
    mapArray = new int* [heightMap];
    mapFields = new CoordinateField* [heightMap];

    for(int i = 0; i< heightMap ; i++)
    {
        mapArray[i] = new int[widthMap];
        mapFields[i] = new CoordinateField[widthMap];
        for(int j=0; j< widthMap; j++) {
            file >> mapArray[i][j];
            mapFields[i][j] = {i,j,0,mapArray[i][j],true};
        }
    }
    return true;
}

