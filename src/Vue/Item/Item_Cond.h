//  Item_Cond.h     1.0     21/02/14    N.Jarnoux
#ifndef ITEM_COND_H
#define ITEM_COND_H

class Item_Cond ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Item.h"

class Item_Cond : public Item
{
    public:
        explicit            Item_Cond(Instruction *   pInst, Item * pItemPrec = 0, QGraphicsItem *pParent = 0) ;        //Constructeur
                    void    paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/) ; //Fonction de dessin
            unsigned int    getValEcartementSortie()    { return nValEcartementSortie ; }   //Accesseur de la valeur d'écartement de sa sortie déportée
                    void    setValEcartementSortie(unsigned int nValEcartementSortie ) ;    //Mutateur de cette même valeur

    private:
            unsigned int    nValEcartementSortie;   //Valeur de l'écartement de la sortie déportée

};

#endif // ITEM_COND_H
