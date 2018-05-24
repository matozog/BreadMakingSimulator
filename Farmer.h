#pragma once
#ifndef PROGRAM_FARMER_H
#define PROGRAM_FARMER_H

#include <iostream>
#include <string>
#include <thread>
#include <ncurses.h>
#include <mutex>
#include <unistd.h>

using namespace std;

extern mutex mutexConsole;

class Farmer {

private:
    int x, y, x_start, y_start, ID;
    string state = "rest";

public:
    Farmer();
    Farmer(int x, int y, int ID);

    int getX();
    int getY();
    int getID();
    string getState();

    void setState(string state);
    void setX(int x);
    void setY(int y);
    void setID(int ID);

    void setPosition(int x, int y);
    void goFieldWithWheat();
    void goFieldWithRye();
    void pickUpGrain();
    void sellGrain();
    void goToHome();
    void Resting();
    void simulatingLife();
    void closeThread();
};


#endif //PROGRAM_FARMER_H
