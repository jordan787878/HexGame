#include "game.h"
#include "hex.h"
#include "button.h"
#include <QDebug>
#include <QFont>
#include <stdlib.h>
#include <time.h>

Game::Game(QWidget *parent) : QGraphicsView(parent)
{
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setScene(scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1027,768);

    PlaceOne = NULL;

    numberOfCard = 0;

    unsigned int seed = time(NULL);
    srand(seed);

}

void Game::displayMainMenu()
{
    QGraphicsTextItem * title = new QGraphicsTextItem(QString("Hex Warz"));
    QFont titlefont("comic sans",50);
    title->setFont(titlefont);
    int txPos = this->width()/2 - title->boundingRect().width()/2;
    title->setPos(txPos,200);
    scene->addItem(title);

    Button * playbtn = new Button("Play");
    int pbxPox = this->width()/2 - playbtn->boundingRect().width()/2;
    playbtn->setPos(pbxPox,320);
    connect(playbtn,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playbtn);

    Button * exitbtn = new Button("Exit");
    exitbtn->setPos(pbxPox,400);
    connect(exitbtn,SIGNAL(clicked()),this,SLOT(exit()));
    scene->addItem(exitbtn);
}

QString Game::getwhosturn_()
{
    return WhosTurn_;
}

void Game::setwhosturn_(QString player)
{
    WhosTurn_ = player;

    //display whosTurn text
    WhosTurnText->setPlainText(QString("Whos Turn:\n ") + player);
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(PlaceOne)
    {
       PlaceOne->setPos(event->pos());
    }


     QGraphicsView::mouseMoveEvent(event);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        if(PlaceOne)
        {
            PlaceOne->setPos(originalPos);
            PlaceOne = NULL;
            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void Game::PickUpCards(Hex * card)
{
   PlaceOne = card;
   PlaceOne->setowner(card->getowner());
   originalPos = PlaceOne->pos();
}

void Game::PlaceCards(Hex *HexToReplace)
{
  if(PlaceOne  && HexToReplace->getIsPlaced() == false)
  {

    PlaceOne->setPos(HexToReplace->pos());
    hexboard->hexes.append(PlaceOne);
    PlaceOne->setIsPlaced(true);
    scene->removeItem(HexToReplace);
    PlaceOne->findNeighbors();
    PlaceOne->captureNeighbors();

    removefromDeck(PlaceOne,getwhosturn_());
    CreateNewCard(getwhosturn_());
    nextTurn();

    PlaceOne = NULL;

    numberOfCard++;
    if(numberOfCard == 32)
    {
        gameover();
    }

  }

}

void Game::removefromDeck(Hex *cardtoremove, QString player)
{
   if(player  == "PLAYER1")
   {
       player1Cards.removeAll(cardtoremove);
   }

   if(player  == "PLAYER2")
   {
       player2Cards.removeAll(cardtoremove);
   }

}

void Game::nextTurn()
{
    if(getwhosturn_() == "PLAYER1")
    {
       setwhosturn_("PLAYER2");
    }

    else
    {
       setwhosturn_("PLAYER1");
    }
}

void Game::start()
{
    scene->clear();
    hexboard = new HexBoard();
    hexboard->placeHexes(-20,-80,10,7);
    drawGUI();
    CreateInitialCards();
}

void Game::restart()
{
    numberOfCard = 0;
    player1Cards.clear();
    player2Cards.clear();
    hexboard->hexes.clear();
    scene->clear();
    start();
}

void Game::exit()
{
    delete this;
}

void Game::gameover()
{
   p1card = 0;
   p2card = 0;

   QString message;
   QColor color;
   for(size_t i=0;i<hexboard->hexes.size();i++)
   {
       if(hexboard->hexes[i]->getowner() == "PLAYER1")
       {
           p1card++;
       }

       if(hexboard->hexes[i]->getowner() == "PLAYER2")
       {
           p2card++;
       }
   }

   if(p1card > p2card)
   {
       message = "Player  1  Win~~";
       color = Qt::blue;
   }

   else if(p1card < p2card)
   {
       message = "Player  2  Win~~";
       color = Qt::red;
   }

   else if(p1card == p2card)
   {
       message = "Tie Game";
   }


   displayOverWindow(message,color);
}

void Game::displayOverWindow(QString Who_Win, QColor coloroftext)
{
    for(size_t i =0, n = scene->items().size(); i<n; i++)
    {
        scene->items()[i]->setEnabled(false);
    }

    drawPanel(0,0,1024,768,Qt::black,0.65);

    drawPanel(220,184,570,360,Qt::white,0.9);

    Button * playagainbtn = new Button("Play Again");
    playagainbtn->setPos(410,330);
    connect(playagainbtn,SIGNAL(clicked()),this,SLOT(restart()));
    scene->addItem(playagainbtn);

    Button * quitbtn = new Button("Exit");
    quitbtn->setPos(410,405);
    connect(quitbtn,SIGNAL(clicked()),this,SLOT(exit()));
    scene->addItem(quitbtn);


//    scene->clear();
    QGraphicsTextItem * overtext = new QGraphicsTextItem(Who_Win);
    QFont overfont("comic sans",50);
    overtext->setFont(overfont);
    overtext->setDefaultTextColor(coloroftext);
    int txPos = this->width()/2 - overtext->boundingRect().width()/2;
    overtext->setPos(txPos,200);
    scene->addItem(overtext);

    QGraphicsTextItem * p1score = new QGraphicsTextItem("P1 Cards: " + QString::number(p1card));
    p1score->setDefaultTextColor(Qt::blue);
    p1score->setPos(430,300);
    scene->addItem(p1score);

    QGraphicsTextItem * p2score = new QGraphicsTextItem("P2 Cards: " + QString::number(p2card));
    p2score->setDefaultTextColor(Qt::red);
    p2score->setPos(530,300);
    scene->addItem(p2score);

}



void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity)
{
   //draw panel with specific loction width x height
   QGraphicsRectItem * panel = new QGraphicsRectItem(x,y,width,height);
   QBrush brush;
   brush.setStyle(Qt::SolidPattern);
   brush.setColor(color);
   panel->setBrush(brush);
   panel->setOpacity(opacity);
   scene->addItem(panel);

}

void Game::drawGUI()
{
    drawPanel(0,0,130,700,Qt::yellow,1);
    QGraphicsTextItem * ply1 = new QGraphicsTextItem(QString("player 1"));
    ply1->setPos(45,15);
    scene->addItem(ply1);

    drawPanel(880,0,130,700,Qt::yellow,1);
    QGraphicsTextItem * ply2 = new QGraphicsTextItem(QString("player 2"));
    ply2->setPos(880+45,15);
    scene->addItem(ply2);

    WhosTurnText = new QGraphicsTextItem();
    setwhosturn_("PLAYER1");
    WhosTurnText->setPos(480,10);
    scene->addItem(WhosTurnText);
}

void Game::CreateNewCard(QString player)
{
    Hex * hexcard = new Hex(player);
    hexcard->setIsPlaced(false);

    for(size_t i=0,n=6; i<n; i++)
    {
       int randNum = 1+ rand()%6;

       hexcard->setAttackOf(i,randNum);
    }


    if(player == QString("PLAYER1"))
    {
        player1Cards.append(hexcard);
    }
    else
    {
        player2Cards.append(hexcard);
    }

    drawCards();
}

void Game::CreateInitialCards()
{
    for(size_t i=0,n=5; i<n; i++)
    {
        CreateNewCard(QString("PLAYER1"));
    }

    for(size_t i=0,n=5; i<n; i++)
    {
        CreateNewCard(QString("PLAYER2"));
    }

}

void Game::drawCards()
{
    for(size_t i=0,n=player1Cards.size(); i<n; i++)
    {
        scene->removeItem(player1Cards[i]);
    }

    for(size_t i=0,n=player2Cards.size(); i<n; i++)
    {
        scene->removeItem(player2Cards[i]);
    }


    for(size_t i=0,n=player1Cards.size(); i<n; i++)
    {
        Hex * cards = player1Cards[i];
        cards->setPos(5,45+125*i);
        scene->addItem(cards);
    }

    for(size_t i=0,n=player2Cards.size(); i<n; i++)
    {
        Hex * cards = player2Cards[i];
        cards->setPos(885,45+125*i);
        scene->addItem(cards);
    }


}



