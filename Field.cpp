
#include "Field.h"
#include "Farmer.h"

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
    do{
        grains = 0;
        for(int i=0; i<this->area.size(); i++){
            if(this->area.at(i).stateOfCrop == 2){
                grains++;
                mutexNature.lock();
                this->area.at(i).stateOfCrop = 0;
                mutexNature.unlock();
                mutexConsole.lock();
                init_pair(4, COLOR_BLACK, COLOR_BLACK);
                attron(COLOR_PAIR(4));
                mvprintw(this->area[i].y,this->area[i].x,"%c", ' ');
                attroff(COLOR_PAIR(4));
                mutexConsole.unlock();
            }
            if(grains == 5){
                return 5;
            }
            usleep(rand()%500000);
        }
    }while(grains<5);
}

void Field::natureThread() {

    while(!endProgram){
        usleep(rand()%6000000+4000000);
        mutexFarmers.lock();
        refreshCrops();
        mutexFarmers.unlock();
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
            init_pair(2, COLOR_BLACK, COLOR_MAGENTA);
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
            init_pair(3, COLOR_BLACK, COLOR_YELLOW);
            attron(COLOR_PAIR(3));
            mvprintw(this->area[i].y,this->area[i].x, "%c", '*');
            attroff(COLOR_PAIR(3));
            mutexConsole.unlock();
        }
        if(this->area.at(i).stateOfCrop == 2){
            this->amountOfGrains ++;
        }
    }
}

bool Field::getAvailable() {
    return isAvailable;
}

void Field::setAvailable(bool available) {
    this->isAvailable = available;
}
