#include "Truck.h"
#include "Bakery.h"
#include <unistd.h>

extern bool endProgram;
extern mutex mutexConsole;
extern mutex mutexMill;
extern Bakery bakery;
extern Mill mill;
extern mutex mutexMill;

Road Truck::roadFromBakeryToMillGate(BakeryToMillGate);
Road Truck::roadFromMillToBakery(MillToBakery);
Road Truck::roadFromShopToMillGate(ShopToMillGate);
Road Truck::roadFromMillToShop(MillToShop);
Road Truck::roadFromShopToBakery(ShopToBakery);
Road Truck::roadFromBakeryToShop(BakeryToShop);
Road Truck::roadBakeryFromGateToMill(BakeryTruckFromGateToMill);
Road Truck::roadShopFromGateToMill(ShopTruckFromGateToMill);


Truck::Truck(int y, int x, string ID){
    this->x_start = x;
    this->y_start = y;
    this->x = x;
    this->y = y;
    this->ID = ID;
    this->setPosition(y,x);
}

void Truck::setPosition(int y, int x){
    if(this->ID == "bakeryTruck"){
        mutexConsole.lock();
        init_pair(8, COLOR_BLACK, COLOR_RED);
        attron(COLOR_PAIR(8));
        mvprintw(y,x," ");
        attroff(COLOR_PAIR(8));
        mutexConsole.unlock();
    }
    else if(this->ID == "shopTruck"){
        mutexConsole.lock();
        init_pair(10, COLOR_BLACK, COLOR_BLUE);
        attron(COLOR_PAIR(10));
        mvprintw(y,x," ");
        attroff(COLOR_PAIR(10));
        mutexConsole.unlock();
    }
}

void Truck::takeFlourFromMill(string type, int weight){
    if(type == "rye"){
        do{
            usleep(10000);
        }while(mill.getAmountOfRyeFlour()<weight);
        usleep(rand()%500000+2000000);
        if(mill.getAmountOfRyeFlour()>=weight){
            mill.sellRyeFlour(weight);
        }
    }
    else{
        do{
            usleep(10000);
        }while((mill.getAmountOfWheatFlour()<weight/2) && (mill.getAmountOfRyeFlour() <weight/2));
        usleep(rand()%500000+2000000);
        if(mill.getAmountOfWheatFlour()>=weight/2 && mill.getAmountOfRyeFlour() >= weight/2){
            mill.sellWheatFlour(weight/2);
            mill.sellRyeFlour(weight/2);
        }
    }
    usleep(rand()%500000 + 100000);
}

void Truck::simulatingLife(){
    while(!endProgram){
        if(bakery.isNeededRyeFlour()){
            roadFromBakeryToMillGate.moveTruckToDestination(this);
            usleep(50000);
            do{
                usleep(1000);
            }while(!mill.getAvailableMillWarehouses());
            if(mill.getAvailableMillWarehouses()){
                mutexMill.lock();
                mill.setAvailableMillWarehouses(false);
                mutexMill.unlock();
            }
            roadBakeryFromGateToMill.moveTruckToDestination(this);
            takeFlourFromMill("rye", MAX_LOAD_TRUCK);
            mutexMill.lock();
            mill.setAvailableMillWarehouses(true);
            mutexMill.unlock();
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadRyeFlour(MAX_LOAD_TRUCK);
        }
        if(bakery.isNeededWheatRyeFlour()){
            roadFromBakeryToMillGate.moveTruckToDestination(this);
            usleep(50000);
            do{
                usleep(1000);
            }while(!mill.getAvailableMillWarehouses());
            if(mill.getAvailableMillWarehouses()){
                mutexMill.lock();
                mill.setAvailableMillWarehouses(false);
                mutexMill.unlock();
            }
            roadBakeryFromGateToMill.moveTruckToDestination(this);
            takeFlourFromMill("wheat-rye", MAX_LOAD_TRUCK);
            mutexMill.lock();
            mill.setAvailableMillWarehouses(true);
            mutexMill.unlock();
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadWheatFlour(MAX_LOAD_TRUCK/2);
            bakery.loadRyeFlour(MAX_LOAD_TRUCK/2);
        }
        usleep(rand()%3000000 + 1000000);
    }
}