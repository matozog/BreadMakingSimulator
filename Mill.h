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
int amountOfRyeGrains;
int amountOfWheatGrains;
bool availableRyeTank = true;
bool availableWheatTank = true;

public:

    Mill();
    void takeGrainsFromFarmer(string typeField);
    void makeFlour();
    void refreshTanks();
    void produceRyeFlour();
    void produceWheatFlour();
    void runProcessLoading();
};


#endif //PROGRAM_MILL_H