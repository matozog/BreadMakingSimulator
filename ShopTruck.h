#ifndef PROGRAM_SHOP_TRUCK_H
#define PROGRAM_SHOP_TRUCK_H

#include "Truck.h"
using namespace std;

class ShopTruck : public Truck{

public:
    ShopTruck();
    ShopTruck(int y, int x, string ID);
    bool checkAmountOfWheatBread();
    bool checkAmountOfRyeBread();
    void simulatingLife();

};

#endif