#include "Mill.h"

extern mutex mutexConsole;
extern bool endProgram;
mutex mutexMill;
condition_variable cond_RyeTank, cond_WheatTank;

Mill::Mill(){
    amountOfRyeGrains = 0;
    amountOfWheatGrains = 0;
    amountOfWheatFlour = 0;
    amountOfRyeFlour = 0;
}

void Mill::takeGrainsFromFarmer(string typeField){

    if(typeField == "goFieldWithRye"){
        {
            unique_lock<mutex> locker_RyeTank(mutexMill);
            cond_RyeTank.wait(locker_RyeTank, [&]{return getAvailableRyeTank();});
            this->amountOfRyeGrains+=30;
            if(this->amountOfRyeGrains<90){
                this->availableRyeTank = true;
                cond_RyeTank.notify_one();
            }
            else{
                this->availableRyeTank = false;
            }
        }
    }
    else{
        {
            unique_lock<mutex> locker_WheatTank(mutexMill);
            cond_WheatTank.wait(locker_WheatTank, [&]{return getAvailableWheatTank();});
            this->amountOfWheatGrains+=30;
            if(this->amountOfWheatGrains<90){
                this->availableWheatTank = true;
                cond_WheatTank.notify_one();
            }
            else{
                this->availableWheatTank = false;
            }
        }
    }
    mutexMill.lock();
    refreshTanks();
    mutexMill.unlock();
    usleep(rand()%4000000);
}

bool Mill::getAvailableRyeTank(){
    return this->availableRyeTank;
}

bool Mill::getAvailableWheatTank(){
    return this->availableWheatTank;
}

void Mill::makeFlour(){
    mutexConsole.lock();
    mvprintw(11,37,"%s","R flour");
    mvprintw(8,37,"%s", "W flour");
    mutexConsole.unlock();
    while(!endProgram){
        usleep(rand()%500000);
        mutexMill.lock();
        if(amountOfRyeFlour <= amountOfWheatFlour){
            mutexMill.unlock();
            produceRyeFlour();
        }
        else{
            mutexMill.unlock();
            produceWheatFlour();
        }
        usleep(rand()%500000);
    }
}

void Mill::produceRyeFlour(){
    if(this->amountOfRyeGrains >=30){
        mutexMill.lock();
        this->amountOfRyeGrains -= 30;
        if(amountOfRyeGrains<90){
            availableRyeTank = true;
            cond_RyeTank.notify_one();
        }
        refreshTanks();
        mutexMill.unlock();
        mutexConsole.lock();
        mvprintw(1,39,"%s","rye flour  ");
        mutexConsole.unlock();
        runProcessLoading();
        loadRyeFlourIntoWarehouse();
    }
}

void Mill::produceWheatFlour(){
    if(this->amountOfWheatGrains>=30){
        mutexMill.lock();
        this->amountOfWheatGrains -= 30;
        if(amountOfWheatGrains<90){
            availableWheatTank = true;
            cond_WheatTank.notify_one();
        }
        refreshTanks();
        mutexMill.unlock();
        mutexConsole.lock();
        mvprintw(1,39,"%s","wheat flour");
        mutexConsole.unlock();
        runProcessLoading();
        loadWheatFlourIntoWarehouse();
    }
}

void Mill::loadWheatFlourIntoWarehouse(){
    do{
        if(this->availableWheatFlourWarehouse){
            mutexMill.lock();
            this->amountOfWheatFlour+=10;
            mutexMill.unlock();
        }
        if(this->amountOfWheatFlour<100){
            mutexMill.lock();
            this->availableWheatFlourWarehouse = true;
            mutexMill.unlock();
        }
    }while(!this->availableWheatFlourWarehouse);
    mutexMill.lock();
    if(this->amountOfWheatFlour >= 100) this->availableWheatFlourWarehouse = false;
    mutexMill.unlock();
    refreshWarehouse();
}

void Mill::loadRyeFlourIntoWarehouse(){
    do{
        if(this->availableRyeFlourWarehouse){
            mutexMill.lock();
            this->amountOfRyeFlour+=10;
            mutexMill.unlock();
        }
        if(this->amountOfRyeFlour<100){
            mutexMill.lock();
            this->availableRyeFlourWarehouse = true;
            mutexMill.unlock();
        }
    }while(!this->availableRyeFlourWarehouse);
    mutexMill.lock();
    if(this->amountOfRyeFlour >= 100) this->availableRyeFlourWarehouse = false;
    mutexMill.unlock();
    refreshWarehouse();
}

void Mill::sellRyeFlour(int weight){
    mutexMill.lock();
    amountOfRyeFlour -= weight;
    mutexMill.unlock();
    refreshWarehouse();
}

void Mill::sellWheatFlour(int weight){
    mutexMill.lock();
    amountOfWheatFlour -= weight;
    mutexMill.unlock();
    refreshWarehouse();
}

void Mill::refreshWarehouse(){
    int flourInRyeWarehouse = this->amountOfRyeFlour*20/MAX_FLOUR_IN_WAREHOUSE;
    int flourInWheatWarehouse = this->amountOfWheatFlour*20/MAX_FLOUR_IN_WAREHOUSE;
    int tmp_x=0, tmp_y=0;
    for(int i=0; i<20;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(12 + tmp_y, 45 + tmp_x," ");
        mvprintw(9 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%10 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%10 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }

    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(7));
        mvprintw(9 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(7));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%10 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
}

void Mill::runProcessLoading(){
    int tmp_x=0;
    for(int i=0; i<7; i++){
        usleep(rand()%700000 + 500000);
        mutexConsole.lock();
        attron(COLOR_PAIR(5));
        mvprintw(5,39+tmp_x," ");
        mvprintw(4,39+tmp_x," ");
        mvprintw(5,40+tmp_x," ");
        mvprintw(4,40+tmp_x," ");
        attroff(COLOR_PAIR(5));
        mutexConsole.unlock();
        tmp_x+=2;
    }
    usleep(500000);
    tmp_x=0;
    for(int i=0; i<7; i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(5,39+tmp_x," ");
        mvprintw(4,39+tmp_x," ");
        mvprintw(5,40+tmp_x," ");
        mvprintw(4,40+tmp_x," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_x+=2;
    }
}

void Mill::refreshTanks(){
    int grainsInRyeTank = this->amountOfRyeGrains*18/MAX_AMOUNT_GRAINS_IN_TANK;
    int grainsInWheatTank = this->amountOfWheatGrains*18/MAX_AMOUNT_GRAINS_IN_TANK;
    int tmp_x=0, tmp_y=0;
    for(int i=0; i<18;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(4));
        mvprintw(12 + tmp_y, 25 + tmp_x," ");
        mvprintw(12 + tmp_y, 31 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<grainsInRyeTank;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 25 + tmp_x," ");
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
    for(int i=0; i<grainsInWheatTank;i++){
        mutexConsole.lock();
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 31 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        mutexConsole.unlock();
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
}
