
#include "Field.h"

extern mutex mutexConsole;

Field::Field() {

}

Field::Field(int typeOfCorp) {
    this->typeOfCrop = typeOfCorp;
}

int Field::getTypeOfCrop() const {
    return typeOfCrop;
}

void Field::setTypeOfCrop(int typeOfCrop) {
    Field::typeOfCrop = typeOfCrop;
}

vector<CoordinateField> Field::getArea() const {
    return area;
}

void Field::setArea(int x, int y) {
    this->area.push_back({y,x,0,true});
}

void Field::natureThread() {

    while(true){
        usleep(rand()%2000000+3000000);
        refreshCrops();
    }
}

void Field::refreshCrops(){

    for(int i=0 ;i<area.size();i++){
        if(area.at(i).stateOfCrop == 0){
            area[i].stateOfCrop = 1;
            mutexConsole.lock();
            init_pair(2, COLOR_BLACK, COLOR_MAGENTA);
            attron(COLOR_PAIR(2));
            mvprintw(area[i].y,area[i].x,"%c", '^');
            attroff(COLOR_PAIR(2));
            mutexConsole.unlock();
        }
        else if( area.at(i).stateOfCrop==1) {
            area[i].stateOfCrop = 2;
            mutexConsole.lock();
            init_pair(3, COLOR_BLACK, COLOR_YELLOW);
            attron(COLOR_PAIR(3));
            mvprintw(area[i].y,area[i].x, "%c", '*');
            attroff(COLOR_PAIR(3));
            mutexConsole.unlock();
        }
    }

}

bool Field::getAvailable() {
    return isAvailable;
}

void Field::setAvailable(bool available) {
    this->isAvailable = available;
}
