#include "ShopTruck.h"
#include "Shop.h"
#include <condition_variable>

extern bool endProgram;
extern mutex mutexMill, mutexStore, mutexMillGate;
condition_variable cond_MillGate, cond_BreadStores;
extern Mill mill;
extern Shop shop;
extern Bakery bakery;
extern mutex mutexStoreShop, mutexBreadStore;

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
        mutexStoreShop.lock();
        if(shop.isNeeded() == "flour"){
            mutexStoreShop.unlock();
            roadFromShopToMillGate.moveTruckToDestination(this);
            {
                unique_lock<mutex> locker_MillGate(mutexMillGate);
                cond_MillGate.wait(locker_MillGate, [&]{return mill.getAvailableMillWarehouses();});
                mill.setAvailableMillWarehouses(false);
                usleep(200000);
                roadShopFromGateToMill.moveTruckToDestination(this);
                takeFlourFromMill("rye", MAX_LOAD_TRUCK/2);
                takeFlourFromMill("wheat", MAX_LOAD_TRUCK/2);
                mill.setAvailableMillWarehouses(true);
                cond_MillGate.notify_one();
            }
            usleep(rand()%200000+300000);
            roadFromMillToShop.moveTruckToDestination(this);
            shop.loadFlourToStore(MAX_LOAD_TRUCK/2, MAX_LOAD_TRUCK/2);
        }
        else if(shop.isNeeded() == "bread"){
            mutexStoreShop.unlock();
            roadFromShopToBakery.moveTruckToDestination(this);
            takeBreadFromBakery("wheat-rye", MAX_LOAD_TRUCK/2);
            takeBreadFromBakery("rye", MAX_LOAD_TRUCK/2);
            usleep(rand()%500000+2000000);
            roadFromBakeryToShop.moveTruckToDestination(this);
            shop.loadBreadToStore(MAX_LOAD_TRUCK/2, MAX_LOAD_TRUCK/2);
        }
        else mutexStoreShop.unlock();
    }
    usleep(rand()%3000000 + 1000000);

}

void ShopTruck::takeBreadFromBakery(string type, int weight){
    if(type == "rye"){
        {
            unique_lock<mutex> locker_RyeBreadStore(mutexBreadStore);
            cond_BreadStores.wait(locker_RyeBreadStore, [&]{return bakery.getAvailableRyeBreadAmount();});
            bakery.setAvailableRyeBreadAmount(false);
            bakery.sellRyeBread(weight);
            if(bakery.getAmountOfRyeBread()>=5){
                bakery.setAvailableRyeBreadAmount(true);
                cond_BreadStores.notify_one();
            }
        }
    }
    else{
        {
            unique_lock<mutex> locker_WheatRyeBreadStore(mutexBreadStore);
            cond_BreadStores.wait(locker_WheatRyeBreadStore, [&]{return bakery.getAvailableWheatRyeBreadAmount();});
            bakery.setAvailableWheatRyeBreadAmount(false);
            bakery.sellWheatRyeBread(weight);
            if(bakery.getAmountOfWheatRyeBread()>=5){
                bakery.setAvailableWheatRyeBreadAmount(true);
                cond_BreadStores.notify_one();
            }
        }
    }
    usleep(rand()%500000 + 100000);

}
