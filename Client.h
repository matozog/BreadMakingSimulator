#ifndef PROGRAM_CLIENT_H
#define PROGRAM_CLIENT_H

#include <iostream>
#include<string>
#include<mutex>
#include "Road.h"

using namespace std;

class Client{


    static Road roadHomeToBakery, roadHomeToShop, roadShopToHome, roadBakeryToHome;
    static const int MAX_LOAD = 5;
    int x, y, x_start, y_start, ID;
    int color = 0;
    string state = "rest", favouriteFlour = "", favouriteBread = "";

public:

    Client();
    Client(int x, int y, int ID);
    int getX(){return x;}
    int getY(){return y;}
    int getID(){return ID;}
    string getState(){return state;}

    void setState(string state){this->state = state;}
    void setX(int x){this->x=x;}
    void setY(int y){this->y=y;}
    void setID(int ID){this->ID=ID;}

    void simulatingLife();
    void setPosition(int y, int x);
    void Resting();
    void goToBakery();
    void goToShop();
    void goFromShopToHome();
    void goFromBakeryToHome();
    void takeFlourFromShop(string type);
    void takeBreadFromBakery(string type);
    void takeBreadFromShop(string type);
};


#endif
