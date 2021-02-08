#include "player.hh"
#include "QDebug"

namespace StudentSide {


Player::Player(std::shared_ptr<City> city): city_(city), isRemoved_(false)
{
  loc_ = Interface::Location();
  loc_.setXY(STARTINGX,STARTINGY);
}

Player::~Player()
{

}

Interface::Location Player::giveLocation() const
{
  return loc_;
}

void Player::move(Interface::Location loc)
{
  loc_ = loc;
}

bool Player::isRemoved() const
{
  return isRemoved_;
}

void Player::remove()
{
  isRemoved_ = true;
}

std::shared_ptr<Interface::IStop> Player::findNearestStop(Interface::Location loc)
{
  std::vector<std::shared_ptr<Interface::IStop>> stops;
  for(std::shared_ptr<Interface::IStop> stop : city_->getNearbyStops(loc)){
    stops.push_back(stop);
  }

  if(stops.size() != 0){
    std::shared_ptr<Interface::IStop> nearestStop = stops.at(0);
    for(std::shared_ptr<Interface::IStop> stop : stops){
      int nearestDiff = (abs(nearestStop->getLocation().giveX()-loc.giveX()))
          +(abs(nearestStop->getLocation().giveY()-loc.giveY()));
      int stopDiff = (abs(stop->getLocation().giveX()-loc.giveX()))
          +(abs(stop->getLocation().giveY()-loc.giveY()));
      if(nearestDiff > stopDiff){
        nearestStop = stop;
      }
    }
    if(loc.isClose(nearestStop->getLocation())){
      return nearestStop;
    }
  }
  return nullptr;
}

std::shared_ptr<Interface::IVehicle> Player::findNearestBus()
{
  std::vector<std::shared_ptr<Interface::IVehicle>> busses;
  for(std::shared_ptr<Interface::IActor> actor : city_->getNearbyActors(loc_)){
    //std::shared_ptr<Interface::IVehicle> bus = std::dynamic_pointer_cast<Interface::IVehicle>(actor);
    if(std::shared_ptr<Interface::IVehicle> bus = std::dynamic_pointer_cast<Interface::IVehicle>(actor)){
       busses.push_back(bus);
    }
  }

  if(busses.size() != 0){
    std::shared_ptr<Interface::IVehicle> nearestBus = busses.at(0);
    for(std::shared_ptr<Interface::IVehicle> bus : busses){
      int nearestDiff = (abs(nearestBus->giveLocation().giveX()-loc_.giveX()))
          +(abs(nearestBus->giveLocation().giveY()-loc_.giveY()));
      int busDiff = (abs(bus->giveLocation().giveX()-loc_.giveX()))
          +(abs(bus->giveLocation().giveY()-loc_.giveY()));
      if(nearestDiff > busDiff){
        nearestBus = bus;
      }
    }
    if(loc_.isClose(nearestBus->giveLocation(),10)){
      return nearestBus;
    }
  }
  return nullptr;
}

void Player::onButtonClicked()
{
  if(currentStop_ == nullptr and currentBus_ == nullptr){
    std::shared_ptr<Interface::IStop> stop = findNearestStop(loc_);
    if(stop != nullptr){
      enterStop(stop);
    }

  }
  else if(currentStop_ != nullptr){

    std::shared_ptr<Interface::IVehicle> bus =  findNearestBus();
    if(bus != nullptr){
      enterBus(bus);
    }

  }
}

std::shared_ptr<Interface::IVehicle> Player::getBus()
{
  return currentBus_;
}

void Player::leaveBus(bool outOfBounds = false)
{
  Interface::Location loc = currentBus_->giveLocation();
  std::shared_ptr<Interface::IStop> stop = findNearestStop(loc);
  if(stop != nullptr or outOfBounds == true){

    if(!outOfBounds){
      enterStop(stop);
    }
    else{
      this->move(currentBus_->giveLocation());
      if(currentBus_->giveLocation().giveX()<0){
        loc_.setXY(loc_.giveX()+6,loc.giveY());
      }
      else if(currentBus_->giveLocation().giveX()>500){
        loc_.setXY(loc_.giveX()-6,loc.giveY());
      }
      else if(currentBus_->giveLocation().giveY()<0){
        loc_.setXY(loc_.giveX(),loc.giveY()+7);
      }
      else{
        loc_.setXY(loc_.giveX(),loc.giveY()-7);
      }
    }
    currentBus_ = nullptr;
  }

}

void Player::enterStop(std::shared_ptr<Interface::IStop> stop)
{
  currentStop_ = stop;
  Interface::Location stopLocation = stop->getLocation();
  loc_ = stopLocation;
}

void Player::enterBus(std::shared_ptr<Interface::IVehicle> bus)
{
  currentBus_ = bus;
  currentStop_ = nullptr;
  Interface::Location busLocation = bus->giveLocation();
  loc_ = busLocation;
}


void Player::onOtherButtonClicked()
{
  if(currentStop_ != nullptr){
    currentStop_ = nullptr;
  }
  else if(currentStop_ == nullptr and currentBus_ != nullptr){
    leaveBus();
  }
}

void Player::addMetPassenger(std::shared_ptr<Interface::IPassenger> passenger)
{
    metPassengers_.push_back(passenger);
}

bool Player::isNewPassenger(std::shared_ptr<Interface::IPassenger> passenger)
{
    return std::find(metPassengers_.begin(), metPassengers_.end(), passenger) == metPassengers_.end();
}


void Player::onMovementClicked(QGraphicsScene* gameArea, std::shared_ptr<std::vector<bool>> wasd)
{
  if(currentStop_ == nullptr and currentBus_ == nullptr){
      if (wasd->at(0) && wasd->at(1)) {
          if(checkMovement('w',gameArea) && checkMovement('a', gameArea)){
            loc_.setXY((loc_.giveX()-1),loc_.giveY()+1);
          }
      } else if (wasd->at(0) && wasd->at(3)) {
          if(checkMovement('w',gameArea) && checkMovement('d', gameArea)){
            loc_.setXY((loc_.giveX()+1),loc_.giveY()+1);
          }
      } else if (wasd->at(2) && wasd->at(1)) {
          if(checkMovement('s',gameArea) && checkMovement('a', gameArea)){
            loc_.setXY((loc_.giveX()-1),loc_.giveY()-1);
          }
      } else if (wasd->at(2) && wasd->at(3)) {
          if(checkMovement('s',gameArea) && checkMovement('d', gameArea)){
            loc_.setXY((loc_.giveX()+1),loc_.giveY()-1);
          }
      } else if (wasd->at(0)) {
          if(checkMovement('w',gameArea)){
            loc_.setXY((loc_.giveX()),loc_.giveY()+1);
          }
      } else if (wasd->at(1)) {
          if(checkMovement('a',gameArea)){
            loc_.setXY((loc_.giveX()-1),loc_.giveY());
          }
      } else if (wasd->at(2)) {
          if(checkMovement('s',gameArea)){
            loc_.setXY((loc_.giveX()),loc_.giveY()-1);
          }
      } else if (wasd->at(3)) {
          if(checkMovement('d',gameArea)){
            loc_.setXY((loc_.giveX()+1),loc_.giveY());
          }
      }
  }

  return;
}

bool Player::checkMovement(char direction, QGraphicsScene* gameArea)
{
  QRectF rect = gameArea->sceneRect();
  // The 2 for d and -2 for s are so that the player won't go 1px inside the border.
  std::map<char, std::vector<int>> xDirection = {{'w', {0, 0}}, {'a', {-1, -SIZE.at(PLAYER).width/2}},
                                                 {'s', {0, 0}}, {'d', {2, SIZE.at(PLAYER).width/2}}};
  std::map<char, std::vector<int>> yDirection = {{'w', {1, SIZE.at(PLAYER).height/2}}, {'a', {0, 0}},
                                                 {'s', {-2, -SIZE.at(PLAYER).height/2}}, {'d', {0, 0}}};

  return rect.contains(loc_.giveX() + xDirection.at(direction).at(0)
                                    + xDirection.at(direction).at(1),
                       loc_.giveY() + yDirection.at(direction).at(0)
                                    + yDirection.at(direction).at(1));
}
}
