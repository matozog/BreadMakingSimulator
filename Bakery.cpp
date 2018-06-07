#include"Bakery.h"
#include <condition_variable>

condition_variable cond_RyeBreadFull, cond_WheatRyeBreadFull;

extern bool endProgram;
extern mutex mutexConsole;
mutex mutexBakery;
mutex mutexBreadStore;
extern condition_variable cond_BreadStores, cond_ClBakeryStore;

Bakery::Bakery(){

}

void Bakery::simulatingBakeryLife(){
    this->deliveryTruck = Truck(5,78, "bakeryTruck");
    this->deliveryTruckThread = thread(&Truck::simulatingLife, &deliveryTruck);
    while(!endProgram){
        if(amountOfRyeFlour>=9 && amountOfRyeBread <= amountOfRyeWheatBread){
            produceRyeBread();
            loadRyeBreadIntoStore();
        }
        if(amountOfWheatFlour >= 6 && amountOfRyeFlour >=6 && amountOfRyeWheatBread <= amountOfRyeBread){
            produceWheatRyeBread();
            loadRyeWheatBreadIntoStore();
        }
        mutexBakery.lock();
        if(amountOfRyeFlour <= amountOfWheatFlour){
            neededRyeFlour = true;
        }
        else neededRyeFlour = false;
        if(amountOfWheatFlour <= amountOfRyeFlour){
            neededWheatRyeFlour = true;
        }
        else neededWheatRyeFlour = false;
        mutexBakery.unlock();
        usleep(5000000);
    }
    this->deliveryTruckThread.join();
}

void Bakery::produceRyeBread(){
    mutexBakery.lock();
    amountOfRyeFlour-=9;
    mutexConsole.lock();
    refreshWarehousesWithFlour();
    mutexConsole.unlock();
    mutexBakery.unlock();
    mutexConsole.lock();
    attron(COLOR_PAIR(8));
    mvprintw(2,93,"%s","   rye   ");
    attroff(COLOR_PAIR(8));
    mutexConsole.unlock();
    runProcessBakeLoading();
}

void Bakery::produceWheatRyeBread(){
    mutexBakery.lock();
    amountOfRyeFlour-=6;
    amountOfWheatFlour-=6;
    mutexConsole.lock();
    refreshWarehousesWithFlour();
    mutexConsole.unlock();
    mutexBakery.unlock();
    mutexConsole.lock();
    attron(COLOR_PAIR(8));
    mvprintw(2,93,"%s","wheat-rye");
    attroff(COLOR_PAIR(8));
    mutexConsole.unlock();
    runProcessBakeLoading();
}

void Bakery::loadRyeBreadIntoStore(){
    {
        unique_lock<mutex> locker_RyeBread(mutexBreadStore);
        cond_RyeBreadFull.wait(locker_RyeBread, [&]{return getFullRyeBreadStore();});
//        this->availableRyeBreadStore = false;
        //this->fullRyeBreadStore = false;
        this->amountOfRyeBread+=20;
        cond_BreadStores.notify_one();
        cond_ClBakeryStore.notify_one();
        if(this->amountOfRyeBread>20)
        {
            this->fullRyeBreadStore = false;
        }
        else{
            this->fullRyeBreadStore = true;
        }

        if(this->amountOfRyeBread>=8){
            this->availableRyeBreadAmount = true;
            cond_BreadStores.notify_one();
            this->availableRyeBreadStore = true;
            cond_ClBakeryStore.notify_one();
        }
        else if(this->amountOfRyeBread>=4){
            this->availableRyeBreadStore = true;
            cond_ClBakeryStore.notify_one();
        }
        else{
            this->availableRyeBreadStore = false;
            this->availableRyeBreadAmount = false;
        }
    }
    mutexConsole.lock();
    refreshStore();
    mutexConsole.unlock();
    usleep(rand()%100000+1000000);
}

void Bakery::loadRyeWheatBreadIntoStore(){
    {
        unique_lock<mutex> locker_WheatBread(mutexBreadStore);
        cond_WheatRyeBreadFull.wait(locker_WheatBread, [&]{return getFullWheatRyeBreadStore();});
        this->amountOfRyeWheatBread+=20;
        cond_BreadStores.notify_one();
        cond_ClBakeryStore.notify_one();
        if(this->amountOfRyeWheatBread>20)
        {
            this->fullWheatRyeBreadStore = false;
        }
        else{
            this->availableWheatRyeAmount = true;
        }


        if(this->amountOfRyeWheatBread>=8){
            this->availableWheatRyeAmount = true;
            cond_BreadStores.notify_one();
            this->availableWheatRyeBreadStore = true;
            cond_ClBakeryStore.notify_one();
        }
        else if(this->amountOfRyeWheatBread>=4){
            this->availableWheatRyeBreadStore = true;
            cond_ClBakeryStore.notify_one();
        }
        else{
            this->availableWheatRyeBreadStore = false;
            this->availableWheatRyeAmount = false;
        }
    }
    mutexConsole.lock();
    refreshStore();
    mutexConsole.unlock();
     usleep(rand()%100000+1000000);
}

