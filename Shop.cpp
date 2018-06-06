#include "Shop.h"

extern bool endProgram;
extern mutex mutexConsole;
mutex mutexStoreShop;

Shop::Shop(){

}

void Shop::simulatingShopLife(){
    mutexConsole.lock();
    attron(COLOR_PAIR(2));
    mvprintw(20, 44,"%s", "rye F");
    mvprintw(23, 44,"%s", "wheat F");
    mvprintw(26, 44 ,"%s", "rye B");
    mvprintw(29, 44 ,"%s", "wheat-rye B");
    attroff(COLOR_PAIR(2));
    mutexConsole.unlock();
    this->deliveryShopTruck = ShopTruck(22,65, "shopTruck");
    this->deliveryShopTruckThread = thread(&ShopTruck::simulatingLife, &deliveryShopTruck);
    while(!endProgram){
        mutexStoreShop.lock();
        if((amountOfRyeBread + amountOfWheatRyeBread) < (amountOfWheatFlour + amountOfRyeFlour)){
            whatIsNeeded = "bread";
        }
        else{
            whatIsNeeded = "flour";
        }
        mutexStoreShop.unlock();
    }
    this->deliveryShopTruckThread.join();
}

void Shop::loadBreadToStore(int amountOfRyeBread, int amountOfWheatRyeBread){
    mutexStoreShop.lock();
    this->amountOfRyeBread += amountOfRyeBread;
    this->amountOfWheatRyeBread += amountOfWheatRyeBread;
    mutexStoreShop.unlock();
    refreshWarehouseWithBread();
}

void Shop::loadFlourToStore(int amountOfRyeFlour, int amountOfWheatFlour){
    mutexStoreShop.lock();
    this->amountOfRyeFlour += amountOfRyeFlour;
    this->amountOfWheatFlour += amountOfWheatFlour;
    mutexStoreShop.unlock();
    refreshWarehouseWithFlour();
}

void Shop::refreshWarehouseWithBread(){
    int breadInRyeWarehouse = this->amountOfRyeBread*11/MAX_AMOUNT_RYE_FLOUR;
    int breadInWheatWarehouse = this->amountOfWheatRyeBread*11/MAX_AMOUNT_WHEAT_FLOUR;
    int tmp_x=0;
    for(int i=0; i<11;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(27, 44 + tmp_x," ");
        mvprintw(28, 44 + tmp_x," ");
        mvprintw(30, 44 + tmp_x," ");
        mvprintw(31, 44 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<breadInRyeWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(27, 44 + tmp_x," ");
        mvprintw(28, 44 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<breadInWheatWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(7));
        mvprintw(30, 44 + tmp_x," ");
        mvprintw(31, 44 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
}

void Shop::refreshWarehouseWithFlour(){
    int flourInRyeWarehouse = this->amountOfRyeFlour*11/MAX_AMOUNT_RYE_FLOUR;
    int flourInWheatWarehouse = this->amountOfWheatFlour*11/MAX_AMOUNT_WHEAT_FLOUR;
    int tmp_x=0;
    for(int i=0; i<11;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(21, 44 + tmp_x," ");
        mvprintw(22, 44 + tmp_x," ");
        mvprintw(24, 44 + tmp_x," ");
        mvprintw(25, 44 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(21, 44 + tmp_x," ");
        mvprintw(22, 44 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }

    tmp_x=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(7));
        mvprintw(24, 44 + tmp_x," ");
        mvprintw(25, 44 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
}
