#include "Mill.h"

extern mutex mutexConsole;
extern bool endProgram;
mutex mutexMill;

Mill::Mill(){
    amountOfRyeGrains = 0;
    amountOfWheatGrains = 0;
}

void Mill::takeGrainsFromFarmer(string typeField){

    if(typeField == "goFieldWithRye"){
        do{
            if(this->availableRyeTank){
                mutexMill.lock();
                this->amountOfRyeGrains+=30;
                mutexMill.unlock();
            }
            if(this->amountOfRyeGrains<90){
                mutexMill.lock();
                this->availableRyeTank = true;
                mutexMill.unlock();
            }
        }while(!this->availableRyeTank);
    }
    else{
        do{
        if(this->availableWheatTank){
            mutexMill.lock();
            this->amountOfWheatGrains+=30;
            mutexMill.unlock();
        }
        if(this->amountOfWheatGrains<90){
            mutexMill.lock();
            this->availableWheatTank = true;
            mutexMill.unlock();
        }
        }while(!this->availableWheatTank);
    }
    mutexMill.lock();
    refreshTanks();
    if(this->amountOfWheatGrains >= 90) this->availableWheatTank = false;
    if(this->amountOfRyeGrains >= 90) this->availableRyeTank = false;
    mutexMill.unlock();
    usleep(rand()%4000000);
}

void Mill::makeFlour(){
    while(!endProgram){
        usleep(rand()%500000);
        produceRyeFlour();
        usleep(rand()%500000);
        produceWheatFlour();
    }
}

void Mill::produceRyeFlour(){
    if(this->amountOfRyeGrains >=20){
        mutexMill.lock();
        this->amountOfRyeGrains -= 20;
        refreshTanks();
        mutexMill.unlock();
        mutexConsole.lock();
        mvprintw(1,39,"%s","rye flour");
        mutexConsole.unlock();
        runProcessLoading();
    }
    mutexConsole.lock();
    mvprintw(40,80,"%s","produkcja zytniej");
    mvprintw(41,80,"%d", this->amountOfRyeGrains);
    mutexConsole.unlock();
}

void Mill::produceWheatFlour(){
    if(this->amountOfWheatGrains>=20){
        mutexMill.lock();
        this->amountOfWheatGrains -= 20;
        refreshTanks();
        mutexMill.unlock();
        mutexConsole.lock();
        mvprintw(1,39,"%s","wheat flour");
        mutexConsole.unlock();
        runProcessLoading();
    }
    mutexConsole.lock();
    mvprintw(40,80,"%s","produkcja pszennej");
    mvprintw(41,83,"%d", this->amountOfWheatGrains);
    mutexConsole.unlock();
}

void Mill::runProcessLoading(){
    int tmp_x=0;
    for(int i=0; i<7; i++){
        usleep(rand()%700000 + 500000);
        mutexConsole.lock();
        init_pair(5, COLOR_BLACK, COLOR_RED);
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
        init_pair(6, COLOR_BLACK, COLOR_BLACK);
        attron(COLOR_PAIR(6));
        mvprintw(12 + tmp_y, 25 + tmp_x," ");
        mvprintw(12 + tmp_y, 31 + tmp_x," ");
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