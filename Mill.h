#ifndef PROGRAM_MILL_H
#define PROGRAM_MILL_H
#pragma once

#include <iostream>
#include <mutex>
#include <unistd.h>
#include <ncurses.h>
#include <atomic>

using namespace std;

class Mill{

static const int MAX_AMOUNT_GRAINS_IN_TANK = 90;
static const int MAX_FLOUR_IN_WAREHOUSE = 100;
int amountOfRyeGrains=0;
int amountOfWheatGrains=0;
int amountOfRyeFlour=0, amountOfWheatFlour=0;
bool availableRyeTank = true, availableRyeFlourWarehouse = true;
bool availableWheatTank = true, availableWheatFlourWarehouse = true;

public:

    Mill();
    void takeGrainsFromFarmer(string typeField);
    void makeFlour();
    void refreshTanks();
    void refreshWarehouse();
    void produceRyeFlour();
    void produceWheatFlour();
    void runProcessLoading();
    void loadRyeFlourIntoWarehouse();
    void loadWheatFlourIntoWarehouse();
    int getAmountOfRyeFlour(){ return amountOfRyeFlour;}
    int getAmountOfWheatFlour(){ return amountOfWheatFlour;}
    void sellRyeFlour(int weight);
    void sellWheatFlour(int weight);
};


#endif //PROGRAM_MILL_H