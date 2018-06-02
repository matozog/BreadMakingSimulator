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

public:
    Shop();

    void refreshWarehouseWithFlour();
    void refreshWarehouseWithBread();
    void simulatingShopLife();
    string isNeeded(){return whatIsNeeded;}
    void loadBreadToStore(int amountOfRyeBread, int amountOfWheatRyeBrad);
    void loadFlourToStore(int amountOfWheatFlour, int amountOfRyeFlour);

};

#endif