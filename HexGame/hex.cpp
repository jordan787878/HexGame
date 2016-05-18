#include "hex.h"
#include <QBrush>
#include "game.h"
#include <QDebug>
#include <typeinfo>

extern Game * game;

Hex::Hex(QString player, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
    //draw
    QVector<QPointF> hexpoints;
    hexpoints << QPointF(1,0) << QPointF(3,0) << QPointF(4,2)
              << QPointF(3,4) << QPointF(1,4) << QPointF(0,2);

    int SCALE_BY = 30;
    for(size_t i=0,n=hexpoints.size(); i<n; i++)
    { hexpoints[i] *= SCALE_BY; }

    QPolygonF hexagon(hexpoints);

    setPolygon(hexagon);

    //set owner
    setowner(player);


    attackside0 = 0;
    attackside1 = 0;
    attackside2 = 0;
    attackside3 = 0;
    attackside4 = 0;
    attackside5 = 0;

if(getowner() != "NOONE") {

    createLines();

    QGraphicsTextItem * text0 = new QGraphicsTextItem(QString::number(attackside0),this);
    QGraphicsTextItem * text1 = new QGraphicsTextItem(QString::number(attackside1),this);
    QGraphicsTextItem * text2 = new QGraphicsTextItem(QString::number(attackside2),this);
    QGraphicsTextItem * text3 = new QGraphicsTextItem(QString::number(attackside3),this);
    QGraphicsTextItem * text4 = new QGraphicsTextItem(QString::number(attackside4),this);
    QGraphicsTextItem * text5 = new QGraphicsTextItem(QString::number(attackside5),this);

    attackTextlist.append(text0);
    attackTextlist.append(text1);
    attackTextlist.append(text2);
    attackTextlist.append(text3);
    attackTextlist.append(text4);
    attackTextlist.append(text5);

    text0->setPos(55,0);
    text1->setPos(18,31);
    text2->setPos(18,75);
    text3->setPos(55,100);
    text4->setPos(90,75);
    text5->setPos(90,31);
  }

}

void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if(IsPlaced == false && this->getowner() == game->getwhosturn_())
  {
      game->PickUpCards(this);
  }

  else if( this->pos().x() >= 160 && this->pos().x() <= 800 && this->pos().y() >= 10 &&
              this->pos().y() <= 600)
  {
      game->PlaceCards(this);
  }

}


bool Hex::getIsPlaced()
{
    return IsPlaced;
}

void Hex::setIsPlaced(bool b)
{
    IsPlaced = b;
}


void Hex::setowner(QString player)
{
    owner = player;

    if(owner == QString("NOONE"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::lightGray);
        setBrush(brush);
    }

    if(owner == QString("PLAYER1"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::blue);
        setBrush(brush);
    }

    if(owner == QString("PLAYER2"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        setBrush(brush);
    }
}

QString Hex::getowner()
{
   return owner;

}

void Hex::setAttackOf(int side, int attack)
{
    switch (side) {
    case 0:
        attackside0 = attack;
        attackTextlist[0]->setPlainText(QString::number(attackside0));
        break;
    case 1:
        attackside1 = attack;
        attackTextlist[1]->setPlainText(QString::number(attackside1));
        break;
    case 2:
        attackside2 = attack;
        attackTextlist[2]->setPlainText(QString::number(attackside2));
        break;
    case 3:
        attackside3 = attack;
        attackTextlist[3]->setPlainText(QString::number(attackside3));
        break;
    case 4:
        attackside4 = attack;
        attackTextlist[4]->setPlainText(QString::number(attackside4));
        break;
    case 5:
        attackside5 = attack;
        attackTextlist[5]->setPlainText(QString::number(attackside5));
        break;
    default:
        break;
    }
}


int Hex::getAttackOf(int side)
{
    switch (side) {
    case 0:
        return attackside0;
    case 1:
        return attackside1;
    case 2:
        return attackside2;
    case 3:
        return attackside3;
    case 4:
        return attackside4;
    case 5:
        return attackside5;
    }
}

void Hex::createLines()
{
    QPointF HexCenter(60,51.96);
    QPointF FinalPoint(HexCenter.x(),HexCenter.y()-80);
    QLineF ln(HexCenter,FinalPoint);

    for(size_t i=0,n=6; i<n; i++)
    {
       QLineF lncopy(ln);
       lncopy.setAngle(100+60*i);
       QGraphicsLineItem * line = new QGraphicsLineItem(lncopy,this);
       lines.append(line);
       line->setVisible(false);
    }

}

void Hex::findNeighbors()
{
    //6 loop
    for(int i=0,n=lines.size(); i<n; i++)
    {
       QList<QGraphicsItem *> colliding_items = lines[i]->collidingItems();

       for(size_t j=0;j<colliding_items.size();j++)
       {
          Hex * items = dynamic_cast<Hex *>(colliding_items[j]);
          if( colliding_items[j] != this && items )
          {
              neighbors.append(items);
          }
       }

    }


}

void Hex::captureNeighbors()
{

   for(size_t i=0,n=neighbors.size();i<n;i++)
   {
      if(this->getowner() != neighbors[i]->getowner() && neighbors[i]->getowner() != "NOONE")
      {
          int MyAttack = 0;
          int OtherAttack = 0;

          switch (i) {
          case 0:
              MyAttack = this->getAttackOf(0);
              OtherAttack = neighbors[0]->getAttackOf(3);
              break;
          case 1:
              MyAttack = this->getAttackOf(1);
              OtherAttack = neighbors[1]->getAttackOf(4);
              break;
          case 2:
              MyAttack = this->getAttackOf(2);
              OtherAttack = neighbors[2]->getAttackOf(5);
              break;
          case 3:
              MyAttack = this->getAttackOf(3);
              OtherAttack = neighbors[3]->getAttackOf(0);
              break;
          case 4:
              MyAttack = this->getAttackOf(4);
              OtherAttack = neighbors[4]->getAttackOf(1);
              break;
          case 5:
              MyAttack = this->getAttackOf(5);
              OtherAttack = neighbors[5]->getAttackOf(2);
              break;

          default:
              break;
          }

          if(MyAttack == 1 && OtherAttack == 6)
          {
              neighbors[i]->switchowner();
          }

          if(MyAttack > OtherAttack)
          {
              neighbors[i]->switchowner();
          }
      }
   }

}


void Hex::switchowner()
{

   if(getowner() == "PLAYER1")
   {
       setowner("PLAYER2");
       return;
   }

   if(getowner() == "PLAYER2")
   {
       setowner("PLAYER1");
       return;
   }

}
