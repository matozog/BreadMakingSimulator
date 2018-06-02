#include "ShopTruck.h"
#include "Shop.h"

extern bool endProgram;
extern mutex mutexMill;
extern Mill mill;
extern Shop shop;
extern Bakery bakery;

ShopTruck::ShopTruck(){

}

ShopTruck::ShopTruck(int y, int x, string ID):Truck(y,x,ID){
    this->x = x;
    this->x_start = x;
    this->y = y;
    this->y_start = y;
}

void ShopTruck::simulatingLife(){
    while(!endProgram){
        if(shop.isNeeded() == "flour"){
            roadFromShopToMillGate.moveTruckToDestination(this);
            usleep(50000);
            do{
                usleep(1000);
            }while(!mill.getAvailableMillWarehouses());
            if(mill.getAvailableMillWarehouses()){
                mutexMill.lock();
                mill.setAvailableMillWarehouses(false);
                mutexMill.unlock();
            }
            roadShopFromGateToMill.moveTruckToDestination(this);
            takeFlourFromMill("rye", MAX_LOAD_TRUCK/2);
            takeFlourFromMill("wheat-rye", MAX_LOAD_TRUCK/2);
            usleep(rand()%500000+2000000);
            mutexMill.lock();
            mill.setAvailableMillWarehouses(true);
            mutexMill.unlock();
            roadFromMillToShop.moveTruckToDestination(this);
            shop.loadFlourToStore(MAX_LOAD_TRUCK/2, MAX_LOAD_TRUCK/2);
        }
        else if(shop.isNeeded() == "bread"){
            roadFromShopToBakery.moveTruckToDestination(this);
            takeBreadFromBakery("wheat-rye", MAX_LOAD_TRUCK/2);
            takeBreadFromBakery("rye", MAX_LOAD_TRUCK/2);
            usleep(rand()%500000+2000000);
            roadFromBakeryToShop.moveTruckToDestination(this);
            shop.loadBreadToStore(MAX_LOAD_TRUCK/2, MAX_LOAD_TRUCK/2);
        }
    }
    usleep(rand()%3000000 + 1000000);
}

void ShopTruck::takeBreadFromBakery(string type, int weight){
    if(type == "rye"){
        do{
            usleep(10000);
        }while(bakery.getAmountOfRyeBread()<weight);
        
        if(bakery.getAmountOfRyeBread()>=weight){
            bakery.sellRyeBread(weight);
        }
    }
    else{
        do{
            usleep(10000);
        }while((bakery.getAmountOfWheatRyeBread()<weight));
        if(bakery.getAmountOfWheatRyeBread()>=weight){
            bakery.sellWheatRyeBread(weight);
        }
    }
    usleep(rand()%500000 + 100000);
}