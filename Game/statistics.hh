#ifndef STATISTICS_HH
#define STATISTICS_HH

#include "interfaces/istatistics.hh"
#include <QObject>

namespace StudentSide
{

class Statistics : public QObject, public Interface::IStatistics
{
    Q_OBJECT
public:
    Statistics();
    ~Statistics();
    void morePassengers(int num);
    void nysseRemoved();
    void newNysse();
    void nysseLeft();

    // Used just for testing
    unsigned int getNysseAmount();
    void setNysseAmount(unsigned int number);
    unsigned int getScore();

public slots:
    void addScore(unsigned int num);

signals:
    void passengersAdded(int num);
    void busRemoved();
    void busAdded();
    void busLeft();
    void changeDisplayedScore(unsigned int score);

private:
    unsigned int score_;
    unsigned int numberOfNysses_;
};

}

#endif // STATISTICS_HH
