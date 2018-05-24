//
// Created by mateusz on 11.05.18.
//

#ifndef PROGRAM_ROAD_H
#define PROGRAM_ROAD_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct Coordinate{
    int x, y;
    bool available;
    int stateOfCrop = 0; // only for Field class: 0 - lack, 1 - growing , 2 - ready for harvest
};

class Road {
    string typeOfRoad;
    vector<Coordinate> road;
    Coordinate start, end;
};


#endif //PROGRAM_ROAD_H
