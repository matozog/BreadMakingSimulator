#include "Farmer.h"

extern mutex mutexConsole;
extern bool endProgram;
extern Field fieldWithWheat;
extern Field fieldWithRye;
extern mutex mutexFarmers;
extern CoordinateField **mapFields;

Road Farmer::roadHomeToFieldWithWheat(HomeToFWheat);
Road Farmer::roadHomeToFieldWithRye(HomeToFRye);
Road Farmer::roadFieldWithWheatToMill(FWheatToMill);
Road Farmer::roadFieldWithRyeToMill(FRyeToMill);
Road Farmer::roadFromMillToHome(MillToHome);

Farmer::Farmer() {

}

Farmer::Farmer(int x, int y, int ID) {
    this->x_start = x;
    this->y_start = y;
    this->x = x;
    this->y = y;
    this->ID = ID;
    setPosition(y,x);
}

void Farmer::goFieldWithWheat() {
    mutexConsole.lock();
    mvprintw(this->y_start, this->x_start, " ");
    //refresh();
    mutexConsole.unlock();
    this->state = "goFieldWithWheat";
    roadHomeToFieldWithWheat.moveFarmerToDestination(this);
    mutexFarmers.lock();
    fieldWithWheat.setAvailable(true);
    mutexFarmers.unlock();
    sellGrain();
}

void Farmer::goFieldWithRye() {
    mutexConsole.lock();
    mvprintw(this->y_start, this->x_start, " ");
    //refresh();
    mutexConsole.unlock();
    this->state = "goFieldWithRye"; 
    roadHomeToFieldWithRye.moveFarmerToDestination(this);
    sellGrain();
    mutexFarmers.lock();
    fieldWithRye.setAvailable(true);
    mutexFarmers.unlock();
}

void Farmer::sellGrain() {
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    //refresh();
    mapFields[this->y][this->x].available = true;
    mutexConsole.unlock();
    if(this->state == "goFieldWithRye"){
        roadFieldWithRyeToMill.moveFarmerToDestination(this);
    }else{
        roadFieldWithWheatToMill.moveFarmerToDestination(this);
    }
    this->state = "sellGrain";

    goToHome();
}

void Farmer::goToHome() {
    mutexFarmers.lock();
    mapFields[this->y][this->x].available = true;
    mutexFarmers.unlock();
    roadFromMillToHome.moveFarmerToDestination(this);
}

void Farmer::simulatingLife() {

    while(!endProgram) {
        Resting();

        mutexFarmers.lock();
        if ((rand() % 2 + 0) == 1 && fieldWithRye.getAvailable()) {
            fieldWithRye.setAvailable(false);
            mutexFarmers.unlock();
            goFieldWithRye();
        } else if (fieldWithWheat.getAvailable()) {
            fieldWithWheat.setAvailable(false);
            mutexFarmers.unlock();
            goFieldWithWheat();
        }
        else mutexFarmers.unlock();
        mutexConsole.lock();
        mvprintw(40,5,"%d", this->ID);
        //refresh();
        mutexConsole.unlock();
    }
}

void Farmer::Resting() {
    setPosition(y_start, x_start);
    usleep(rand()%400000+300000);
}

void Farmer::setPosition(int y, int x) {
    this->x = x;
    this->y = y;
    mutexConsole.lock();
    mvprintw(y,x,"%d", this->ID);
    //refresh();
    mutexConsole.unlock();
}

void Farmer::clearPosition(int y, int x){
    mutexConsole.lock();
    mvprintw(y,x," ");
    mvprintw(y,x+1," ");
    //refresh();
    mutexConsole.unlock();
}

void Farmer::setID(int ID) {
    this->ID = ID;
}

void Farmer::setState(string state) {
    this->state = state;
}

void Farmer::setX(int x) {
    this->x = x;
}

void Farmer::setY(int y) {
    this->y = y;
}

int Farmer::getID() {
    return this->ID;
}

int Farmer::getX() {
    return this->x;
}

int Farmer::getY() {
    return this->y;
}

string Farmer::getState() {
    return this->state;
}