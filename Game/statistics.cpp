#include "statistics.hh"

namespace StudentSide
{

Statistics::Statistics(): score_(0), numberOfNysses_(0)
{
}

Statistics::~Statistics()
{
}

void Statistics::morePassengers(int num)
{
    emit passengersAdded(num);
}

void Statistics::nysseRemoved()
{
    emit busRemoved();
    --numberOfNysses_;
}

void Statistics::newNysse()
{
    emit busAdded();
    ++numberOfNysses_;
}

void Statistics::nysseLeft()
{
    emit busLeft();
}

unsigned int Statistics::getNysseAmount()
{
    return numberOfNysses_;
}

void Statistics::setNysseAmount(unsigned int number)
{
    numberOfNysses_ = number;
}

unsigned int Statistics::getScore()
{
    return score_;
}

void Statistics::addScore(unsigned int num)
{
    score_ += num;
    emit changeDisplayedScore(score_);
}

}
