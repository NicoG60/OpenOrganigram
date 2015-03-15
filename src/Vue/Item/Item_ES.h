//  Item_ES.h   1.0     21/02/14    N.Jarnoux
#ifndef ITEM_ES_H
#define ITEM_ES_H

class Item_ES ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_ES : public Item
{
    public:
        explicit    Item_ES(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;             //Constructeur
            void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin

};

#endif // ITEM_ES_H
