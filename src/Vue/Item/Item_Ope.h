//  Item_Ope.h  1.0     21/02/14    N.Jarnoux
#ifndef ITEM_OPE_H
#define ITEM_OPE_H

class Item_Ope ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Ope : public Item
{
    public:
        explicit    Item_Ope(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;    //Constructeur
            void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Dessin l'item
};

#endif // ITEM_OPE_H
