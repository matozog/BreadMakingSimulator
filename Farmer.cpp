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
    // remove farmer from farm
    mutexConsole.lock();
    mvprintw(this->y_start, this->x_start, " ");
    mutexConsole.unlock();

    // set farmer state and run function which will move
    this->state = "goFieldWithWheat";
    roadHomeToFieldWithWheat.moveFarmerToDestination(this);

    // set available field with wheat on true
    mutexFarmers.lock();
    fieldWithWheat.setAvailable(true);
    mutexFarmers.unlock();

    // farmer go to mill, where he will sell grain
    sellGrain();
}

void Farmer::goFieldWithRye() {
    // remove farmer from farm
    mutexConsole.lock();
    mvprintw(this->y_start, this->x_start, " ");
    mutexConsole.unlock();

    // set farmer state and run function moveFarmer
    this->state = "goFieldWithRye"; 
    roadHomeToFieldWithRye.moveFarmerToDestination(this);

    // set available field with rye on true
    mutexFarmers.lock();
    fieldWithRye.setAvailable(true);
    mutexFarmers.unlock();

    // farmer go to mill
    sellGrain();
}

void Farmer::sellGrain() {
    // remove farmer from field and go to mill
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mapFields[this->y][this->x].available = true;
    mutexConsole.unlock();

    // choosing field from which the farmer go
    if(this->state == "goFieldWithRye"){
        roadFieldWithRyeToMill.moveFarmerToDestination(this);
    }else{
        roadFieldWithWheatToMill.moveFarmerToDestination(this);
    }
    this->state = "sellGrain";

    // if he sell grain, he will return to home
    goToHome();
}

void Farmer::goToHome() {
    // remove farmer from mill and go to home
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mapFields[this->y][this->x].available = true;
    mutexConsole.unlock();
    mutexFarmers.lock();
    mapFields[this->y][this->x].available = true;
    mutexFarmers.unlock();

    roadFromMillToHome.moveFarmerToDestination(this);
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mapFields[this->y][this->x].available = true;
    mutexConsole.unlock();

    // set position farmer on start position
    this->setPosition(this->y_start, this->x_start);
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
        mutexConsole.unlock();
    }
}

void Farmer::Resting() {
    setPosition(y_start, x_start);
    usleep(rand()%1000000);
}

void Farmer::setPosition(int y, int x) {
    this->x = x;
    this->y = y;
    mutexConsole.lock();
    mvprintw(y,x,"%d", this->ID);
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