#include "Truck.h"
#include "Bakery.h"
#include <unistd.h>

extern bool endProgram;
extern mutex mutexConsole;
extern Bakery bakery;
extern Mill mill;
extern mutex mutexMill;

Road Truck::roadFromBakeryToMill(BakeryToMill);
Road Truck::roadFromMillToBakery(MillToBakery);
/*Road Truck::roadFromShopToMill(ShopToMill);
Road Truck::roadFromMillToShop(MillToShop);
Road Truck::roadFromShopToBakery(ShopToBakery);
Road Truck::roadFromBakeryToShop(BakeryToShop);*/


Truck::Truck(int y, int x, string ID){
    this->x_start = x;
    this->y_start = y;
    this->x = x;
    this->y = y;
    this->ID = ID;
    this->setPosition(y,x);
}

void Truck::setPosition(int y, int x){
    if(ID == "bakeryTruck"){
        mutexConsole.lock();
        init_pair(8, COLOR_BLACK, COLOR_RED);
        attron(COLOR_PAIR(8));
        mvprintw(y,x," ");
        attroff(COLOR_PAIR(8));
        mutexConsole.unlock();
    }
    else{

    }
}

void Truck::takeFlourFromMill(string type){
    if(type == "rye"){
        do{
            usleep(10000);
        }while(mill.getAmountOfRyeFlour()<10);
        if(mill.getAmountOfRyeFlour()>=10){
            mill.sellRyeFlour(MAX_LOAD_TRUCK);
        }
    }
    else{
        do{
            usleep(10000);
        }while(mill.getAmountOfWheatFlour()<10);
        if(mill.getAmountOfWheatFlour()>=10){
            mill.sellWheatFlour(MAX_LOAD_TRUCK);
        }
    }
    usleep(rand()%500000 + 100000);
}

void Truck::simulatingLife(){
    while(!endProgram){
        if(bakery.isNeededRyeFlour()){
            roadFromBakeryToMill.moveTruckToDestination(this);
            takeFlourFromMill("rye");
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadRyeFlour(MAX_LOAD_TRUCK);
        }
        if(bakery.isNeededWheatFlour()){
            roadFromBakeryToMill.moveTruckToDestination(this);
            takeFlourFromMill("wheat");
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadWheatFlour(MAX_LOAD_TRUCK);
        }
        usleep(rand()%3000000 + 1000000);
    }
}