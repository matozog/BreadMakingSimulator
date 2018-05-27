#ifndef PROGRAM_ROAD_H
#define PROGRAM_ROAD_H

#pragma once

#include "Field.h"
#include <vector>
#include <mutex>

using namespace std;

enum ID_Road{
    HomeToFWheat,
    HomeToFRye,
    FWheatToMill,
    FRyeToMill,
    MillToHome
};

class Road {
    vector<CoordinateField> road;
    Coordinate start, end;
    ID_Road ID;
    bool available;

public:
    Road(ID_Road);
    void createRoad(vector<CoordinateField> tmpRoad);
    void moveFarmerToDestination(class Farmer* farmer);
    bool isAvailable();
    void refreshRoad();
    ID_Road getID();
};

#endif //PROGRAM_ROAD_H
