#ifndef PROGRAM_MILL_H
#define PROGRAM_MILL_H

#include <iostream>
#include <mutex>
#include <unistd.h>
#include <ncurses.h>
#include <condition_variable>
#include <atomic>

using namespace std;

class Mill{

static const int MAX_AMOUNT_GRAINS_IN_TANK = 90;
static const int MAX_FLOUR_IN_WAREHOUSE = 100;
int amountOfRyeGrains=0;
int amountOfWheatGrains=0;
int amountOfRyeFlour=10, amountOfWheatFlour=10;
bool availableRyeTank = true, availableRyeFlourWarehouse = true;
bool availableWheatTank = true, availableWheatFlourWarehouse = true;
bool availableMillWarehouses = true, availableAmountWheatFlour = true, availableAmountRyeFlour  = true;

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
    bool getAvailableMillWarehouses(){return availableMillWarehouses;}
    void setAvailableMillWarehouses(bool availableWarehouses){this->availableMillWarehouses = availableWarehouses;}
    bool getAvailableRyeTank();
    bool getAvailableWheatTank();
    bool getAvailableWheatWarehouse(){return availableWheatFlourWarehouse;}
    bool getAvailableRyeWarehouses(){return availableRyeFlourWarehouse;}
    bool getAvailableAmountWheatFlour(){return this->availableAmountWheatFlour;}
    bool getAvailableAmountRyeFlour(){return this->availableAmountRyeFlour;}
    void setStatusAmountOfRyeFlour(bool status){ this->availableAmountRyeFlour = status;}
    void setStatusAmountOfWheatFlour(bool status){this->availableAmountWheatFlour = status;}
};


#endif //PROGRAM_MILL_H
