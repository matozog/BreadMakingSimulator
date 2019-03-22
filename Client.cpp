#include "Client.h"
#include "Bakery.h"
#include "Shop.h"
#include <ncurses.h>
#include <unistd.h>
#include <condition_variable>

extern bool endProgram;
extern mutex mutexConsole, mutexClients, mutexBreadStore, mutexStoreShop;
extern CoordinateField **mapFields;
extern condition_variable cond_ShopFlourStore, cond_ShopBreadStore;
condition_variable cond_ClBakeryStore;
extern Bakery bakery;
extern Shop shop;

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
        {
            unique_lock<mutex> locker_RyeBreadStore(mutexBreadStore);
            cond_ClBakeryStore.wait(locker_RyeBreadStore, [&]{return bakery.getAvailableRyeBreadStore();});
            bakery.setAvailableRyeBreadStore(false);
            bakery.sellRyeBread(MAX_LOAD);
        }
    }
    else{
        {
            unique_lock<mutex> locker_WheatRyeBreadStore(mutexBreadStore);
            cond_ClBakeryStore.wait(locker_WheatRyeBreadStore, [&]{return bakery.getAvailableWheatRyeBreadStore();});
            bakery.setAvailableWheatRyeBreadStore(false);
            bakery.sellWheatRyeBread(MAX_LOAD);
        }
    }
    usleep(rand()%500000 + 100000);
}

void Client::takeBreadFromShop(string type){
    if(type == "rye"){
        {
            unique_lock<mutex> locker_ShopRyeBread(mutexStoreShop);
            cond_ShopBreadStore.wait(locker_ShopRyeBread, [&]{return shop.getAvailableStoreRBread();});
            shop.setAvailableStoreRBread(false);
            shop.sellRyeBread(MAX_LOAD);
            if(shop.getAmountRyeBread()>=1){
                shop.setAvailableStoreRBread(true);
                cond_ShopBreadStore.notify_one();
            }
        }
    }
    else{
        {
            unique_lock<mutex> locker_ShopWheatBread(mutexStoreShop);
            cond_ShopBreadStore.wait(locker_ShopWheatBread, [&]{return shop.getAvailableStoreWRBread();});
            shop.setAvailableStoreWRBread(false);
            shop.sellWheatRyeBread(MAX_LOAD);
            if(shop.getAmountWheatRyeBread()>=5){
                shop.setAvailableStoreWRBread(true);
                cond_ShopBreadStore.notify_one();
            }
        }
    }
    usleep(rand()%500000 + 100000);
}

void Client::takeFlourFromShop(string type){
    if(type == "rye"){
        {
            unique_lock<mutex> locker_ShopFlourStore(mutexStoreShop);
            cond_ShopFlourStore.wait(locker_ShopFlourStore, [&]{return shop.getAvailableStoreRFlour();});
            shop.setAvailableStoreRFlour(false);
            shop.sellRyeFlour(MAX_LOAD);
            if(shop.getAmountRyeBread()>=1){
                shop.setAvailableStoreRFlour(true);
                cond_ShopFlourStore.notify_one();
            }
        }
    }
    else{
        {
            unique_lock<mutex> locker_ShopFlourStore(mutexStoreShop);
            cond_ShopFlourStore.wait(locker_ShopFlourStore, [&]{return shop.getAvailableStoreWFlour();});
            shop.setAvailableStoreWFlour(false);
            shop.sellWheatFlour(MAX_LOAD);
            if(shop.getAmountWheatFlour()>=5){
                shop.setAvailableStoreWFlour(true);
                cond_ShopFlourStore.notify_one();
            }
        }
    }
    usleep(rand()%500000 + 100000);
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
