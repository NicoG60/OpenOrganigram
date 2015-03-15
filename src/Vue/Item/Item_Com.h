//  Item_Com.h  1.0     22/02/14        N.Jarnoux
#ifndef ITEM_COM_H
#define ITEM_COM_H

class Item_Com ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Com : public Item
{
    public:
        explicit    Item_Com(Instruction *   pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;         //Constructeur                                                          //Constructeur
        void        paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin

};

#endif // ITEM_COM_H
