#ifndef PLAYER_HH
#define PLAYER_HH

#include <QKeyEvent>
#include <QGraphicsScene>

#include "actors/nysse.hh"
#include "core/location.hh"
#include "city.hh"
#include "actoritem.hh"


/**
  * @file
  * @brief Defines a player that can move freely and still be in a bus or stop
  */



namespace StudentSide {
const int STARTINGX = 250;
const int STARTINGY = 250;

/**
 * @brief Player is a class that is used to create the playable character for the game
 */
class Player : public Interface::IActor
{

public:

  Player(std::shared_ptr<City> city);

  ~Player();

  Interface::Location giveLocation() const;
  void move(Interface::Location loc);
  bool isRemoved() const;
  void remove();

  /**
   * @brief function that handles e button click
   * @pre -
   * @post Player does wanted action (get on stop or bus)
   * *Expection guarantee: ?
   */
  void onButtonClicked();

  /**
   * @brief function that returns currentBus
   * @pre -
   * @post currentbus is given
   * *Expection guarantee: ?
   */
  std::shared_ptr<Interface::IVehicle> getBus();

  /**
   * @brief function that makes Player leave current bus
   * @pre player is in a bus
   * @post player is either left on a stop or left on the edges of the screen
   * *Expection guarantee: ?
   */
  void leaveBus(bool outOfBounds);

  /**
   * @brief function that handles q button click
   * @pre -
   * @post Player does wanted action (get off from stop or bus)
   * *Expection guarantee: ?
   */
  void onOtherButtonClicked();

  /**
   * @brief function that adds passangers to metPassangers
   * @pre passanger is valid pointer
   * @post passanger is added to the vector
   * *Expection guarantee: ?
   */
  void addMetPassenger(std::shared_ptr<Interface::IPassenger> passenger);

  /**
   * @brief function that checks if given passanger is allready known
   * @pre passanger is valid pointer
   * @post return true if passanger is allready saved on Players metPassanger vector
   * *Expection guarantee: ?
   */
  bool isNewPassenger(std::shared_ptr<Interface::IPassenger> passenger);

public slots:

  /**
   * @brief slot function that handles moving the player location
   * @pre -
   * @post players position is changed if needed
   * *Expection guarantee: ?
   */
  void onMovementClicked(QGraphicsScene *gameArea, std::shared_ptr<std::vector<bool>> wasd);






private:

  Interface::Location loc_;
  /**
   * @brief function that checks if given direction is still in bounds
   * @pre -
   * @post true is given if movement is possible
   * *Expection guarantee: ?
   */
  bool checkMovement(char direction, QGraphicsScene* gameArea);
  //Find the nearest stop
  std::shared_ptr<Interface::IStop> findNearestStop(Interface::Location loc);
  //Find the nearest bus
  std::shared_ptr<Interface::IVehicle> findNearestBus();
  /**
   * @brief function that makes player enter stop
   * @pre valid stop pointer is given
   * @post Player_ has currentStop updated
   * *Expection guarantee: ?
   */
  void enterStop(std::shared_ptr<Interface::IStop> stop);
  /**
   * @brief function that makes player enter bus
   * @pre valid bus pointer is given
   * @post Player_ has currentBus updated
   * *Expection guarantee: ?
   */
  void enterBus(std::shared_ptr<Interface::IVehicle> bus);
  //Current bus
  std::shared_ptr<Interface::IVehicle> currentBus_ = nullptr;
  //Current stop
  std::shared_ptr<Interface::IStop> currentStop_ = nullptr;
  std::shared_ptr<City> city_ = nullptr;
  std::vector<std::shared_ptr<Interface::IPassenger>> metPassengers_;
  bool isRemoved_;



};
}

#endif // PLAYER_HH
