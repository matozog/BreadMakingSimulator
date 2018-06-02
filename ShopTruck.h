#ifndef PROGRAM_SHOP_TRUCK_H
#define PROGRAM_SHOP_TRUCK_H

#include "Truck.h"
using namespace std;

class ShopTruck : public Truck{

public:
    bool checkAmountOfWheatBread();
    bool checkAmountOfRyeBread();

};

#endif