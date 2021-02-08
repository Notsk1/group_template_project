#ifndef ENGINE_HH
#define ENGINE_HH

#include "mainwindow.hh"
#include "dialog.hh"
#include "../Course/CourseLib/core/logic.hh"
#include "../Course/CourseLib/creategame.hh"
#include "../Course/CourseLib/errors/initerror.hh"
#include "../Course/CourseLib/interfaces/istop.hh"
#include "../Course/CourseLib/interfaces/ivehicle.hh"
#include "city.hh"
#include "player.hh"
#include "statistics.hh"
#include <fstream>
#include "enemy.hh"

#include <QObject>
#include <iostream>
#include <unordered_map>


namespace StudentSide
{

const QString BASIC_BG_PATH = ":/offlinedata/offlinedata/kartta_pieni_500x500.png";
const QString BIG_BG_PATH = ":/offlinedata/offlinedata/kartta_iso_1095x592.png";

class Engine : public QObject
{
    Q_OBJECT
public:
    // Connects signals and slots, starts up the starting dialog and the main
    // window in its starting state if dialog was accepted.
    Engine();

    // Destructor: deletes the dialog and main window
    ~Engine();

    // Returns if dialog was accepted or not. Called by main function.
    bool wasDialogAccepted() const;

signals:
    // Used to update the displayed time left in the game in main window.
    void timeChanged(unsigned int min, unsigned int sec);
    // Tells how much score the player should be given.
    void scoreAdded(unsigned int score);

private slots:
    // Sets settings for the game based on selections in the starting dialog.
    void setSettings(unsigned int min, unsigned int sec, bool manualTime, QTime gameTime, QString playerName);
    // Calls everything necessary for the game to start.
    void startGame();
    // Decreases remaining game time by 1 second.
    void decreaseTimeLeft();
    // Checks if given actor is a bus. Removes the bus from its own data structure.
    // Also calls the main window to destroy the graphical bus item.
    void removeBus(std::shared_ptr<Interface::IActor> actor);
    // Checks if given actor is a bus, calls the main window to create and draw
    // the bus and adds them to a data strucuture.
    void addBus(std::shared_ptr<Interface::IActor> actor);
    // Updates the positions of player's and buses' graphics
    void updatePositions();
    // Creates and initializes a player actor and a graphical player item.
    void initPlayer();
    // Calls a function to change the player's coordinates if the city is in game state.
    void movePlayer();
    // Checks that city is in game state and calls a function to move the enemy.
    void moveEnemy();
    // Calls a function to go into a bus or stop if the city is in game state.
    void on_E_pressed();
    // Calls a function to leave a bus or stop if the city is in game state.
    void on_Q_pressed();
    // Calculates how much score the player should get and gives it
    void giveScore();

private:
    // Gets stops, saves them in a data structure and calls main window to draw them
    void initStops();
    // Creates the enemy.
    void initEnemy();
    // Checks if the player collides with the enemy and ends the game if it does
    void checkCollision();
    // Stops all timers and tells city that the game is over
    void endGame();

    void setTop10();

    MainWindow* w_;
    Dialog* d_;
    bool dialogAccepted_;
    unsigned int minutes_;
    unsigned int seconds_;
    bool manualTime_;
    QTime manualGameTime_;
    // The number of pixels the player can move in 1 direction per second.
    unsigned int playerSpeed_;
    CourseSide::Logic logic_;
    // Decreases remaining game time every second.
    QTimer timeLeftDecreaser_;
    std::shared_ptr<City> city_;
    std::unordered_map<std::shared_ptr<Interface::IStop>, Actoritem*> stops_;
    std::unordered_map<std::shared_ptr<Interface::IActor>, Actoritem*> buses_;
    std::unordered_map<std::shared_ptr<Player>, Actoritem*> players_;
    std::unordered_map<std::shared_ptr<Enemy>, Actoritem*> enemy_;
    // Timer for updating player's and buses' visible positions on the screen.
    QTimer animationTimer_;
    // Timer for moving player's x-y-coordinates. Interval is defined by playerSpeed_.
    QTimer playerMovementTimer_;
    // Timer for moving the enemy's x-y-coordinates. Interval is double of playerMovementTimer_'s.
    QTimer enemyMovementTimer_;
    // Timer for checking if player fulfills the conditions of getting score.
    QTimer updateScoreTimer_;
    std::shared_ptr<Player> player_;
    std::shared_ptr<Enemy> enemyActor_;
    // Keeps track of score.
    Statistics stats_;
    // A vector that keeps track of which WASD-keys are currently pressed.
    // Index 0 = W, index 1 = A, index 2 = S, index 3 = D.
    std::shared_ptr<std::vector<bool>> wasd_;
    QGraphicsScene* map_;
    QString playerName_;
};

}

#endif // ENGINE_HH
