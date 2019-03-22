#include "Road.h"
#include "Farmer.h"
#include "Truck.h"
#include "Client.h"

extern mutex mutexConsole;
extern CoordinateField **mapFields;
extern mutex mutexFarmers, mutexClients;
extern int widthMap, heightMap;
condition_variable cond_moveFarmers, cond_moveClients;

Road::Road(ID_Road ID){
    this->ID = ID;
    mutexConsole.lock();
    mvprintw(40,5,"%d", this->ID);
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
        case BakeryToMillGate:{
            tmp_road.push_back({5,78,true});
            tmp_road.push_back({5,59,true});
            tmp_road.push_back({7,59,true});
            break;
        }
        case MillToBakery:{
            tmp_road.push_back({10,56,true});
            tmp_road.push_back({10,61,true});
            tmp_road.push_back({6,61,true});
            tmp_road.push_back({6,78,true});
            tmp_road.push_back({5,78,true});
            break;
        }
        case ShopToMillGate:{
            tmp_road.push_back({22,65,true});
            tmp_road.push_back({20,65,true});
            tmp_road.push_back({20,59,true});
            tmp_road.push_back({13,59,true});
            break;
        }
        case MillToShop:{
            tmp_road.push_back({10,56,true});
            tmp_road.push_back({10,58,true});
            tmp_road.push_back({21,58,true});
            tmp_road.push_back({21,65,true});
            tmp_road.push_back({22,65,true});
            break;
        }
        case ShopToBakery:{
            tmp_road.push_back({22,65,true});
            tmp_road.push_back({21,65,true});
            tmp_road.push_back({21,72,true});
            tmp_road.push_back({15,72,true});
            tmp_road.push_back({15,79,true});
            tmp_road.push_back({9,79,true});
            break;
        }
        case BakeryToShop:{
            tmp_road.push_back({9,79,true});
            tmp_road.push_back({9,77,true});
            tmp_road.push_back({15,77,true});
            tmp_road.push_back({15,70,true});
            tmp_road.push_back({20,70,true});
            tmp_road.push_back({20,65,true});
            tmp_road.push_back({22,65,true});
            break;
        }
        case ShopTruckFromGateToMill:{
            tmp_road.push_back({14,59,true});
            tmp_road.push_back({10,59,true});
            tmp_road.push_back({10,55,true});
            break;
        }
        case BakeryTruckFromGateToMill:{
            tmp_road.push_back({6,59,true});
            tmp_road.push_back({10,59,true});
            tmp_road.push_back({10,55,true});
            break;
        }
        case ClBakeryToHome:{
            tmp_road.push_back({8,109,true});
            tmp_road.push_back({8,107,true});
            tmp_road.push_back({15,107,true});
            break;
        }
        case ClHomeToBakery:{
            tmp_road.push_back({15,110,true});
            tmp_road.push_back({8,110,true});
            tmp_road.push_back({8,110,true});
            tmp_road.push_back({8,109,true});
            break;
        }
        case ClHomeToShop:{
            tmp_road.push_back({18,99,true});
            tmp_road.push_back({18,86,true});
            tmp_road.push_back({28,86,true});
            tmp_road.push_back({28,64,true});
            break;
        }
        case ClShopToHome:{
            tmp_road.push_back({28,64,true});
            tmp_road.push_back({29,64,true});
            tmp_road.push_back({29,88,true});
            tmp_road.push_back({19,88,true});
            tmp_road.push_back({19,99,true});
            break;
        }
   }
   createRoad(tmp_road);
}

void Road::moveClientToDestination(Client *client){
    for(int j=0; j<road.size()-1; j++){
        usleep(rand()%100000+200000);
         {
            unique_lock<mutex> locker(mutexClients);
            cond_moveClients.wait(locker, [&]{return availableNextField(road.at(j));});
            mapFields[road.at(j).y][road.at(j).x].available = false;
            road.at(j).available = false;

            mapFields[client->getY()][client->getX()].available = true;
            cond_moveClients.notify_one();
            if(j>0){
                road.at(j-1).available = true;
            }
            mutexConsole.lock();
            mvprintw(client->getY(), client->getX(), " ");
            client->setPosition(road.at(j).y, road.at(j).x);
            mutexConsole.unlock();
        }
    }
    mutexClients.lock();
    mapFields[road.back().y][road.back().x].available = true;
    mutexClients.unlock();
    road.back().available = true;
}

void Road::moveTruckToDestination(Truck *truck){
    for(int j=0; j<road.size()-1; j++){
        if(mapFields[road.at(j).y][road.at(j).x].available == true){
            mutexConsole.lock();
            mvprintw(truck->getY(), truck->getX(), " ");
            truck->setPosition(road.at(j).y, road.at(j).x);
            mutexConsole.unlock();
        }
        usleep(100000);
    }
}

void Road::moveFarmerToDestination(Farmer *farmer) {

    for(int j=0; j<road.size()-1; j++){
        usleep(rand()%100000+200000);
         {
            unique_lock<mutex> locker(mutexFarmers);
            cond_moveFarmers.wait(locker, [&]{return availableNextField(road.at(j));});
            mapFields[road.at(j).y][road.at(j).x].available = false;
            road.at(j).available = false;
            
            mapFields[farmer->getY()][farmer->getX()].available = true;
            cond_moveFarmers.notify_one();
            if(j>0){
                road.at(j-1).available = true;
            }
            mutexConsole.lock();
            mvprintw(farmer->getY(), farmer->getX(), " ");
            farmer->setPosition(road.at(j).y, road.at(j).x);
            mutexConsole.unlock();
        }
        
    }
    mutexFarmers.lock();
    mapFields[road.back().y][road.back().x].available = true;
    mutexFarmers.unlock();
    road.back().available = true;
}

bool Road::availableNextField(Coordinate nextField){
    return mapFields[nextField.y][nextField.x].available;
}

void Road::createRoad(vector<Coordinate> tmpRoad){
    vector<CoordinateField> roadToXY;
    Coordinate start, end;
    this->road.push_back({tmpRoad.at(0).y, tmpRoad.at(0).x,true});
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

bool Road::isAvailable() {
    return this->available;
}

ID_Road Road::getID() {
    return this->ID;
}
