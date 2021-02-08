#include "enemy.hh"
#include <QDebug>

namespace StudentSide
{

Enemy::Enemy()
{
    loc_ = Interface::Location();
    loc_.setXY(ENEMY_STARTING_X, ENEMY_STARTING_Y);
}

Interface::Location Enemy::giveLocation() const
{
    return loc_;
}

void Enemy::moveTowards(Interface::Location playerLoc)
{
    int xDir = 0;
    int yDir = 0;
    if (loc_.giveX() != playerLoc.giveX()) {
        xDir = (playerLoc.giveX()-loc_.giveX())/std::labs(playerLoc.giveX()-loc_.giveX());
    }
    if (loc_.giveY() != playerLoc.giveY()) {
        yDir = (playerLoc.giveY()-loc_.giveY())/std::labs(playerLoc.giveY()-loc_.giveY());
    }
    int oldX = loc_.giveX();
    int oldY = loc_.giveY();
    loc_.setXY(oldX + xDir, oldY + yDir);
}

}