void Bakery::sellRyeBread(int weight){
    amountOfRyeBread -= weight;
    if(this->amountOfRyeBread<=20){
        this->fullRyeBreadStore = true;
        cond_RyeBreadFull.notify_all();
    }
    else this->fullRyeBreadStore = false;

    if(this->amountOfRyeBread>=8){
        this->availableRyeBreadAmount = true;
        cond_BreadStores.notify_one();
        this->availableRyeBreadStore = true;
        cond_ClBakeryStore.notify_one();
    }
    else if(this->amountOfRyeBread>=4){
        this->availableRyeBreadStore = true;
        cond_ClBakeryStore.notify_one();
    }
    else{
        this->availableRyeBreadStore = false;
        this->availableRyeBreadAmount = false;
    }

    mutexConsole.lock();
    refreshStore();
    mutexConsole.unlock();
    usleep(2000000);
}

void Bakery::sellWheatRyeBread(int weight){
    amountOfRyeWheatBread -= weight;
    if(this->amountOfRyeWheatBread<=20){
        this->fullWheatRyeBreadStore = true;
        cond_WheatRyeBreadFull.notify_all();
    }
    else this->fullWheatRyeBreadStore = false;

    if(this->amountOfRyeWheatBread>=8){
        this->availableWheatRyeAmount = true;
        cond_BreadStores.notify_one();
        this->availableWheatRyeBreadStore = true;
        cond_ClBakeryStore.notify_one();
    }
    else if(this->amountOfRyeWheatBread>=4){
        this->availableWheatRyeBreadStore = true;
        cond_ClBakeryStore.notify_one();
    }
    else{
        this->availableWheatRyeBreadStore = false;
        this->availableWheatRyeAmount = false;
    }
    mutexConsole.lock();
    refreshStore();
    mutexConsole.unlock();
    usleep(2000000);
}

void Bakery::refreshStore(){
    int breadInRyeStore = this->amountOfRyeBread*10/MAX_AMOUNT_RYE_BREAD;
    int breadInRyeWheatStore = this->amountOfRyeWheatBread*10/MAX_AMOUNT_RYE_WHEAT_BREAD;
    int tmp_x=0;
    for(int i=0; i<10;i++){
        attron(COLOR_PAIR(4));
        mvprintw(6, 103 + tmp_x," ");
        mvprintw(3, 103 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
    }
    tmp_x=0;
    for(int i=0; i<breadInRyeStore;i++){
        attron(COLOR_PAIR(2));
        mvprintw(6, 103 + tmp_x," ");
        attroff(COLOR_PAIR(2));
        tmp_x ++;
    }

    tmp_x=0;
    for(int i=0; i<breadInRyeWheatStore;i++){
        attron(COLOR_PAIR(7));
        mvprintw(3, 103 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        tmp_x ++;
    }
}

void Bakery::runProcessBakeLoading(){
    int tmp_y=0;
    for(int i=0; i<5; i++){
        usleep(rand()%200000 + 200000);
        mutexConsole.lock();
        attron(COLOR_PAIR(5));
        mvprintw(8+tmp_y,95," ");
        mvprintw(8+tmp_y,96," ");
        mvprintw(8+tmp_y,97," ");
        mvprintw(8+tmp_y,98," ");
        mvprintw(8+tmp_y,99," ");
        attroff(COLOR_PAIR(5));
        mutexConsole.unlock();
        tmp_y--;
    }
    usleep(rand()%200000+200000);
    tmp_y=0;
    for(int i=0; i<5; i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(8+tmp_y,95," ");
        mvprintw(8+tmp_y,96," ");
        mvprintw(8+tmp_y,97," ");
        mvprintw(8+tmp_y,98," ");
        mvprintw(8+tmp_y,99," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_y--;
    }
}

void Bakery::loadRyeFlour(int weight){
    this->amountOfRyeFlour += weight;
    mutexConsole.lock();
    refreshWarehousesWithFlour();
    mutexConsole.unlock();
}

void Bakery::loadWheatFlour(int weight){
    this->amountOfWheatFlour += weight;
    mutexConsole.lock();
    refreshWarehousesWithFlour();
    mutexConsole.unlock();
}

void Bakery::refreshWarehousesWithFlour(){
    int flourInRyeWarehouse = this->amountOfRyeFlour*18/MAX_AMOUNT_RYE_FLOUR;
    int flourInWheatWarehouse = this->amountOfWheatFlour*18/MAX_AMOUNT_WHEAT_FLOUR;
    int tmp_x=0, tmp_y=0;
    for(int i=0; i<18;i++){
        attron(COLOR_PAIR(4));
        mvprintw(8 + tmp_y, 82 + tmp_x," ");
        mvprintw(8 + tmp_y, 88 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        attron(COLOR_PAIR(3));
        mvprintw(8 + tmp_y, 82 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }

    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        attron(COLOR_PAIR(7));
        mvprintw(8 + tmp_y, 88 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
}

bool Bakery::isNeededRyeFlour(){
    return neededRyeFlour;
}

bool Bakery::isNeededWheatRyeFlour(){
    return neededWheatRyeFlour;
}
