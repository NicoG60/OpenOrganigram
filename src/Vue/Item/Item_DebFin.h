//  Item_DebFin.h   1.0     21/02/14    N.Jarnoux
#ifndef ITEM_DEBFIN_H
#define ITEM_DEBFIN_H

class Item_DebFin ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_DebFin : public Item
{
    public:
        explicit            Item_DebFin(Instruction *pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;         //Constructeur
                    void    paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ;//Fonction de dessin

    protected:
                    void    contextMenuEvent(QGraphicsSceneContextMenuEvent *   pEvent) ;   //Gestion de l'évenement clic droit
                    void    mouseDoubleClickEvent(QGraphicsSceneMouseEvent *    pEvent) ;   //Gestion du double clic
};

#endif // ITEM_DEBFIN_H
