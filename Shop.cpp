#include "Shop.h"

extern bool endProgram;
extern mutex mutexConsole;

Shop::Shop(){

}

void Shop::simulatingShopLife(){
    this->deliveryShopTruck = ShopTruck(22,65, "shopTruck");
    this->deliveryShopTruckThread = thread(&ShopTruck::simulatingLife, &deliveryShopTruck);
    while(!endProgram){

    }
}