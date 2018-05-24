//
// Created by mateusz on 11.05.18.
//

#ifndef PROGRAM_FIELD_H
#define PROGRAM_FIELD_H

#include <thread>
#include "Road.h"
#include <vector>
#include <unistd.h>
#include <mutex>
#include <ncurses.h>

extern mutex mutexConsole;

using namespace std;

class Field {

    vector<Coordinate> area;
    int typeOfCrop; // 0 - wheat , 1 - rye
    bool isAvailable = true;

public:
    Field(int typeOfCorp);
    Field();

    vector<Coordinate> getArea() const;
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
