
#ifndef PROGRAM_FIELD_H
#define PROGRAM_FIELD_H

#pragma once

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

public:
    Field(int typeOfCorp);
    Field();

    vector<CoordinateField> getArea() const;
    void setArea(int x, int y);
    int getTypeOfCrop() const;
    void setTypeOfCrop(int typeOfCrop);
    bool getAvailable();
    void setAvailable(bool available);
    void refreshCrops();
    void closeThread();

    void natureThread();

};

#endif //PROGRAM_FIELD_H