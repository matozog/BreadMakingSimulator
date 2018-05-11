#include <iostream>
#include <thread>
#include <unistd.h>
#include <ncurses.h>
#include <mutex>

using namespace std;

void loadMapFromFile(int **mapArray);

static string mapFileName = "map";

int main() {

    initscr();
    nodelay(stdscr,TRUE);
    start_color();

    int **mapArray;
    loadMapFromFile(mapArray);


    endwin();

    return 0;
}

void loadMapFromFile(int **mapArray)
{

}