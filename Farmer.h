
#ifndef PROGRAM_FARMER_H
#define PROGRAM_FARMER_H
#pragma once

#include <thread>
#include <mutex>
#include "Mill.h"
#include "Road.h"
#include <iostream>
#include <string>
#include <ncurses.h>
#include <unistd.h>


using namespace std;

class Farmer {

private:
    static const int MAX_LOAD_TRAILER = 5;
    static Road roadHomeToFieldWithRye, roadHomeToFieldWithWheat, roadFieldWithWheatToMill, roadFieldWithRyeToMill, roadFromMillToHome;
    int x, y, x_start, y_start, ID, loadTrailer = 0;
    string state = "rest";

public:
    Farmer();
    Farmer(int x, int y, int ID);

    int getX();
    int getY();
    int getID();
    string getState();

    void setState(string state);
    void setX(int x);
    void setY(int y);
    void setID(int ID);

    void setPosition(int y, int x);
    void clearPosition(int y, int x);
    void goFieldWithWheat();
    void goFieldWithRye();
    void collectCrops();
    void sellGrain();
    void goToHome();
    void Resting();
    void simulatingLife();
    void closeThread();
};

#endif //PROGRAM_FARMER_H