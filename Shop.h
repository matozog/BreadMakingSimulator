#ifndef PROGRAM_SHOP_H
#define PROGRAM_SHOP_H

#include<iostream>
#include<thread>
#include "ShopTruck.h"

using namespace std;

class Shop{

static const int MAX_AMOUNT_WHEAT_FLOUR = 55;
static const int MAX_AMOUNT_RYE_FLOUR = 55;
static const int MAX_AMOUNT_RYE_BREAD = 55;
static const int MAX_AMOUNT_RYE_WHEAT_BREAD = 55;
ShopTruck deliveryShopTruck;
thread deliveryShopTruckThread;
string whatIsNeeded = "flour";
int amountOfRyeFlour = 0, amountOfWheatFlour = 0, amountOfRyeBread = 0, amountOfWheatRyeBread = 0;
bool availableStoreWithRyeFlour = true, availableStoreWithWeathFlour = true, availableStoreWithRyeBread = true;
bool availableStoreWithWheatRyeBread=true, availablePlaceInFlourStore = true, availablePlaceInBreadStore=true;

public:
    Shop();

    void refreshWarehouseWithFlour();
    void refreshWarehouseWithBread();
    void simulatingShopLife();
    string isNeeded(){return whatIsNeeded;}
    void loadBreadToStore(int amountOfRyeBread, int amountOfWheatRyeBrad);
    void loadFlourToStore(int amountOfWheatFlour, int amountOfRyeFlour);
    bool getAvailableStoreRFlour(){return availableStoreWithRyeFlour;}
    bool getAvailableStoreWFlour(){return availableStoreWithWeathFlour;}
    bool getAvailableStoreRBread(){return availableStoreWithRyeBread;}
    bool getAvailableStoreWRBread(){return availableStoreWithWheatRyeBread;}
    void setAvailableStoreRFlour(bool flag){this->availableStoreWithRyeFlour = flag;}
    void setAvailableStoreWFlour(bool flag){this->availableStoreWithWeathFlour = flag;}
    void setAvailableStoreRBread(bool flag){this->availableStoreWithRyeBread = flag;}
    void setAvailableStoreWRBread(bool flag){this->availableStoreWithWheatRyeBread = flag;}
    bool getAvailablePlaceInFStore(){return availablePlaceInFlourStore;}
    void setAvailablePlaceInFStore(bool flag){this->availablePlaceInFlourStore = flag;}
    bool getAvailablePlaceInBStore(bool flag){this->availablePlaceInBreadStore = flag;}
};

#endif
