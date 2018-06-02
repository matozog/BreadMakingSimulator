#ifndef PROGRAM_TRUCK_H
#define PROGRAM_TRUCK_H

#include "Road.h"
#include <iostream>
#include <mutex>
#include "Mill.h"

using namespace std;

class Truck{

static const int MAX_LOAD_TRUCK = 10;
static Road roadFromBakeryToMill, roadFromMillToBakery, roadFromShopToMill, roadFromMillToShop;
static Road roadFromShopToBakery, roadFromBakeryToShop;
string ID;
int y,x, x_start, y_start;

public:
    Truck(){};
    Truck(int y, int x, string ID);
    void simulatingLife();
    bool checkAmountOfRyeFlour();
    bool checkAmountOfWheatFlour();
    void setPosition(int y, int x);
    void takeFlourFromMill(string type);
    int getXStart(){return x_start;}
    int getYStart(){return y_start;}

};

#endif