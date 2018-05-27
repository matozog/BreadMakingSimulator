#include "Road.h"
#include "Farmer.h"

extern mutex mutexConsole;
extern CoordinateField **mapFields;
extern mutex mutexFarmers;

Road::Road(ID_Road ID){
    this->ID = ID;
    mutexConsole.lock();
    mvprintw(40,5,"%d", this->ID);
    refresh();
    mutexConsole.unlock();
    vector <CoordinateField> tmp_road;
    switch(this->ID)
    {
        case HomeToFWheat: {
            tmp_road.push_back({32, 13, 0, true});
            tmp_road.push_back({32, 16, 0, true});
            tmp_road.push_back({14, 16, 0, true});
            tmp_road.push_back({14, 11, 0, true});
            tmp_road.push_back({15, 11, 0, true});
            break;
        }
        case HomeToFRye:{
            tmp_road.push_back({32, 13, 0, true});
            tmp_road.push_back({32, 16, 0, true});
            tmp_road.push_back({22, 16, 0, true});
            tmp_road.push_back({22, 11, 0, true});
            tmp_road.push_back({23, 11, 0, true});
            break;
        }
        case FWheatToMill: {
            tmp_road.push_back({15, 11, 0, true});
            tmp_road.push_back({15, 31, 0, true});
            tmp_road.push_back({16, 31, 0, true});
            break;
        }
        case FRyeToMill:{
            tmp_road.push_back({23, 11, 0, true});
            tmp_road.push_back({23, 16, 0, true});
            tmp_road.push_back({15, 16, 0, true});
            tmp_road.push_back({15, 31, 0, true});
            tmp_road.push_back({16, 31, 0, true});
            break;
        }
        case MillToHome:{
            tmp_road.push_back({16, 31, 0, true});
            tmp_road.push_back({16, 14, 0, true});
            tmp_road.push_back({31, 14, 0, true});
            tmp_road.push_back({31, 13, 0, true});
            break;
        }
   }
   createRoad(tmp_road);
}

void Road::moveFarmerToDestination(Farmer *farmer) {
    for(int j=0; j<road.size()-1; j++){
        if(j>0){
           // mutexFarmers.lock();
            mapFields[road.at(j-1).y][road.at(j-1).x].available = true;
            //mutexFarmers.unlock();
            road.at(j-1).available = true;
        }
        farmer->setPosition(road.at(j).y, road.at(j).x);
        //mutexFarmers.lock();
        mapFields[road.at(j).y][road.at(j).x].available = false;
        //mutexFarmers.unlock();
        road.at(j).available = false;
        refreshRoad();
       // do{
            usleep(500000);
       // }while(!mapFields[road.at(j+1).y][road.at(j+1).x].available);
    }
}

void Road::createRoad(vector<CoordinateField> tmpRoad){
    vector<CoordinateField> roadToXY;
    CoordinateField start, end;
    for(int i=0; i<tmpRoad.size()-1; i++){
        start = tmpRoad.at(i);
        end = tmpRoad.at(i+1);
        if(start.y == end.y){
            if(start.x > end.x){
                while(start.x != end.x){
                    this->road.push_back({start.y, start.x--,0,true});
                }
            }
            else{
                while(start.x != end.x){
                    this->road.push_back({start.y, start.x++,0,true});
                }
            }
        }
        else if(start.x == end.x){
            if(start.y > end.y){
                while(start.y != end.y){
                    this->road.push_back({start.y--, start.x,0,true});
                }
            }
            else{
                while(start.y != end.y){
                    this->road.push_back({start.y++, start.x,0,true});
                }
            }
        }
    }
}

void Road::refreshRoad(){
    for(int i=0; i<road.size();i++){
        if(road.at(i).available){
            mutexConsole.lock();
            mvprintw(road.at(i).y, road.at(i).x, " ");
            mutexConsole.unlock();
        }
    }
}

bool Road::isAvailable() {
    return this->available;
}

ID_Road Road::getID() {
    return this->ID;
}