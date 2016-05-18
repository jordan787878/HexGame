#ifndef HEX_H
#define HEX_H

#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QMouseEvent>

class Hex : public QGraphicsPolygonItem{
public:
    Hex(QString player,QGraphicsItem * parent=0);

    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    bool getIsPlaced();
    void setIsPlaced(bool b);
    void setowner(QString player);
    QString getowner();
    void setAttackOf(int side,int attack);
    int getAttackOf(int side);

    void createLines();
    void findNeighbors();
    void captureNeighbors();
    void switchowner();


private:

    bool IsPlaced;

    int attackside0;
    int attackside1;
    int attackside2;
    int attackside3;
    int attackside4;
    int attackside5;
    QString owner;
    QList<QGraphicsTextItem *> attackTextlist;
    QList<Hex *> neighbors;
    QList<QGraphicsLineItem *> lines;
};

#endif // HEX_H
