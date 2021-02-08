#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QPixmap>
#include <iostream>

namespace StudentSide {

  enum itemType {PLAYER, ENEMY, BUS, STOP};

  struct Dimensions {
    int width;
    int height;
  };

  const std::map<itemType, Dimensions> SIZE = {{PLAYER,{8,16}},{ENEMY,{8,16}},
                                               {BUS,{20,10}},{STOP,{10,10}}};

  class Actoritem : public QGraphicsItem
  {
  public:
    Actoritem(int x, int y, itemType type);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setValues(itemType type);

  private:
    int r_;
    int g_;
    int b_;
    int width_;
    int height_;
    const std::map<itemType, std::vector<int>> COLORS = {{PLAYER,{1,255,1}},{ENEMY,{255,1,1}},
                                                         {BUS,{1,1,255}},{STOP,{100,100,100}}};
  };

}

#endif // PLAYERITEM_HH
