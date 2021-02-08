#ifndef CITY_HH
#define CITY_HH

#include <QTime>
#include <QTimer>
#include <QPixmap>

#include "interfaces/icity.hh"
#include "errors/gameerror.hh"
#include "core/location.hh"
#include "errors/initerror.hh"


namespace StudentSide {
class Player;
class Location;
class City : public QObject, public Interface::ICity
{
  Q_OBJECT

public:
  City();
  virtual ~City();

  //Functions inherited from ICity
  void setBackground(QImage &basicbackground, QImage &bigbackground);
  void setClock(QTime clock);
  void addStop(std::shared_ptr<Interface::IStop> stop);
  void startGame();
  void addActor(std::shared_ptr<Interface::IActor> newactor);
  void removeActor(std::shared_ptr<Interface::IActor> actor);
  void actorRemoved(std::shared_ptr<Interface::IActor> actor);
  bool findActor(std::shared_ptr<Interface::IActor> actor) const;
  void actorMoved(std::shared_ptr<Interface::IActor> actor);
  std::vector<std::shared_ptr<Interface::IActor> > getNearbyActors(Interface::Location loc) const;
  bool isGameOver() const;


  /**
   * @brief Saves the background for the game
   * @pre -
   * @post Exception guarantee: nothrow.
   * @return The background as a Bitmap
   */
  QPixmap getBackGround();

  /**
    * @brief Function for setting isGameOver to true
    * @pre -
    * @post isGameOver is changed to true if not true allready
    * Exception guarantee: Northrow
    */
  void setGameOver();

  /**
   * @brief function that returns stops
   * @pre -
   * @post Citystops vector is given
   * *Expection guarantee: Northrow
   */
  std::vector<std::shared_ptr<Interface::IStop>> getStops() const;

  /**
    * @brief Function for getting nearby stops
    * @pre stops have been made and given location is valid
    * @post nearbystops are given.  If non are nearby, empty vector given
    * Exception guarantee: ?
    */
  std::vector<std::shared_ptr<Interface::IStop>> getNearbyStops(Interface::Location loc);

  /**
   * @brief Function that sets player_ with given player pointer
   * @pre Valid pointer is given
   * @post player_ has been set correctly
   * *Expection guarantee: Northrow
   */
  void setPlayer(std::shared_ptr<Player> player);

  /**
   * @brief function that returns player_
   * @pre -
   * @post player_ is given
   * *Expection guarantee: Northrow
   */
  std::shared_ptr<Player> givePlayer();

  /**
   * @brief function that returns inGameState
   * @pre -
   * @post inGameState is given
   * *Expection guarantee: Northrow
   */
  bool getGameState();

  //Functions created for testing:

  /**
    * @brief Gives cityActors length
    * @pre -
    * @post Length is given
    * Exception guarantee: Northrow
    */
  int giveCityActorsLength();

signals:

  //Signaling that actor given in the signal parameters was removed
  void actorWasRemoved(std::shared_ptr<Interface::IActor> actor);

  //Signaling that actor given in the signal parameters was added
  void actorWasAdded(std::shared_ptr<Interface::IActor> actor);

  // Signaling that the city's clock changed
  void clockChanged(QTime clock);


private:
  std::vector<std::shared_ptr<Interface::IActor>> cityActors_;
  bool inGameState_ = false;
  bool isGameOver_ = false;
  QPixmap cityMap_;
  QTime cityClock_;
  std::vector<std::shared_ptr<Interface::IStop>> cityStops_;
  std::shared_ptr<Player> player_ = nullptr;
};

}
#endif // CITY_HH
