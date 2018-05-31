#include "Road.h"
#include "Farmer.h"

extern mutex mutexConsole;
extern CoordinateField **mapFields;
extern mutex mutexFarmers;
extern int widthMap, heightMap;

Road::Road(ID_Road ID){
    this->ID = ID;
    mutexConsole.lock();
    mvprintw(40,5,"%d", this->ID);
    //refresh();
    mutexConsole.unlock();
    vector <Coordinate> tmp_road;
    switch(this->ID)
    {
        case HomeToFWheat: {
            tmp_road.push_back({32, 13, true});
            tmp_road.push_back({32, 16, true});
            tmp_road.push_back({14, 16, true});
            tmp_road.push_back({14, 11, true});
            tmp_road.push_back({15, 11, true});
            break;
        }
        case HomeToFRye:{
            tmp_road.push_back({32, 13, true});
            tmp_road.push_back({32, 16, true});
            tmp_road.push_back({22, 16, true});
            tmp_road.push_back({22, 11, true});
            tmp_road.push_back({23, 11, true});
            break;
        }
        case FWheatToMill: {
            tmp_road.push_back({15, 11, true});
            tmp_road.push_back({15, 31, true});
            tmp_road.push_back({16, 31, true});
            break;
        }
        case FRyeToMill:{
            tmp_road.push_back({23, 11, true});
            tmp_road.push_back({23, 16, true});
            tmp_road.push_back({15, 16, true});
            tmp_road.push_back({15, 31, true});
            tmp_road.push_back({16, 31, true});
            break;
        }
        case MillToHome:{
            tmp_road.push_back({16, 31, true});
            tmp_road.push_back({16, 14, true});
            tmp_road.push_back({31, 14, true});
            tmp_road.push_back({31, 13, true});
            break;
        }
   }
   createRoad(tmp_road);
}

void Road::moveFarmerToDestination(Farmer *farmer) {
    for(int j=0; j<road.size()-1; j++){
        if(j>0){
            mutexFarmers.lock();
            mapFields[road.at(j-1).y][road.at(j-1).x].available = true;
            mutexFarmers.unlock();
            road.at(j-1).available = true;
        }
        do{
            if(mapFields[road.at(j).y][road.at(j).x].available == true){
                if(j>0){
                    mutexConsole.lock();
                    mvprintw(road.at(j-1).y, road.at(j-1).x, " ");
                    mutexConsole.unlock();
                }
                farmer->setPosition(road.at(j).y, road.at(j).x);
            }
        }while(mapFields[road.at(j).y][road.at(j).x].available == false);
        mutexFarmers.lock();
        mapFields[road.at(j).y][road.at(j).x].available = false;
        mutexFarmers.unlock();
        road.at(j).available = false;
        do{
           usleep(500000);
        }while(!mapFields[road.at(j+1).y][road.at(j+1).x].available);
    }
    mutexFarmers.lock();
    mapFields[road.back().y][road.back().x].available = true;
    mutexFarmers.unlock();
    road.back().available = true;
    mutexConsole.lock();
    mvprintw(road.back().y,road.back().x ," ");
    mutexConsole.unlock();
}

void Road::createRoad(vector<Coordinate> tmpRoad){
    vector<CoordinateField> roadToXY;
    Coordinate start, end;
    for(int i=0; i<tmpRoad.size()-1; i++){
        start = tmpRoad.at(i);
        end = tmpRoad.at(i+1);
        if(start.y == end.y){
            if(start.x > end.x){
                while(start.x != end.x){
                    start.x--;
                    this->road.push_back({start.y, start.x,true});
                }
            }
            else{
                while(start.x != end.x){
                    start.x++;
                    this->road.push_back({start.y, start.x,true});
                }
            }
        }
        else if(start.x == end.x){
            if(start.y > end.y){
                while(start.y != end.y){
                    start.y--;
                    this->road.push_back({start.y, start.x,true});
                }
            }
            else{
                while(start.y != end.y){
                    start.y++;
                    this->road.push_back({start.y, start.x,true});
                }
            }
        }
    }
}

/*void Road::refreshRoad(){
    /*for(int i=0; i<road.size();i++){
        if(road.at(i).available){
            mutexConsole.lock();
            mvprintw(road.at(i).y, road.at(i).x, " ");
            //refresh();
            mutexConsole.unlock();
        }
    }
    mutexFarmers.lock();
    for(int i=0; i<heightMap; i++){
        for(int j=0; i<widthMap; j++){
            if(mapFields[i][j].type == 7){
                if(mapFields[i][j].available){
                mutexConsole.lock();
                mvprintw(mapFields[i][j].y, mapFields[i][j].x, " ");
                //refresh();
                mutexConsole.unlock();
                }
            }
        }
    }
    mutexFarmers.unlock();
}*/

bool Road::isAvailable() {
    return this->available;
}

ID_Road Road::getID() {
    return this->ID;
}