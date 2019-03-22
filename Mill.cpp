#include "Mill.h"

extern mutex mutexConsole;
extern bool endProgram;
mutex mutexMill, mutexTank;
mutex mutexStore;
condition_variable cond_RyeTank, cond_WheatTank, cond_WheatFlourStore, cond_RyeFlourStore;
extern condition_variable cond_amountRyeMillFlour, cond_amountWheatMillFlour;

Mill::Mill(){
    amountOfRyeGrains = 0;
    amountOfWheatGrains = 0;
    mutexConsole.lock();
    refreshWarehouse();
    mutexConsole.unlock();
}

void Mill::takeGrainsFromFarmer(string typeField){
    usleep(rand()%100000+2000000);
    if(typeField == "goFieldWithRye"){
        {
            unique_lock<mutex> locker_RyeTank(mutexTank);
            cond_RyeTank.wait(locker_RyeTank, [&]{return getAvailableRyeTank();});
            this->availableRyeTank = false;
            this->amountOfRyeGrains+=30;
            if(this->amountOfRyeGrains<90){
                this->availableRyeTank = true;
                cond_RyeTank.notify_all();
            }
            else{
                this->availableRyeTank = false;
            }
        }
    }
    else{
        {
            unique_lock<mutex> locker_WheatTank(mutexTank);
            cond_WheatTank.wait(locker_WheatTank, [&]{return getAvailableWheatTank();});
            this->availableWheatTank = false;
            this->amountOfWheatGrains+=30;
            if(this->amountOfWheatGrains<90){
                this->availableWheatTank = true;
                cond_WheatTank.notify_all();
            }
            else{
                this->availableWheatTank = false;
            }
        }
    }
    mutexConsole.lock();
    refreshTanks();
    mutexConsole.unlock();
    usleep(rand()%2000000);
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
        mutexStore.lock();
        if(amountOfRyeFlour <= amountOfWheatFlour && amountOfRyeGrains != 0){
            mutexStore.unlock();
            produceRyeFlour();
        }
        else{
            mutexStore.unlock();
            produceWheatFlour();
        }
        mutexStore.lock();
        if(amountOfRyeFlour>=10)
        {
            this->availableAmountRyeFlour = true;
            cond_amountRyeMillFlour.notify_all();
        }
        else if(amountOfWheatFlour >= 10){
            this->availableAmountWheatFlour = true;
            cond_amountWheatMillFlour.notify_all();
        }
        else{
            this->availableAmountWheatFlour = false;
            this->availableAmountRyeFlour = false;
        }
        mutexStore.unlock();
        usleep(rand()%300000);
    }
}

void Mill::produceRyeFlour(){
    if(this->amountOfRyeGrains >=30){
        usleep(rand()%100000+100000);
        mutexTank.lock();
        this->amountOfRyeGrains -= 30;
        if(amountOfRyeGrains<90){
            availableRyeTank = true;
            cond_RyeTank.notify_one();
        }
        mutexTank.unlock();
        mutexConsole.lock();
        refreshTanks();
        mvprintw(1,39,"%s","rye flour  ");
        mutexConsole.unlock();
        runProcessLoading();
        loadRyeFlourIntoWarehouse();
    }
}

void Mill::produceWheatFlour(){
    if(this->amountOfWheatGrains>=30){
        usleep(rand()%100000+100000);
        mutexTank.lock();
        this->amountOfWheatGrains -= 30;
        if(amountOfWheatGrains<90){
            availableWheatTank = true;
            cond_WheatTank.notify_one();
        }
        mutexTank.unlock();
        mutexConsole.lock();
        refreshTanks();
        mvprintw(1,39,"%s","wheat flour");
        mutexConsole.unlock();
        runProcessLoading();
        loadWheatFlourIntoWarehouse();
    }
}

