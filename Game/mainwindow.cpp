#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include <QDebug>


const int PADDING = 10;

namespace StudentSide
{

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    player_(nullptr),
    enemy_(nullptr),
    wasd_(std::make_shared<std::vector<bool>>(std::vector<bool>{false, false, false, false}))
{
    ui->setupUi(this);
    ui->gameView->setFixedSize(width_+2, height_+2);
    ui->centralwidget->setFixedSize(width_ + 2*PADDING, height_ + 60);
    setWindowTitle("Nysse");

    ui->gameView->move(PADDING-1, 48);
    ui->startButton->move(PADDING , PADDING);
    ui->pushButtonScores->move(150, PADDING);
    ui->scoreInfoLabel->move(width_/2+40, 15);
    ui->scoreLabel->setText("0");
    ui->scoreLabel->move(width_/2+90, 15);
    ui->timeInfoLabel->move(width_-90, 15);
    ui->timeLabel->setText("");
    ui->timeLabel->move(width_-20, 15);
    ui->gameOverLabel->setText("");
    ui->clockLabel->move((width_ + 2*PADDING - 90)/2, 48);
    ui->clockLabel->setText("");
    ui->clockLabel->resize(0, 0);  // Not displayed until there's a clock time to display
    ui->clockLabel->setStyleSheet("QLabel {font-size: 16px; background: rgba(255, 255, 255, 0.65);}");

    map = new QGraphicsScene(this);
    ui->gameView->setScene(map);
    map->setSceneRect(0,0,width_,height_);

    resize(minimumSizeHint());
}

MainWindow::~MainWindow()
{
    delete ui;
    for (auto a : actors_) {
        delete a;
    }
    actors_.clear();

    if (player_ != nullptr) {
        delete player_;
    }

    if (enemy_ != nullptr) {
        delete enemy_;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W:
            wasd_->at(0) = true;
            break;
        case Qt::Key_A:
            wasd_->at(1) = true;
            break;
        case Qt::Key_S:
            wasd_->at(2) = true;
            break;
        case Qt::Key_D:
            wasd_->at(3) = true;
            break;
        case Qt::Key_E:
            emit E_pressed();
            break;
        case Qt::Key_Q:
            emit Q_pressed();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W:
            wasd_->at(0) = false;
            break;
        case Qt::Key_A:
            wasd_->at(1) = false;
            break;
        case Qt::Key_S:
            wasd_->at(2) = false;
            break;
        case Qt::Key_D:
            wasd_->at(3) = false;
    }
}

Actoritem* MainWindow::addActor(int locX, int locY, itemType type)
{
    Actoritem* nActor = new Actoritem(locX, locY, type);
    actors_.push_back(nActor);
    map->addItem(nActor);
    return nActor;
}

void MainWindow::setPicture(QPixmap &bgPixmap)
{
  map->setBackgroundBrush(bgPixmap);
}

Actoritem *MainWindow::addPlayer(int x, int y)
{
  Actoritem* player = new Actoritem(x, y, PLAYER);
  map->addItem(player);
  player_ = player;
  return player;
}

Actoritem *MainWindow::addEnemy(int x, int y)
{
    Actoritem* enemy = new Actoritem(x, y, ENEMY);
    map->addItem(enemy);
    enemy_ = enemy;
    return enemy;
}

void MainWindow::removeActor(QGraphicsItem *actor)
{
    actors_.removeOne(actor);
    map->removeItem(actor);
    delete actor;
}

std::shared_ptr<std::vector<bool> > MainWindow::giveWasd()
{
    return wasd_;
}

QGraphicsScene *MainWindow::giveMap()
{
    return map;
}

void MainWindow::showGameOver()
{
    ui->gameOverLabel->resize(200, 100);
    ui->gameOverLabel->move((width_ + 2*PADDING-200)/2, 100);
    ui->gameOverLabel->setStyleSheet("QLabel { background-color: white;"
                                              "border: 2px solid black;"
                                              "font: bold;"
                                              "font-size: 24px;}");
    ui->gameOverLabel->setAlignment(Qt::AlignCenter);
    ui->gameOverLabel->setText("Game over!\nFinal score: " + ui->scoreLabel->text());
}

void MainWindow::changeTimeLeft(unsigned int min, unsigned int sec)
{
    QString secondsString = sec < 10 ? "0" + QString::number(sec) : QString::number(sec);
    ui->timeLabel->setText(QString::number(min) + ":" + secondsString);
}

void MainWindow::changeScore(unsigned int newScore)
{
    ui->scoreLabel->setText(QString::number(newScore));
}

void MainWindow::updateClock(QTime clock)
{
    QString hours = clock.hour() < 10 ? "0" + QString::number(clock.hour()) : QString::number(clock.hour());
    QString minutes = clock.minute() < 10 ? "0" + QString::number(clock.minute()) : QString::number(clock.minute());
    ui->clockLabel->setText("Clock: " + hours + "." + minutes);
    ui->clockLabel->resize(ui->clockLabel->minimumSizeHint());
}

void MainWindow::on_startButton_clicked()
{
    ui->startButton->setDisabled(true);
    emit gameStarted();
}

void MainWindow::on_pushButtonScores_clicked()
{
    Topwindow topwindw;
    topwindw.exec();
}

}


