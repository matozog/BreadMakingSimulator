#include "Farmer.h"
#include <fstream>
#include <atomic>

using namespace std;


mutex mutexConsole;
mutex mutexFarmers;

bool loadMapFromFile();
void drawMap();

Field fieldWithWheat(0);
Field fieldWithRye(1);

static string mapFileName = "map";
int **mapArray;
atomic<CoordinateField**> mapFields;
int widthMap = 0, heightMap = 0;
bool endProgram = false;
int amountOfFarmers = 9;  // max 9

int main(int argc, char **argv ) {

    srand(time(NULL));
    initscr();
    nodelay(stdscr,TRUE);
    start_color();
    curs_set(0);

    // declare threads and objects

    thread *farmerThreads = new thread[amountOfFarmers];
    vector<Farmer> farmers;
    thread natureFieldWithWheat, natureFieldWithRye;

    // load map from file

    if(loadMapFromFile())
        drawMap();
    else cout<< "Brak pliku";

    // run nature threads on the fields
    natureFieldWithRye = thread(&Field::natureThread, fieldWithRye);
    natureFieldWithWheat = thread(&Field::natureThread, fieldWithWheat);

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
                   init_pair(1, COLOR_RED, COLOR_GREEN);
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
                   fieldWithRye.setArea(j,i);
                   break;
               case 5:
                   fieldWithWheat.setArea(j,i);
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

