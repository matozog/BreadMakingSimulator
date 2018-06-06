
#include "Field.h"
#include "Farmer.h"
#include <condition_variable>

condition_variable cond_harvestFinised;

extern mutex mutexConsole;
extern mutex mutexFarmers;
extern bool endProgram;
mutex mutexNature;

Field::Field() {

}

Field::Field(int typeOfCorp) {
    this->typeOfCrop = typeOfCorp;
}

int Field::getTypeOfCrop(){
    return typeOfCrop;
}

void Field::setTypeOfCrop(int typeOfCrop) {
    Field::typeOfCrop = typeOfCrop;
}

vector<CoordinateField> Field::getArea() {
    return this->area;
}

void Field::setArea(int x, int y, int type) {
    this->area.push_back({y,x,0,type,true});
}

int Field::harvest(Farmer *farmer){
    int grains = 0;
    {
        unique_lock<mutex> locker_harvest(mutexNature);
        cond_harvestFinised.wait(locker_harvest, [&]{return isReadyToHarvest();});
        for(int i=0; i<this->area.size(); i++){
            if(this->area.at(i).stateOfCrop == 2){
                grains++;
                this->area.at(i).stateOfCrop = 0;
                mutexConsole.lock();
                attron(COLOR_PAIR(4));
                mvprintw(this->area[i].y,this->area[i].x,"%c", ' ');
                attroff(COLOR_PAIR(4));
                mutexConsole.unlock();
                amountOfGrains --;
            }
            if(grains == 5){
                cond_harvestFinised.notify_one();
                return 5;
            }
            usleep(rand()%500000);
        }
    }
}

bool Field::isReadyToHarvest(){
    if(this->amountOfGrains>=5) return true;
    else return false;
}

void Field::natureThread() {

    while(!endProgram){
        usleep(rand()%7000000+4000000);
        refreshCrops();
    }
}

void Field::refreshCrops(){

    amountOfGrains = 0;
    for(int i=0 ;i<this->area.size();i++){

        if(this->area.at(i).stateOfCrop == 0){
            mutexNature.lock();
            this->area[i].stateOfCrop ++;
            mutexNature.unlock();

            mutexConsole.lock();
            attron(COLOR_PAIR(2));
            mvprintw(this->area[i].y,this->area[i].x,"%c", '^');
            attroff(COLOR_PAIR(2));
            mutexConsole.unlock();
        }
        else if( this->area.at(i).stateOfCrop==1) {
            mutexNature.lock();
            this->area[i].stateOfCrop ++;
            mutexNature.unlock();
            
            mutexConsole.lock();
            attron(COLOR_PAIR(3));
            mvprintw(this->area[i].y,this->area[i].x, "%c", '*');
            attroff(COLOR_PAIR(3));
            mutexConsole.unlock();
        }
        if(this->area.at(i).stateOfCrop == 2){
            mutexNature.lock();
            this->amountOfGrains ++;
            mutexNature.unlock();
        }
    }
    cond_harvestFinised.notify_one();
}

bool Field::getAvailable() {
    return isAvailable;
}

void Field::setAvailable(bool available) {
    this->isAvailable = available;
}
