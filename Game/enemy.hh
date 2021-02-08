#ifndef ENEMY_HH
#define ENEMY_HH

#include "core/location.hh"

namespace StudentSide
{
int const ENEMY_STARTING_X = 250;
int const ENEMY_STARTING_Y = 400;  // 100 from the top

class Enemy
{

public:
    Enemy();
    Interface::Location giveLocation() const;
    void moveTowards(Interface::Location loc);

private:
    Interface::Location loc_;
};

}

#endif // ENEMY_HH
