//  MarqueurDrop.h  1.0     04/04/14    N. Jarnoux
#ifndef MARQUEURDROP_H
#define MARQUEURDROP_H

class MarqueurDrop ;

//===== Headers standards =====
#include <QGraphicsItem>

//=====   Headers Peros   =====

class MarqueurDrop : public QGraphicsItem
{
public:
    MarqueurDrop(QGraphicsItem *  pParent = 0);   //Constructeur
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) ;                                 //Fonction de dessin de la fleche
    QRectF  boundingRect() const;   //Rectangle de raffraichissement
};

#endif // MARQUEURDROP_H
