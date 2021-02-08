
#include <QtTest>
#include <QString>

#include "city.hh"
namespace StudentSide {

class CityTest : public QObject
{
  Q_OBJECT

public:
  CityTest();
  ~CityTest();

private slots:
  void testAddActor();


  void testRemoveActor();

};

CityTest::CityTest()
{

}

CityTest::~CityTest()
{

}

void CityTest::testAddActor()
{
  std::unique_ptr<City> kaupunki = std::make_unique<City>();
  std::shared_ptr<Interface::IActor> newactor = nullptr;
  kaupunki->addActor(newactor);
  QCOMPARE(kaupunki->giveCityActorsLength(),1);
}

void CityTest::testRemoveActor()
{
  std::unique_ptr<City> kaupunki = std::make_unique<City>();
  std::shared_ptr<Interface::IActor> newactor = nullptr;
  kaupunki->addActor(newactor);
  QCOMPARE(kaupunki->giveCityActorsLength(),1);
  kaupunki->removeActor(newactor);
  QCOMPARE(kaupunki->giveCityActorsLength(),0);
}
}

//QTEST_MAIN cause city uses pixmap which needs qui app
QTEST_MAIN(StudentSide::CityTest)


#include "citytest.moc"
