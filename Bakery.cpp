#include"Bakery.h"

extern bool endProgram;
extern mutex mutexConsole;
mutex mutexBakery;

//Truck Bakery::deliveryTruck(5,78, "bakeryTruck");

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
            neededWheatFlour = true;
        }
        else neededWheatFlour = false;
        mutexBakery.unlock();
        usleep(5000000);
    }
}

void Bakery::produceRyeBread(){
    mutexBakery.lock();
    amountOfRyeFlour-=9;
    refreshWarehousesWithFlour();
    mutexBakery.unlock();
    mutexConsole.lock();
    attron(COLOR_PAIR(9));
    mvprintw(2,93,"%s","   rye   ");
    attroff(COLOR_PAIR(9));
    mutexConsole.unlock();
    runProcessBakeLoading();
}

void Bakery::produceWheatRyeBread(){
    mutexBakery.lock();
    amountOfRyeFlour-=6;
    amountOfWheatFlour-=6;
    refreshWarehousesWithFlour();
    mutexBakery.unlock();
    mutexConsole.lock();
    attron(COLOR_PAIR(9));
    mvprintw(2,93,"%s","wheat-rye");
    attroff(COLOR_PAIR(9));
    mutexConsole.unlock();
    runProcessBakeLoading();
}

void Bakery::loadRyeBreadIntoStore(){
    do{
    if(this->availableRyeBreadStore){
        mutexBakery.lock();
        this->amountOfRyeBread+=8;
        mutexBakery.unlock();
    }
    if(this->amountOfRyeBread<40){
        mutexBakery.lock();
        this->availableRyeBreadStore = true;
        mutexBakery.unlock();
    }
    }while(!this->availableRyeBreadStore);
    mutexBakery.lock();
    if(this->amountOfRyeBread >= 40) this->availableRyeBreadStore = false;
    mutexBakery.unlock();
    refreshStore();
}

void Bakery::loadRyeWheatBreadIntoStore(){
    do{
    if(this->availableWheatRyeBreadStore){
        mutexBakery.lock();
        this->amountOfRyeWheatBread+=8;
        mutexBakery.unlock();
    }
    if(this->amountOfRyeWheatBread<40){
        mutexBakery.lock();
        this->availableWheatRyeBreadStore = true;
        mutexBakery.unlock();
    }
    }while(!this->availableWheatRyeBreadStore);
    mutexBakery.lock();
    if(this->amountOfRyeWheatBread >= 40) this->availableWheatRyeBreadStore = false;
    mutexBakery.unlock();
    refreshStore();
}

void Bakery::refreshStore(){
    int breadInRyeStore = this->amountOfRyeBread*10/MAX_AMOUNT_RYE_BREAD;
    int breadInRyeWheatStore = this->amountOfRyeWheatBread*10/MAX_AMOUNT_RYE_WHEAT_BREAD;
    int tmp_x=0;
    for(int i=0; i<10;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(6));
        mvprintw(6, 103 + tmp_x," ");
        mvprintw(3, 103 + tmp_x," ");
        attroff(COLOR_PAIR(6));
        mutexConsole.unlock();
        tmp_x ++;
    }
    tmp_x=0;
    for(int i=0; i<breadInRyeStore;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(2));
        mvprintw(6, 103 + tmp_x," ");
        attroff(COLOR_PAIR(2));
        mutexConsole.unlock();
        tmp_x ++;
    }

    tmp_x=0;
    for(int i=0; i<breadInRyeWheatStore;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(7));
        mvprintw(3, 103 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        mutexConsole.unlock();
        tmp_x ++;
    }
}

void Bakery::runProcessBakeLoading(){
    int tmp_y=0;
    for(int i=0; i<5; i++){
        usleep(rand()%700000 + 500000);
        mutexConsole.lock();
        init_pair(5, COLOR_BLACK, COLOR_RED);
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
    usleep(500000);
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
    refreshWarehousesWithFlour();
}

void Bakery::loadWheatFlour(int weight){
    this->amountOfWheatFlour += weight;
    refreshWarehousesWithFlour();
}

void Bakery::refreshWarehousesWithFlour(){
    int flourInRyeWarehouse = this->amountOfRyeFlour*18/MAX_AMOUNT_RYE_FLOUR;
    int flourInWheatWarehouse = this->amountOfWheatFlour*18/MAX_AMOUNT_WHEAT_FLOUR;
    int tmp_x=0, tmp_y=0;
    for(int i=0; i<18;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(6));
        mvprintw(8 + tmp_y, 82 + tmp_x," ");
        mvprintw(8 + tmp_y, 88 + tmp_x," ");
        attroff(COLOR_PAIR(6));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(8 + tmp_y, 82 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }

    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        mutexConsole.lock();
        init_pair(7, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(7));
        mvprintw(8 + tmp_y, 88 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        mutexConsole.unlock();
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

bool Bakery::isNeededWheatFlour(){
    return neededWheatFlour;
}
