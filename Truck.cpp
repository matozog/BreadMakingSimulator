#include "Truck.h"
#include "Bakery.h"
#include <unistd.h>

extern bool endProgram;
extern mutex mutexConsole;
extern mutex mutexMill;
extern Bakery bakery;
extern Mill mill;
extern mutex mutexStore;
extern mutex mutexMill;
mutex mutexMillGate;
extern condition_variable cond_MillGate;

condition_variable cond_amountRyeMillFlour, cond_amountWheatMillFlour;

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
    this->x = x;
    this->y=y;
    if(this->ID == "bakeryTruck"){
//        mutexConsole.lock();
        attron(COLOR_PAIR(5));
        mvprintw(y,x," ");
        attroff(COLOR_PAIR(5));
//        mutexConsole.unlock();
    }
    else if(this->ID == "shopTruck"){
//        mutexConsole.lock();
        attron(COLOR_PAIR(6));
        mvprintw(y,x," ");
        attroff(COLOR_PAIR(6));
//        mutexConsole.unlock();
    }
}

void Truck::takeFlourFromMill(string type, int weight){
    {

        if(type == "rye"){
           unique_lock<mutex> locker_millFlourWarehouses(mutexStore);
           cond_amountRyeMillFlour.wait(locker_millFlourWarehouses, [&]{return mill.getAvailableAmountRyeFlour() ;});
           mill.setStatusAmountOfRyeFlour(false);
           mill.sellRyeFlour(weight);
        }
        else{
           unique_lock<mutex> locker_millFlourWarehouses(mutexStore);
           cond_amountWheatMillFlour.wait(locker_millFlourWarehouses, [&]{return mill.getAvailableAmountWheatFlour() ;});
           mill.setStatusAmountOfWheatFlour(false);
           mill.sellRyeFlour(weight/2);
           mill.sellWheatFlour(weight/2);
        }
    }
    usleep(rand()%500000 + 100000);
}

void Truck::simulatingLife(){
    while(!endProgram){
        if(bakery.isNeededRyeFlour()){
            roadFromBakeryToMillGate.moveTruckToDestination(this);
            usleep(50000);
            {
                unique_lock<mutex> locker_MillGate(mutexMillGate);
                cond_MillGate.wait(locker_MillGate, [&]{return mill.getAvailableMillWarehouses();});
                mill.setAvailableMillWarehouses(false);
                usleep(200000);
                roadBakeryFromGateToMill.moveTruckToDestination(this);
                takeFlourFromMill("rye", MAX_LOAD_TRUCK);
                mill.setAvailableMillWarehouses(true);
                cond_MillGate.notify_one();
            }
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadRyeFlour(MAX_LOAD_TRUCK);
        }
        if(bakery.isNeededWheatRyeFlour()){
            roadFromBakeryToMillGate.moveTruckToDestination(this);
            usleep(50000);
            {
            unique_lock<mutex> locker_MillGate(mutexMillGate);
            cond_MillGate.wait(locker_MillGate, [&]{return mill.getAvailableMillWarehouses();});
            mill.setAvailableMillWarehouses(false);
            roadBakeryFromGateToMill.moveTruckToDestination(this);
            takeFlourFromMill("wheat-rye", MAX_LOAD_TRUCK);
            mill.setAvailableMillWarehouses(true);
            cond_MillGate.notify_one();
            }
            roadFromMillToBakery.moveTruckToDestination(this);
            bakery.loadWheatFlour(MAX_LOAD_TRUCK/2);
            bakery.loadRyeFlour(MAX_LOAD_TRUCK/2);
        }
        usleep(rand()%3000000 + 1000000);
    }
}
