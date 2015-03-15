//  Item_Rout.h     1.0     22/02/14    N.Jarnoux
#ifndef ITEM_ROUT_H
#define ITEM_ROUT_H

class Item_Rout ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Rout : public Item
{
    public:
        explicit    Item_Rout(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;           //Constructeur
            void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin

};

#endif // ITEM_ROUT_H
