#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "../Course/CourseLib/interfaces/iactor.hh"
#include "../Course/CourseLib/graphics/simpleactoritem.hh"
#include "player.hh"
#include "actoritem.hh"
#include "topwindow.h"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <memory>
#include <QVector>
#include <QKeyEvent>


namespace Ui {
class MainWindow;
}

namespace StudentSide
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    Actoritem* addActor(int locX, int locY, itemType type);

    void setPicture(QPixmap& bgPixmap);
    Actoritem* addPlayer(int x, int y);
    Actoritem* addEnemy(int x, int y);
    void removeActor(QGraphicsItem* actor);
    std::shared_ptr<std::vector<bool>> giveWasd();
    QGraphicsScene* giveMap();
    void showGameOver();

public slots:
    void changeTimeLeft(unsigned int min, unsigned int sec);
    void changeScore(unsigned int newScore);
    void updateClock(QTime clock);

signals:
    void gameStarted();
    void E_pressed();
    void Q_pressed();

private slots:
    void on_startButton_clicked();

    void on_pushButtonScores_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *map;
    QVector<QGraphicsItem*> actors_;
    Actoritem* player_;
    Actoritem* enemy_;
    std::shared_ptr<std::vector<bool>> wasd_;

    int width_ = 500; //pxls
    int height_ = 500;
};

}

#endif // MAINWINDOW_HH
