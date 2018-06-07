#include "Client.h"
#include <ncurses.h>
#include <unistd.h>

extern bool endProgram;
extern mutex mutexConsole, mutexClients;
extern CoordinateField **mapFields;

Road Client::roadHomeToBakery(ClHomeToBakery);
Road Client::roadHomeToShop(ClHomeToShop);
Road Client::roadBakeryToHome(ClBakeryToHome);
Road Client::roadShopToHome(ClShopToHome);


Client::Client() {

}

Client::Client(int x, int y, int ID) {
    this->x_start = x;
    this->y_start = y;
    this->x = x;
    this->y = y;
    this->ID = ID;
    color = rand()%3+1;
    if((rand()%2+0) == 1){
        this->favouriteFlour = "rye";
        this->favouriteBread = "rye";
    }
    else{
        this->favouriteFlour = "wheat";
        this->favouriteBread = "wheat-rye";
    }
    mutexConsole.lock();
    setPosition(y,x);
    mutexConsole.unlock();
}

void Client::simulatingLife(){
    while(!endProgram){
        Resting();

        if ((rand() % 2 + 0) == 1 ) {
            goToBakery();
            usleep(rand()%400000+400000);
            takeBreadFromBakery(this->favouriteBread);
            goFromBakeryToHome();
            usleep(rand()%400000+400000);
        }
        else{
            goToShop();
            usleep(rand()%400000+400000);
            if((rand()%2+0)==1){
                takeBreadFromShop(this->favouriteBread);
            }
            else{
                takeFlourFromShop(this->favouriteFlour);
            }
            goFromShopToHome();
            usleep(rand()%400000+400000);
        }
    }
}

void Client::goFromBakeryToHome(){
    roadBakeryToHome.moveClientToDestination(this);
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mutexConsole.unlock();

    mutexClients.lock();
    mapFields[this->y][this->x].available = true;
    mutexClients.unlock();

    mutexConsole.lock();
    this->setPosition(this->y_start, this->x_start);
    mutexConsole.unlock();
}

void Client::goFromShopToHome(){

    roadShopToHome.moveClientToDestination(this);
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mutexConsole.unlock();

    mutexClients.lock();
    mapFields[this->y][this->x].available = true;
    mutexClients.unlock();

    mutexConsole.lock();
    this->setPosition(this->y_start, this->x_start);
    mutexConsole.unlock();
}

void Client::goToBakery(){
    roadHomeToBakery.moveClientToDestination(this);
}

void Client::goToShop(){
    roadHomeToShop.moveClientToDestination(this);
}

void Client::takeBreadFromBakery(string type){
    if(type == "rye"){

    }
    else{

    }
}

void Client::takeBreadFromShop(string type){
    if(type == "rye"){

    }
    else{

    }
}

void Client::takeFlourFromShop(string type){
    if(type == "rye"){

    }
    else{

    }
}

void Client::setPosition(int y, int x){
    this->x = x;
    this->y = y;
    attron(COLOR_PAIR(color));
    mvprintw(y,x,"%d", this->ID);
    attroff(COLOR_PAIR(color));
}

void Client::Resting(){
    mutexConsole.lock();
    setPosition(y_start, x_start);
    mutexConsole.unlock();
    usleep(rand()%3000000 + 2000000);
}
