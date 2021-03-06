#ifndef PROGRAM_ROAD_H
#define PROGRAM_ROAD_H

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
    ShopTruckFromGateToMill,
    ClHomeToShop,
    ClShopToHome,
    ClHomeToBakery,
    ClBakeryToHome
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
    void moveClientToDestination(class Client* client);
    ID_Road getID();
    bool availableNextField(Coordinate nextField);
    vector<Coordinate> getRoad(){return road;}
};

#endif //PROGRAM_ROAD_H
