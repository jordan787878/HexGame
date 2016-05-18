#include "hexboard.h"
#include "hex.h"
#include "game.h"
#include <QDebug>
#include <QBrush>

extern Game * game;

HexBoard::HexBoard()
{

}

QList<Hex *> HexBoard::getHexes()
{
    return hexes;
}

void HexBoard::placeHexes(int x, int y, int cols, int rows)
{
    int X_SHIFT = 95;
    int Y_SHIFT = 60;
    int k=0;

    for(int i=0,n=cols; i<n; i++)
    {
        k = ((i%2) == 0 ? 0 : 1);
        createHexCol(x+X_SHIFT*i, y+Y_SHIFT*k ,rows);
    }
}

void HexBoard::createHexCol(int x, int y, int num)
{
    for(int i=0,n=num; i<n ;i++)
    {
        Hex * hex = new Hex("NOONE");
        hex->setPos(x,y+125*i);
        game->scene->addItem(hex);

        if(hex->pos().x() <= 160 || hex->pos().x() >= 800 || hex->pos().y() <= 10 ||
                hex->pos().y() >= 600)
        {
            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(Qt::white);
            hex->setBrush(brush);
            hex->setPen(Qt::DotLine);
        }


    }
}
