//  Item_Tempo.h    1.0     22/02/14    N.Jarnoux
#ifndef ITEM_TEMPO_H
#define ITEM_TEMPO_H

class Item_Tempo ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Tempo : public Item
{
    public:
        explicit    Item_Tempo(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;          //Constructeur
            void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin

};

#endif // ITEM_TEMPO_H