void Mill::loadWheatFlourIntoWarehouse(){
    {
        unique_lock<mutex> locker_WheatFlour(mutexStore);
        cond_WheatFlourStore.wait(locker_WheatFlour, [&]{return getAvailableWheatWarehouse();});
        this->availableWheatFlourWarehouse = false;
        this->amountOfWheatFlour+=10;
        if(amountOfWheatFlour < 100 ){
            this->availableWheatFlourWarehouse = true;
            cond_WheatFlourStore.notify_one();
        }
        else{
            this->availableWheatFlourWarehouse = false;
        }
        mutexConsole.lock();
        refreshWarehouse();
        mutexConsole.unlock();
        usleep(100000);
    }

}

void Mill::loadRyeFlourIntoWarehouse(){
    {
        unique_lock<mutex> locker_RyeFlour(mutexStore);
        cond_RyeFlourStore.wait(locker_RyeFlour, [&]{return getAvailableRyeWarehouses();});
        this->availableRyeFlourWarehouse = false;
        this->amountOfRyeFlour+=10;
        if(amountOfRyeFlour < 100 ){
            this->availableRyeFlourWarehouse = true;
            cond_RyeFlourStore.notify_one();
        }
        else{
            this->availableWheatFlourWarehouse = false;
        }
        mutexConsole.lock();
        refreshWarehouse();
        mutexConsole.unlock();
        usleep(100000);
    }
}

void Mill::sellRyeFlour(int weight){
    amountOfRyeFlour -= weight;
    if(amountOfRyeFlour<100){
        this->availableRyeFlourWarehouse = true;
        cond_RyeFlourStore.notify_one();
        if(amountOfRyeFlour >= 10){
            this->availableAmountRyeFlour = true;
            cond_amountRyeMillFlour.notify_one();
        }
        else{
            this->availableAmountRyeFlour = false;
        }
    }
    else{
        this->availableRyeFlourWarehouse = false;
    }
    mutexConsole.lock();
    refreshWarehouse();
    mutexConsole.unlock();
    usleep(100000);

}

void Mill::sellWheatFlour(int weight){
    amountOfWheatFlour -= weight;
    if(amountOfWheatFlour<100){
        this->availableWheatFlourWarehouse = true;
        cond_WheatFlourStore.notify_one();
        if(amountOfWheatFlour >= 10){
            this->availableAmountWheatFlour = true;
            cond_amountWheatMillFlour.notify_one();
        }
        else{
            this->availableAmountWheatFlour = false;
        }
    }
    else{
        this->availableWheatFlourWarehouse = false;
    }
    mutexConsole.lock();
    refreshWarehouse();
    mutexConsole.unlock();
    usleep(100000);

}

void Mill::refreshWarehouse(){
    int flourInRyeWarehouse = this->amountOfRyeFlour*20/MAX_FLOUR_IN_WAREHOUSE;
    int flourInWheatWarehouse = this->amountOfWheatFlour*20/MAX_FLOUR_IN_WAREHOUSE;
    int tmp_x=0, tmp_y=0;
    for(int i=0; i<20;i++){
        attron(COLOR_PAIR(4));
        mvprintw(12 + tmp_y, 45 + tmp_x," ");
        mvprintw(9 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
        if(tmp_x%10 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInRyeWarehouse;i++){
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%10 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }

    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<flourInWheatWarehouse;i++){
        attron(COLOR_PAIR(7));
        mvprintw(9 + tmp_y, 45 + tmp_x," ");
        attroff(COLOR_PAIR(7));
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
        usleep(rand()%200000 + 200000);
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
    usleep(rand()%200000+200000);
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
        attron(COLOR_PAIR(4));
        mvprintw(12 + tmp_y, 25 + tmp_x," ");
        mvprintw(12 + tmp_y, 31 + tmp_x," ");
        attroff(COLOR_PAIR(4));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<grainsInRyeTank;i++){
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 25 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }

    tmp_x=0;
    tmp_y=0;
    for(int i=0; i<grainsInWheatTank;i++){
        attron(COLOR_PAIR(3));
        mvprintw(12 + tmp_y, 31 + tmp_x," ");
        attroff(COLOR_PAIR(3));
        tmp_x ++;
        if(tmp_x%3 == 0){
            tmp_x = 0;
            tmp_y --;
        }
    }
}
