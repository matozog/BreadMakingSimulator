//
// Created by mateusz on 11.05.18.
//

#include "Farmer.h"


Farmer::Farmer() {

}

Farmer::Farmer(int x, int y, int ID) {
    this->x_start = x;
    this->y_start = y;
    this->x = x;
    this->y = y;
    this->ID = ID;
    setPosition(x,y);
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

void Farmer::goFieldWithWheat() {

}

void Farmer::goFieldWithRye() {

}

void Farmer::sellGrain() {

}

void Farmer::goToHome() {

}

void Farmer::simulatingLife() {

    while(true) {
        Resting();

        if ((rand() % 1 + 1) == 1) {
            goFieldWithRye();
        } else {
            goFieldWithWheat();
        }

        sellGrain();
        goToHome();
    }
}

void Farmer::Resting() {
    setPosition(x_start, y_start);
    usleep(rand()%400000+300000);
}

void Farmer::setPosition(int x, int y) {
    mutexConsole.lock();
    mvprintw(y,x,"%c%d", 'F', this->ID);
    mutexConsole.unlock();
}