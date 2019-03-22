#include "Shop.h"

extern bool endProgram;
extern mutex mutexConsole;
mutex mutexStoreShop;
extern condition_variable cond_ShopFlourStore, cond_ShopBreadStore;

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
    this->availableStoreWithRyeBread = true;
    this->availableStoreWithWheatRyeBread = true;
    cond_ShopBreadStore.notify_one();
    if(amountOfRyeBread <=55 && amountOfWheatRyeBread <=55){
        this->availablePlaceInBreadStore = true;
    }
    else{
        this->availablePlaceInBreadStore = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithBread();
    mutexConsole.unlock();
    usleep(300000);
    mutexStoreShop.unlock();

}

void Shop::loadFlourToStore(int amountOfRyeFlour, int amountOfWheatFlour){
    mutexStoreShop.lock();
    this->amountOfRyeFlour += amountOfRyeFlour;
    this->amountOfWheatFlour += amountOfWheatFlour;
    this->availableStoreWithRyeFlour = true;
    this->availableStoreWithWeathFlour = true;
    cond_ShopFlourStore.notify_one();
    if(amountOfRyeFlour<=55 && amountOfWheatFlour<=55){
        this->availablePlaceInFlourStore = true;
    }
    else{
        this->availablePlaceInFlourStore = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithFlour();
    mutexConsole.unlock();
    usleep(300000);
    mutexStoreShop.unlock();

    //usleep(200000);
}

void Shop::sellRyeBread(int weight){
    this->amountOfRyeBread -= weight;
    if(this->amountOfRyeBread>=4){
        this->availableStoreWithRyeBread = true;
        cond_ShopBreadStore.notify_one();
    }
    else{
        this->availableStoreWithRyeBread = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithBread();
    mutexConsole.unlock();
}

void Shop::sellWheatRyeBread(int weight){
    this->amountOfWheatRyeBread -= weight;
    if(this->amountOfWheatRyeBread>=4){
        this->availableStoreWithRyeBread = true;
        cond_ShopBreadStore.notify_one();
    }
    else{
        this->availableStoreWithRyeBread = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithBread();
    mutexConsole.unlock();
}

void Shop::sellRyeFlour(int weight){
    this->amountOfRyeFlour -= weight;
    if(this->amountOfRyeFlour>=4){
        this->availableStoreWithRyeFlour = true;
        cond_ShopFlourStore.notify_one();
    }
    else{
        this->availableStoreWithRyeFlour = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithFlour();
    mutexConsole.unlock();
}

void Shop::sellWheatFlour(int weight){
    this->amountOfWheatFlour -= weight;
    if(this->amountOfWheatFlour>=4){
        this->availableStoreWithWeathFlour = true;
        cond_ShopFlourStore.notify_one();
    }
    else{
        this->availableStoreWithWeathFlour = false;
    }
    mutexConsole.lock();
    refreshWarehouseWithFlour();
    mutexConsole.unlock();
}

void Shop::refreshWarehouseWithBread(){
    int breadInRyeWarehouse = this->amountOfRyeBread*11/MAX_AMOUNT_RYE_FLOUR;
    int breadInWheatWarehouse = this->amountOfWheatRyeBread*11/MAX_AMOUNT_WHEAT_FLOUR;
    int tmp_x=0;
    for(int i=0; i<11;i++){
        attron(COLOR_PAIR(4));
        mvprintw(27, 44 + tmp_x," ");
        mvprintw(28, 44 + tmp_x," ");
        mvprintw(30, 44 + tmp_x," ");
        mvprintw(31, 44 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<breadInRyeWarehouse;i++){
        attron(COLOR_PAIR(3));
        mvprintw(27, 44 + tmp_x," ");
        mvprintw(28, 44 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<breadInWheatWarehouse;i++){
        attron(COLOR_PAIR(7));
        mvprintw(30, 44 + tmp_x," ");
        mvprintw(31, 44 + tmp_x," ");
        attroff(COLOR_PAIR(7));
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
        attron(COLOR_PAIR(4));
        mvprintw(21, 44 + tmp_x," ");
        mvprintw(22, 44 + tmp_x," ");
        mvprintw(24, 44 + tmp_x," ");
        mvprintw(25, 44 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
    tmp_x=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        attron(COLOR_PAIR(3));
        mvprintw(21, 44 + tmp_x," ");
        mvprintw(22, 44 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }

    tmp_x=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        attron(COLOR_PAIR(7));
        mvprintw(24, 44 + tmp_x," ");
        mvprintw(25, 44 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        tmp_x ++;
        if(tmp_x%11 == 0){
            tmp_x = 0;
        }
    }
}
