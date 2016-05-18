#ifndef HEXBOARD_H
#define HEXBOARD_H
#include <QList>
#include "hex.h"

class HexBoard {
public:
    HexBoard();

    QList<Hex*> getHexes();
    void placeHexes(int x, int y, int cols, int rows);
    QList<Hex*> hexes;

private:
    void createHexCol(int x,int y,int num);
};

#endif // HEXBOARD_H
