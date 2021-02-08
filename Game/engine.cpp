#include "engine.hh"
#include <QDebug>

namespace StudentSide
{

Engine::Engine() :
    w_(new MainWindow()),
    d_(new Dialog(w_)),
    dialogAccepted_(true),
    minutes_(3),  // Default settings
    seconds_(0),
    manualTime_(false),
    playerSpeed_(30),
    city_(nullptr)
{
    connect(w_, &MainWindow::gameStarted, this, &Engine::startGame);
    connect(d_, &Dialog::settingsSelected, this, &Engine::setSettings);
    connect(&timeLeftDecreaser_, &QTimer::timeout, this, &Engine::decreaseTimeLeft);
    connect(&playerMovementTimer_, &QTimer::timeout, this, &Engine::movePlayer);
    connect(w_, &MainWindow::E_pressed, this, &Engine::on_E_pressed);
    connect(w_, &MainWindow::Q_pressed, this, &Engine::on_Q_pressed);
    connect(this, &Engine::timeChanged, w_, &MainWindow::changeTimeLeft);
    connect(&animationTimer_, &QTimer::timeout, this, &Engine::updatePositions);
    connect(this, &Engine::scoreAdded, &stats_, &Statistics::addScore);
    connect(&stats_, &Statistics::changeDisplayedScore, w_, &MainWindow::changeScore);
    connect(&updateScoreTimer_, &QTimer::timeout, this, &Engine::giveScore);
    connect(&enemyMovementTimer_, &QTimer::timeout, this, &Engine::moveEnemy);

    if (d_->exec() == QDialog::Accepted) {
        timeLeftDecreaser_.setInterval(1000);
        animationTimer_.setInterval(1000/30);
        playerMovementTimer_.setInterval(1000/playerSpeed_);
        enemyMovementTimer_.setInterval(3*1000/playerSpeed_);
        updateScoreTimer_.setInterval(100);
        if (manualTime_){
            logic_.configChanged(manualGameTime_, false);
        } else {
            logic_.fileConfig();
        }
        wasd_ = w_->giveWasd();
        map_ = w_->giveMap();
        city_ = std::dynamic_pointer_cast<City>(Interface::createGame());
        connect(city_.get(), &City::clockChanged, w_, &MainWindow::updateClock);
        connect(city_.get(), &City::actorWasAdded, this, &Engine::addBus);
        connect(city_.get(), &City::actorWasRemoved, this, &Engine::removeBus);
        try {
            if (!logic_.takeCity(city_)){
                throw Interface::InitError("Error in giving the city to logic_");
            }

            QImage basicBg;
            basicBg.load(BASIC_BG_PATH);
            QImage bigBg;
            bigBg.load(BIG_BG_PATH);
            city_->setBackground(basicBg, bigBg);  // Throws InitError if an error occurs
            QPixmap bgPixmap = city_->getBackGround();
            w_->setPicture(bgPixmap);
        }
        catch (const Interface::InitError& error) {
            std::cerr << error.what() << ": "
                      << error.giveMessage().toStdString() << std::endl;
            throw;
        }

        emit timeChanged(minutes_, seconds_);  // Setting the starting time
        qDebug() << "Closing the dialog and opening the main program";
        w_->show();
    } else {
        dialogAccepted_ = false;
        qDebug() << "Dialog rejected";
    }
}

Engine::~Engine()
{
    delete d_;
    delete w_;
}

bool Engine::wasDialogAccepted() const
{
    return dialogAccepted_;
}

void Engine::setSettings(unsigned int minutes, unsigned int seconds, bool manualTime, QTime gameTime, QString playername)
{
    qDebug() << "minutes: " << minutes << ", seconds: " << seconds
             << ", manual time:" << manualTime;
    minutes_ = minutes;
    seconds_ = seconds;
    manualTime_ = manualTime;
    manualGameTime_ = gameTime;
    if(playername == "" or playername.contains(':')){
      playerName_ = "Anon";
    }else{
      playerName_ = playername;
    }

}

void Engine::startGame()
{
    logic_.finalizeGameStart();
    initStops();
    initPlayer();
    initEnemy();
    timeLeftDecreaser_.start();
    animationTimer_.start();
    playerMovementTimer_.start();
    updateScoreTimer_.start();
    enemyMovementTimer_.start();
}

void Engine::decreaseTimeLeft()
{
    if (seconds_ > 0) {
        seconds_ = seconds_ - 1;
    } else if (minutes_ > 0) {
        minutes_ = minutes_ - 1;
        seconds_ = 59;
    } else {
        endGame();
    }
    emit timeChanged(minutes_, seconds_);
}

void Engine::removeBus(std::shared_ptr<Interface::IActor> actor)
{
    if (std::dynamic_pointer_cast<Interface::IVehicle>(actor) != 0) {
        auto itemToBeRemoved = buses_.at(actor);
        w_->removeActor(itemToBeRemoved);
        buses_.erase(actor);
        stats_.nysseRemoved();
    }
}

void Engine::addBus(std::shared_ptr<Interface::IActor> actor)
{
    if (std::dynamic_pointer_cast<Interface::IVehicle>(actor) != 0) {
        Interface::Location loc = actor->giveLocation();
        int x = loc.giveX();
        int y = loc.giveY();
        // (0, 0) is bottom left corner in these coords but top left for the
        // scene so y must be subtracted from 500. Additionally, the graphics
        // items are drawn so that their bounding rect's top left corner is
        // in the given coordinates so half their width and height are subtracted.
        Actoritem* busItem = w_->addActor(x-SIZE.at(BUS).width/2,
                                          500-y-SIZE.at(BUS).height/2, BUS);
        buses_.insert({actor, busItem});
        stats_.newNysse();
    } else if (std::dynamic_pointer_cast<Interface::IPassenger>(actor) != 0) {
        stats_.morePassengers(1);
    }
}

void Engine::updatePositions()
{
    // The coordinates are explained above in addBus()

    for (auto pair : buses_) {
        pair.second->setPos(pair.first->giveLocation().giveX()-SIZE.at(BUS).width/2,
                            500-pair.first->giveLocation().giveY()-SIZE.at(BUS).height/2);
    }

    QRectF rect = map_->sceneRect();
    for (auto pair:players_){
        if(pair.first->getBus() == nullptr){
            pair.second->setPos(pair.first->giveLocation().giveX()-SIZE.at(PLAYER).width/2,
                                500-pair.first->giveLocation().giveY()-SIZE.at(PLAYER).height/2);
        }
        else if(rect.contains(pair.first->getBus()->giveLocation().giveX(),
                              500-pair.first->getBus()->giveLocation().giveY())){
            pair.second->setPos(pair.first->getBus()->giveLocation().giveX()-SIZE.at(PLAYER).width/2,
                                500-pair.first->getBus()->giveLocation().giveY()-SIZE.at(PLAYER).height/2);
        }
        else{
            pair.first->leaveBus(true);
            pair.second->setPos(pair.first->giveLocation().giveX()-SIZE.at(PLAYER).width/2,
                                500-pair.first->giveLocation().giveY()-SIZE.at(PLAYER).height/2);
        }
    }
    for (auto pair : enemy_){
        pair.second->setPos(pair.first->giveLocation().giveX()-SIZE.at(ENEMY).width/2,
                            500-pair.first->giveLocation().giveY()-SIZE.at(ENEMY).height/2);
    }
    checkCollision();
}

void Engine::initPlayer()
{
    std::shared_ptr<Player> player = std::make_shared<Player>(city_);
    player_ = player;
    city_->setPlayer(player);
    Interface::Location loc = player->giveLocation();
    int x = loc.giveX();
    int y = loc.giveY();
    Actoritem* playerItem = w_->addPlayer(x-SIZE.at(PLAYER).width/2, y-SIZE.at(PLAYER).height/2);
    players_.insert({player, playerItem});
}

void Engine::movePlayer()
{
    if(city_->getGameState()){
        player_->onMovementClicked(map_, wasd_);
    }
}

void Engine::moveEnemy()
{
    if (city_->getGameState()){
        enemyActor_->moveTowards(player_->giveLocation());
    }
}

void Engine::on_E_pressed()
{
    if (city_->getGameState()){
        player_->onButtonClicked();
    }
}

void Engine::on_Q_pressed()
{
    if (city_->getGameState()){
        player_->onOtherButtonClicked();
    }
}

void Engine::giveScore()
{
    std::shared_ptr<Interface::IVehicle> currBus = player_->getBus();
    if (currBus != nullptr) {
        std::vector<std::shared_ptr<Interface::IPassenger>> passengers = currBus->getPassengers();
        unsigned int scoreToAdd = 0;
        for (auto passenger : passengers) {
            if (player_->isNewPassenger(passenger)) {
                player_->addMetPassenger(passenger);
                scoreToAdd += 10;
            }
        }
        emit scoreAdded(scoreToAdd);
    }
}

void Engine::initStops()
{
    std::vector<std::shared_ptr<Interface::IStop>> stops = city_->getStops();
    for (auto& stop : stops) {
        Interface::Location loc = stop->getLocation();
        int x = loc.giveX();
        int y = loc.giveY();
        Actoritem* stopItem = w_->addActor(x-SIZE.at(STOP).width/2,
                                           500-y-SIZE.at(STOP).height/2, STOP);
        stops_.insert({stop, stopItem});
    }
}

void Engine::setTop10()
{

  std::ofstream outfile;

  outfile.open(TOP10_PATH, std::ios_base::app); // append instead of overwrite
  outfile << playerName_.toStdString() << ": " << stats_.getScore() << "\n";
}

void Engine::initEnemy()
{
    std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>();
    Interface::Location loc = enemy->giveLocation();
    int x = loc.giveX();
    int y = loc.giveY();
    Actoritem* enemyItem = w_->addEnemy(x-SIZE.at(ENEMY).width/2, 500-y-SIZE.at(ENEMY).height/2);
    enemyActor_ = enemy;
    enemy_.insert({enemy, enemyItem});
}

void Engine::checkCollision()
{
    for (auto player : players_) {
        for (auto enemy : enemy_) {
            if (player.second->collidesWithItem(enemy.second)) {
                qDebug() << "Player collided with enemy -- game over";
                endGame();
            }
        }
    }
}

void Engine::endGame()
{
    city_->setGameOver();
    timeLeftDecreaser_.stop();
    animationTimer_.stop();
    playerMovementTimer_.stop();
    updateScoreTimer_.stop();
    setTop10();
    enemyMovementTimer_.stop();
    w_->showGameOver();
}

}
