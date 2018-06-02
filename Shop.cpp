#include "Shop.h"

extern bool endProgram;
extern mutex mutexConsole;

Shop::Shop(){

}

void Shop::simulatingShopLife(){
    this->deliveryShopTruck = ShopTruck(22,65, "shopTruck");
    this->deliveryShopTruckThread = thread(&ShopTruck::simulatingLife, &deliveryShopTruck);
    while(!endProgram){
        if((amountOfRyeBread + amountOfWheatRyeBread) < (amountOfWheatFlour + amountOfRyeFlour)){
            whatIsNeeded = "bread";
        }
        else{
            whatIsNeeded = "flour";
        }
    }
    this->deliveryShopTruckThread.join();
}

void Shop::loadBreadToStore(int amountOfRyeBread, int amountOfWheatRyeBread){
    this->amountOfRyeBread += amountOfRyeBread;
    this->amountOfWheatRyeBread += amountOfWheatRyeBread;
    refreshWarehouseWithBread();
}

void Shop::loadFlourToStore(int amountOfRyeFlour, int amountOfWheatFlour){
    this->amountOfRyeFlour += amountOfRyeFlour;
    this->amountOfWheatFlour += amountOfWheatFlour;
    refreshWarehouseWithFlour();
}

void Shop::refreshWarehouseWithBread(){
    
}

void Shop::refreshWarehouseWithFlour(){

}