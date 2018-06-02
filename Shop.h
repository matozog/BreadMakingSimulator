#ifndef PROGRAM_SHOP_H
#define PROGRAM_SHOP_H

#include<iostream>
#include<thread>
#include "ShopTruck.h"

using namespace std;

class Shop{

static const int MAX_AMOUNT_WHEAT_FLOUR = 54;
static const int MAX_AMOUNT_RYE_FLOUR = 54;
static const int MAX_AMOUNT_RYE_BREAD = 40;
static const int MAX_AMOUNT_RYE_WHEAT_BREAD = 40;
ShopTruck deliveryShopTruck;
thread deliveryShopTruckThread;
bool neededFlour = true, neededBread = true;

public:
    Shop();

    void refreshWarehouseWithFlour();
    void refreshWarehouseWithBread();
    void simulatingShopLife();
    bool isNeededFlour(){return neededFlour;}
    bool isNeededBread(){return neededBread;}

};

#endif