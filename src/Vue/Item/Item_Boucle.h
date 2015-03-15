//  Item_Boucle.h  1.0     21/02/14    N.Jarnoux
#ifndef ITEM_BOUCLE_H
#define ITEM_BOUCLE_H

class Item_Boucle ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Boucle : public Item
{
    public:
        explicit    Item_Boucle(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;           //Constructeur
            void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin
};

#endif // ITEM_BOUCLE_H
