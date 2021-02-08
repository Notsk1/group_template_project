#include "statistics.hh"
#include <QtTest>
#include <QSignalSpy>

namespace StudentSide
{

class StatisticsTest : public QObject
{
    Q_OBJECT

public:
    StatisticsTest();
    ~StatisticsTest();

private slots:
    void testMorePassengers();
    void testNysseRemoved();
    void testNewNysse();
    void testNysseLeft();
    void testAddScore();
};

StatisticsTest::StatisticsTest()
{

}

StatisticsTest::~StatisticsTest()
{

}

void StatisticsTest::testMorePassengers()
{
    Statistics stats;
    QSignalSpy spy(&stats, &Statistics::passengersAdded);
    QVERIFY(spy.isValid());
    QCOMPARE(spy.count(), 0);
    stats.morePassengers(99);
    QCOMPARE(spy.count(), 1);
    stats.morePassengers(1);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.takeFirst().at(0), 99);
    QCOMPARE(spy.takeLast().at(0), 1);
}

void StatisticsTest::testNysseRemoved()
{
    Statistics stats;
    stats.setNysseAmount(10);
    QSignalSpy spy(&stats, &Statistics::busRemoved);
    QVERIFY(spy.isValid());
    QCOMPARE(spy.count(), 0);
    stats.nysseRemoved();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(stats.getNysseAmount(), 9u);
    stats.nysseRemoved();
    stats.nysseRemoved();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(stats.getNysseAmount(), 7u);
}

void StatisticsTest::testNewNysse()
{
    Statistics stats;
    stats.setNysseAmount(5);
    QSignalSpy spy(&stats, &Statistics::busAdded);
    QVERIFY(spy.isValid());
    QCOMPARE(spy.count(), 0);
    stats.newNysse();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(stats.getNysseAmount(), 6u);
    stats.newNysse();
    stats.newNysse();
    QCOMPARE(spy.count(), 3);
    QCOMPARE(stats.getNysseAmount(), 8u);
}

void StatisticsTest::testNysseLeft()
{
    Statistics stats;
    QSignalSpy spy(&stats, &Statistics::busLeft);
    QVERIFY(spy.isValid());
    QCOMPARE(spy.count(), 0);
    stats.nysseLeft();
    QCOMPARE(spy.count(), 1);
    stats.nysseLeft();
    stats.nysseLeft();
    QCOMPARE(spy.count(), 3);
}

void StatisticsTest::testAddScore()
{
    Statistics stats;
    QSignalSpy spy(&stats, &Statistics::changeDisplayedScore);
    QVERIFY(spy.isValid());
    QCOMPARE(spy.count(), 0);
    QCOMPARE(stats.getScore(), 0u);
    stats.addScore(100);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(stats.getScore(), 100u);
    stats.addScore(123);
    QCOMPARE(stats.getScore(), 223u);
    stats.addScore(0);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(stats.getScore(), 223u);
    QCOMPARE(spy.takeFirst().at(0), 100u);
    QCOMPARE(spy.takeLast().at(0), 223u);
}

}

QTEST_APPLESS_MAIN(StudentSide::StatisticsTest)

#include "tst_statisticstest.moc"
