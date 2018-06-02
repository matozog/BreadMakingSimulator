#ifndef PROGRAM_ROAD_H
#define PROGRAM_ROAD_H

#pragma once

#include "Field.h"
#include <vector>
#include <mutex>
#include <string>

using namespace std;

enum ID_Road{
    HomeToFWheat,
    HomeToFRye,
    FWheatToMill,
    FRyeToMill,
    MillToHome,
    BakeryToMillGate,
    MillToBakery,
    ShopToMillGate,
    MillToShop,
    ShopToBakery,
    BakeryToShop,
    BakeryTruckFromGateToMill,
    ShopTruckFromGateToMill
};

class Road {
    vector<Coordinate> road;
    Coordinate start, end;
    ID_Road ID;
    bool available;

public:
    Road(ID_Road);
    void createRoad(vector<Coordinate> tmpRoad);
    void moveFarmerToDestination(class Farmer* farmer);
    bool isAvailable();
    void moveTruckToDestination(class Truck* truck);
    ID_Road getID();
};

#endif //PROGRAM_ROAD_H
