#include "actoritem.hh"

namespace StudentSide {
  Actoritem::Actoritem(int x, int y, itemType type)
  {
    setPos(mapToParent(x, y ));
    setValues(type);
  }

  QRectF Actoritem::boundingRect() const
  {
    return QRectF(0, 0, width_, height_);
  }

  void Actoritem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
  {
    QRectF bounds = boundingRect();
    QColor color(r_%256, g_%256, b_%256);
    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawRect(bounds);
    Q_UNUSED(option);
    Q_UNUSED(widget);
  }

  void Actoritem::setValues(itemType type)
  {
    if(type == PLAYER){
      setZValue(1);
    }else if(type == ENEMY){
      setZValue(2);
    }
    for(auto pair : COLORS){
      if(pair.first == type){
        r_ = pair.second.at(0);
        g_ = pair.second.at(1);
        b_ = pair.second.at(2);
      }
    }
    for(auto pair : SIZE){
      if(pair.first == type){
        width_ = pair.second.width;
        height_ = pair.second.height;
      }
    }
  }
}


