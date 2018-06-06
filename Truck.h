#ifndef PROGRAM_TRUCK_H
#define PROGRAM_TRUCK_H

#include "Road.h"
#include <iostream>
#include <mutex>
#include "Mill.h"

using namespace std;

class Truck{

protected:
    static const int MAX_LOAD_TRUCK = 10;
    static Road roadFromBakeryToMillGate, roadFromMillToBakery, roadFromShopToMillGate, roadFromMillToShop;
    static Road roadFromShopToBakery, roadFromBakeryToShop, roadBakeryFromGateToMill, roadShopFromGateToMill;
    string ID;
    int y,x, x_start, y_start;

public:
    Truck(){};
    Truck(int y, int x, string ID);
    void simulatingLife();
    bool checkAmountOfRyeFlour();
    bool checkAmountOfWheatFlour();
    void setPosition(int y, int x);
    void takeFlourFromMill(string type, int weight);
    int getXStart(){return x_start;}
    int getYStart(){return y_start;}
    int getX(){return x;}
    int getY(){return y;}
    void takeRyeFlour(int weight);
    void takeWheatFlour(int weight);

};

#endif
