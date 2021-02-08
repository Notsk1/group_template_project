#include "city.hh"

// Bad practice but the linker can't find these...
#include "core/location.cc"
#include "errors/gameerror.cc"
#include "errors/initerror.cc"

namespace StudentSide{

City::City()
{

}

City::~City()
{

}

void City::setBackground(QImage &basicbackground, QImage &bigbackground)
{
  try {
    cityMap_ = QPixmap::fromImage(basicbackground);
  } catch (...) {
    throw Interface::InitError("Setting the picture was unsuccesful or the picture was invalid.");
  }
  Q_UNUSED(bigbackground);
}

void City::setClock(QTime clock)
{
  cityClock_ = clock;
  emit clockChanged(clock);
}

void City::addStop(std::shared_ptr<Interface::IStop> stop)
{
  try {
    cityStops_.push_back(stop);
  } catch (...) {
    throw Interface::InitError("Stops position is not valid.");
  }
}

void City::startGame()
{
  inGameState_ = true;
}

void City::addActor(std::shared_ptr<Interface::IActor> newactor)
{
  if(findActor(newactor)){
    throw Interface::GameError ("Actor is already in the city.");
  }
  else{
    emit actorWasAdded(newactor);
    cityActors_.push_back(newactor);
  }
}

void City::removeActor(std::shared_ptr<Interface::IActor> actor)
{
  if(!findActor(actor)){
    throw Interface::GameError ("Actor not found in the city.");
  }
  cityActors_.erase(std::remove(cityActors_.begin(), cityActors_.end(),actor), cityActors_.end());
  actorRemoved(actor);
}

void City::actorRemoved(std::shared_ptr<Interface::IActor> actor)
{
  emit actorWasRemoved(actor);
}

bool City::findActor(std::shared_ptr<Interface::IActor> actor) const
{
  //Count() works cause number of unique actor is allways 0 or 1
  return std::count(cityActors_.begin(), cityActors_.end(),actor);
}

void City::actorMoved(std::shared_ptr<Interface::IActor> actor)
{
  // Not used in our game
  Q_UNUSED(actor);
}

std::vector<std::shared_ptr<Interface::IActor> > City::getNearbyActors(Interface::Location loc) const
{
  std::vector<std::shared_ptr<Interface::IActor>> nearbyActors;
  for(std::shared_ptr<Interface::IActor> actor:cityActors_){
    Interface::Location loca = actor->giveLocation();
    if(loc.isClose(loca)){
      nearbyActors.push_back(actor);
    }

  }
  return nearbyActors;
}

bool City::isGameOver() const
{
  return isGameOver_;
}

std::vector<std::shared_ptr<Interface::IStop> > City::getStops() const
{
  return cityStops_;
}

std::vector<std::shared_ptr<Interface::IStop> > City::getNearbyStops(Interface::Location loc)
{
  std::vector<std::shared_ptr<Interface::IStop>> nearbyStops;
  for(std::shared_ptr<Interface::IStop> stop:cityStops_){
    Interface::Location loca = stop->getLocation();
    if(loc.isClose(loca,30)){
      nearbyStops.push_back(stop);
    }

  }
  return nearbyStops;
}

void City::setPlayer(std::shared_ptr<Player> player)
{
  player_ = player;
}

std::shared_ptr<Player> City::givePlayer()
{
  return player_;
}

bool City::getGameState()
{
  return inGameState_;
}

int City::giveCityActorsLength()
{
  return cityActors_.size();
}

QPixmap City::getBackGround()
{
  return cityMap_;
}

void City::setGameOver()
{
  isGameOver_ = true;
}
}
