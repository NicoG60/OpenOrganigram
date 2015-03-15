// Fleche.h     1.0     21/03/14    N. Jarnoux
#ifndef FLECHE_H
#define FLECHE_H

class Fleche ;

//===== Headers standards =====
#include <QGraphicsItem>

//=====   Headers Peros   =====
#include "Item.h"

class Fleche : public QGraphicsItem
{
    public:
        Fleche(Item *   Depart, Item *  Arrivee, bool bSortieDecaleeCondition = false, QGraphicsItem *  pParent = 0);   //Constructeur
        void    paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) ;                                 //Fonction de dessin de la fleche
        QRectF  boundingRect() const;   //Rectangle de raffraichissement

        inline  Item *  getItemArrivee()    { return pItemArrivee ; }
        inline  Item *  getItemDepart()     { return pItemDepart ; }

        QPointF PointLePlusPres(QPointF Position) ;
        bool    EstVerticale();
        bool    EstDecaleeDroite() ;
        bool    EstPresDuDepart(QPointF Point) ;

    private:
        QList<QPointF>  ListePoints ;   //Liste des points par lesquels passent les flèches

        Item *          pItemDepart ;   //Pointeur vers l'item de départ de la flèche
        Item *          pItemArrivee ;  //Pointeur vers l'item d'arrivé

        bool            bSortieDecaleeCondition ;   //True si c'est une sortie de condition, False sinon

        void            CalculerCoordonnees() ;     //Calcul la liste des points
        void            CreerFlecheDecalleeVide(QPointF Depart, QPointF Arrivee); //CRéer un fleche pour sortie ecartée sans item
};

#endif // FLECHE_H
