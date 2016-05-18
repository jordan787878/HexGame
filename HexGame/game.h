#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "hexboard.h"
#include <QMouseEvent>

class Game : public QGraphicsView {
    Q_OBJECT
public:
    Game(QWidget * parent=0);
    void displayMainMenu();
    QString getwhosturn_();
    void setwhosturn_(QString player);

    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

    void PickUpCards(Hex * card);
    void PlaceCards(Hex * HexToReplace);
    void removefromDeck(Hex * cardtoremove, QString player);
    void nextTurn();

    Hex * PlaceOne;
    QGraphicsScene * scene;
    HexBoard * hexboard;
    QPointF originalPos;

    int numberOfCard;
    int p1card;
    int p2card;

public slots:
    void start();
    void restart();
    void exit();

private:
    void gameover();
    void displayOverWindow(QString Who_Win,QColor coloroftext);
    void drawPanel(int x,int y,int width,int height,QColor color,double opacity);
    void drawGUI();

    void CreateNewCard(QString player);
    void CreateInitialCards();
    void drawCards();

    QString WhosTurn_;
    QGraphicsTextItem * WhosTurnText;
    QList<Hex *> player1Cards;
    QList<Hex *> player2Cards;



};

#endif // GAME_H
