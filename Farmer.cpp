#include "Farmer.h"

extern mutex mutexConsole;
extern bool endProgram;
extern Field fieldWithWheat;
extern Field fieldWithRye;
extern mutex mutexFarmers;
extern CoordinateField **mapFields;
extern Mill mill;
condition_variable cond_available_field;

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
    color = rand()%3+1;
    mutexConsole.lock();
    setPosition(y,x);
    mutexConsole.unlock();
}

void Farmer::goFieldWithWheat() {
    // remove farmer from farm
    mutexConsole.lock();
    mvprintw(this->y_start, this->x_start, " ");
    mutexConsole.unlock();

    // set farmer state and run function which will move
    this->state = "goFieldWithWheat";
    roadHomeToFieldWithWheat.moveFarmerToDestination(this);

    collectCrops();

    // set available field with wheat on true
    mutexFarmers.lock();
    fieldWithWheat.setAvailable(true);
    mutexFarmers.unlock();

    // farmer go to mill, where he will sell grain
    sellGrain();
}

void Farmer::goFieldWithRye() {

    // set farmer state and run function moveFarmer
    this->state = "goFieldWithRye"; 
    roadHomeToFieldWithRye.moveFarmerToDestination(this);

    collectCrops();

    // set available field with rye on true
    mutexFarmers.lock();
    fieldWithRye.setAvailable(true);
    mutexFarmers.unlock();

    // farmer go to mill
    sellGrain();
}

void Farmer::sellGrain() {
    // remove farmer from field and go to mill
    mutexFarmers.lock();
    mapFields[this->y][this->x].available = true;
    mutexFarmers.unlock();

    // choosing field from which the farmer go
    if(this->state == "goFieldWithRye"){
        roadFieldWithRyeToMill.moveFarmerToDestination(this);
    }else{
        roadFieldWithWheatToMill.moveFarmerToDestination(this);
    }

    // mill taking grains from farmer
    mill.takeGrainsFromFarmer(this->state);
    this->loadTrailer = 0;

    this->state = "sellGrain";

    // if he sell grain, he will return to home
    goToHome();
}

void Farmer::goToHome() {
    // remove farmer from mill and go to home
    mutexFarmers.lock();
    mapFields[this->y][this->x].available = true;
    mutexFarmers.unlock();

    roadFromMillToHome.moveFarmerToDestination(this);
    
    mutexConsole.lock();
    mvprintw(this->y,this->x," ");
    mutexConsole.unlock();

    mutexFarmers.lock();
    mapFields[this->y][this->x].available = true;
    mutexFarmers.unlock();

    // set position farmer on start position
    mutexConsole.lock();
    this->setPosition(this->y_start, this->x_start);
    mutexConsole.unlock();
    this->state = "rest";
}

void Farmer::collectCrops(){
    if(this->state == "goFieldWithRye"){
        this->loadTrailer = fieldWithRye.harvest(this);
    }else{
        this->loadTrailer = fieldWithWheat.harvest(this);
    }
}

void Farmer::simulatingLife() {
    condition_variable cond_startPosition;
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
    }
}

bool Farmer::startPositionIsAvailable(){
    return mapFields[roadHomeToFieldWithWheat.getRoad().at(0).y][roadHomeToFieldWithWheat.getRoad().at(0).x].available;
}

bool Farmer::isAvailableField(){
    return (fieldWithRye.getAvailable() || fieldWithWheat.getAvailable());
}

void Farmer::Resting() {
    mutexConsole.lock();
    setPosition(y_start, x_start);
    mutexConsole.unlock();
    usleep(rand()%2000000 + 2000000);
}

void Farmer::setPosition(int y, int x) {
    this->x = x;
    this->y = y;
    attron(COLOR_PAIR(color));
    mvprintw(y,x,"%d", this->ID);
    attroff(COLOR_PAIR(color));
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
