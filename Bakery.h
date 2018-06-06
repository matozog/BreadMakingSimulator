#ifndef PROGRAM_BAKERY_H
#define PROGRAM_BAKERY_H

#include<iostream>
#include<thread>
#include "Truck.h"

using namespace std;

class Bakery{

static const int MAX_AMOUNT_WHEAT_FLOUR = 54;
static const int MAX_AMOUNT_RYE_FLOUR = 54;
static const int MAX_AMOUNT_RYE_BREAD = 40;
static const int MAX_AMOUNT_RYE_WHEAT_BREAD = 40;
Truck deliveryTruck;
thread deliveryTruckThread;
bool neededRyeFlour = true, neededWheatRyeFlour = true;
bool availableRyeBreadStore =true, availableWheatRyeBreadStore = true, fullRyeBreadStore = false, fullWheatRyeBreadStore = false;
bool availableRyeBreadAmount = false, availableWheatRyeAmount = false;
int amountOfRyeFlour=10, amountOfWheatFlour = 10, amountOfRyeBread = 0, amountOfRyeWheatBread=0;

public:
    Bakery();
    void simulatingBakeryLife();
    bool isNeededRyeFlour();
    bool isNeededWheatRyeFlour();
    void loadRyeFlour(int weight);
    void loadWheatFlour(int weight);
    void refreshWarehousesWithFlour();
    void produceRyeBread();
    void produceWheatRyeBread();
    void runProcessBakeLoading();
    void loadRyeBreadIntoStore();
    void loadRyeWheatBreadIntoStore();
    void refreshStore();
    int getAmountOfRyeBread(){return amountOfRyeBread;}
    int getAmountOfWheatRyeBread(){return amountOfRyeWheatBread;}
    void sellRyeBread(int weight);
    void sellWheatRyeBread(int weight);
    bool getAvailableRyeBreadStore(){return this->availableRyeBreadStore;}
    bool getAvailableWheatRyeBreadStore(){return this->availableWheatRyeBreadStore;}
    bool getAvailableRyeBreadAmount(){ return availableRyeBreadAmount;}
    bool getAvailableWheatRyeBreadAmount(){return availableWheatRyeAmount;}
    void setAvailableRyeBreadAmount(bool flag) {this->availableRyeBreadAmount = flag;}
    void setAvailableWheatRyeBreadAmount(bool flag){this->availableWheatRyeAmount = flag;}
};

#endif
