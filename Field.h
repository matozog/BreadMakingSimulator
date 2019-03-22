
#ifndef PROGRAM_FIELD_H
#define PROGRAM_FIELD_H

#include <vector>
#include <atomic>
#include <mutex>
#include <unistd.h>
#include <ncurses.h>

using namespace std;

struct CoordinateField{
    int y, x, stateOfCrop, type; // state of Crop only for Field class: 0 - lack, 1 - growing , 2 - ready for harvest
    bool available;
};

struct Coordinate{
    int y, x;
    bool available;
};

class Field {

    vector<CoordinateField> area;
    int typeOfCrop; // 0 - wheat , 1 - rye
    bool isAvailable = true;
    int amountOfGrains = 0; // in tons

public:
    Field(int typeOfCorp);
    Field();

    vector<CoordinateField> getArea();
    void setArea(int x, int y,int type);
    int getTypeOfCrop();
    void setTypeOfCrop(int typeOfCrop);
    bool getAvailable();
    void setAvailable(bool available);
    void refreshCrops();
    void closeThread();

    bool isReadyToHarvest();
    int harvest(class Farmer *farmer);
    void natureThread();

};

#endif //PROGRAM_FIELD_H
