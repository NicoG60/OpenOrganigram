//-------------------------------------------------------------------------------
/**
 * @file        Item.cpp
 * @brief       Représentation d'un élément grpahique
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Représente un élément graphique, cette classe sera dérivée en classe fille pour différencier tout les
 * types d'items.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include<QtCore/qmath.h>

//=====   Headers Peros   =====
#include "Fleche.h"
#include "Item/Item_Cond.h"

/**
 * Constructeur
 *
 * @brief   Fleche::Fleche(Item *Depart, Item *Arrivee, QGraphicsItem *pParent)
 * @param   Depart      Item de départ de la fleche
 * @param   Arrivee     Item d'arrivee de la fleche
 * @param   pParent     Pointeur vers l'objet Item (ou dérivé) parent.
 */
Fleche::Fleche(Item *Depart, Item *Arrivee, bool bSortieDecaleeCondition, QGraphicsItem *pParent) :
    QGraphicsItem           (pParent),
    pItemDepart             (Depart),
    pItemArrivee            (Arrivee),
    bSortieDecaleeCondition (bSortieDecaleeCondition)
{
    this->CalculerCoordonnees() ;
}

/**
 * Calcul les coordonnnées des différents points
 *
 * @brief   Fleche::CalculerCoordonnees()
 */
void Fleche::CalculerCoordonnees()
{
    QPointF PointDepart     (pItemDepart->pos()) ;
    QPointF PointArrivee    (pItemArrivee->pos()) ;

    //Si les deux item sont parfaitement allignés
    if(PointDepart.x() == PointArrivee.x())
    {
        if(this->bSortieDecaleeCondition)
        {
            this->CreerFlecheDecalleeVide(PointDepart, PointArrivee);
        }
        else
        {
            qreal   YDepart     (PointDepart.y() + TAILLE_V/2) ;
            qreal   YArrivee    (PointArrivee.y() - TAILLE_V/2) ;

            PointDepart.setY(YDepart) ;
            PointArrivee.setY(YArrivee) ;

            this->ListePoints.append(PointDepart);
            this->ListePoints.append(PointArrivee);
        }
    }
    else
    {
        //Sinon, si c'est un décalage vers la droite
        if(PointDepart.x() < PointArrivee.x())
        {
            qreal   XDepart     (PointDepart.x() + TAILLE_H/2) ;
            qreal   YArrivee    (PointArrivee.y() - TAILLE_V/2) ;

            PointDepart.setX(XDepart) ;
            PointArrivee.setY(YArrivee) ;

            QPointF Intermediraire  (PointArrivee.x(), PointDepart.y()) ;

            this->ListePoints.append(PointDepart);
            this->ListePoints.append(Intermediraire);
            this->ListePoints.append(PointArrivee);

        }
        else //Sinon c'est un décalage vers la gauche
        {
            if(this->bSortieDecaleeCondition)
            {
                this->CreerFlecheDecalleeVide(PointDepart, PointArrivee);
            }
            else
            {
                qreal   YDepart     (PointDepart.y() + TAILLE_V/2) ;
                qreal   YArrivee    (PointArrivee.y() - TAILLE_V) ;

                PointDepart.setY(YDepart) ;
                PointArrivee.setY(YArrivee) ;

                QPointF Intermediraire  (PointDepart.x(), PointArrivee.y()) ;

                this->ListePoints.append(PointDepart);
                this->ListePoints.append(Intermediraire);
                this->ListePoints.append(PointArrivee);
            }

        }
    }
}

/**
 * Dessine la fleche
 *
 * @brief   Fleche::CalculerCoordonnees()
 */
void Fleche::paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    for(register int i = 0; i < this->ListePoints.length() -1; i++)
    {
        painter->drawLine(this->ListePoints[i], this->ListePoints[i+1]);
    }

    QBrush  brush(Qt::black) ;

    QPointF PointeTriangle  (this->ListePoints.last()) ;
    int     IndicePointPrec (this->ListePoints.length() - 2) ;
    qreal   YPointe         (PointeTriangle.y()) ;
    qreal   YPointPrec      (this->ListePoints[IndicePointPrec].y()) ;

    qreal   X1 ;
    qreal   Y1 ;
    qreal   X2 ;
    qreal   Y2 ;


    if(YPointe == YPointPrec)
    {
        X1 = PointeTriangle.x() + 5 ;
        Y1 = PointeTriangle.y() - 5 ;
        X2 = PointeTriangle.x() + 5 ;
        Y2 = PointeTriangle.y() + 5 ;
    }
    else
    {
        X1 = PointeTriangle.x() + 5 ;
        Y1 = PointeTriangle.y() - 5 ;
        X2 = PointeTriangle.x() - 5 ;
        Y2 = PointeTriangle.y() - 5 ;
    }
    QPointF AutrePoint1     (X1, Y1) ;
    QPointF AutrePoint2     (X2, Y2) ;

    QPolygonF poly ;

    poly.append(PointeTriangle) ;
    poly.append(AutrePoint1) ;
    poly.append(AutrePoint2) ;

    painter->setBrush(brush);
    painter->drawPolygon(poly);
}

/**
 * Retourne le rectangle à prendre en compte pour l'affichage
 * @brief Fleche::boundingRect
 * @return  le rectangle à prendre en compte pour l'affichage
 */
QRectF Fleche::boundingRect() const
{
    QPolygonF poly;

    for(register int i = 0; i < this->ListePoints.length(); i++)
    {
        poly.append(this->ListePoints[i]) ;
    }

    return poly.boundingRect() ;
}

/**
 * Calcul le point sur la fleche le plus près du point donné en parametre
 * @brief Fleche::PointLePlusPres
 * @param Position  La osition à analyser
 * @return  La position sur la fleche la plus proche
 */
QPointF Fleche::PointLePlusPres(QPointF Position)
{
    //Calcul de la projection orthogonale de la position de la souris sur les segments verticaux de la flèche.
    QPointF Retour ;
    QPointF Projection ;
    qreal   Distance    (0);
    qreal   DistanceMini(INFINITY) ;
    QLineF  LigneBase ;
    QLineF  LigneSourisProj ;

    LigneSourisProj.setPoints(Position, Projection);

    //Pour tout les segments
    for(register int i = 0; i < this->ListePoints.length() -1; i++)
    {
        LigneBase.setPoints(this->ListePoints[i], this->ListePoints[i+1]);

        //S'il sont verticaux
        if(LigneBase.dx() == 0)
        {
            //On calcul les coordonnées du point
            Projection.setX(this->ListePoints[i].x());
            Projection.setY(Position.y());

            //Si le point sort du segment
            if(Projection.y() < this->ListePoints[i].y() || Projection.y() > this->ListePoints[i+1].y())
            {
                QLineF  LigneSourisP1 ;
                QLineF  LigneSourisP2 ;

                LigneSourisP1.setPoints(Position, this->ListePoints[i]);
                LigneSourisP2.setPoints(Position, this->ListePoints[i+1]);

                //On le recale sur l'extrémité la plus proche
                if(LigneSourisP1.length() < LigneSourisP2.length())
                {
                    Projection = this->ListePoints[i] ;
                }
                else
                {
                    Projection = this->ListePoints[i+1] ;
                }
            }

            //On renvois le point le plus proche de la souris
            Distance = LigneSourisProj.length() ;
            if(Distance < DistanceMini)
            {
                Retour          = Projection ;
                DistanceMini    = Distance ;
            }
        }
    }

    return Retour;
}

/**
 * Si la fleche est verticale
 * @brief Fleche::EstVerticale
 * @return Vrai si elle est vertacle, faux sinon
 */
bool Fleche::EstVerticale()
{
    bool Retour (false) ;

    //Si le point de départ est egal au point d'arrivé et qu'il n'y a que 2 point
    if(this->pItemDepart->x() == this->pItemArrivee->x() && this->ListePoints.length() == 2)
    {
        Retour = true;
    }

    return Retour ;
}

/**
 * Si la fleche est décalée à droite
 * @brief Fleche::EstDecaleeDroite
 * @return CRai si elle est décalée à droite, faux sinon
 */
bool Fleche::EstDecaleeDroite()
{
    bool Retour (false) ;

    //Si la liste de point est égale à trois
    if(this->ListePoints.length() == 3)
    {
        // et si l'absysse du point de départ est inférieure au celle du point d'arrivée
        if(this->pItemDepart->x() < this->pItemArrivee->x())
        {
            Retour = true ;
        }
    }
    else    //Sinon si le nombre de point est supérieur à trois et que l'absysse du point de départ est supérieur à celle du point d'arrivée (cas d'un retour de condition vide
    {
        if(this->ListePoints.length() >= 3 && this->pItemDepart->x() >= this->pItemArrivee->x())
        {
            Retour = true ;
        }
    }

    return Retour ;
}

/**
 * Si le poiunt est plus près du dépar que de l'arrivée
 * @brief Fleche::EstPresDuDepart
 * @param Point Le point à analyser
 * @return vrai S'il est plus près du départ faux sinon
 */
bool Fleche::EstPresDuDepart(QPointF Point)
{
    bool Retour(false) ;

    //Si la distance entre le point à analyser et le point de départ est inférieure à celle entre le point à analyser et le point d'arrivée
    if((Point.y() - this->pItemDepart->y()) < (this->pItemArrivee->y() - Point.y()))
    {
        Retour = true;
    }

    return Retour ;
}

/**
 * Crée une flech décalée vie pour les condition vide
 * @brief Fleche::CreerFlecheDecalleeVide
 * @param Depart
 * @param Arrivee
 */
void Fleche::CreerFlecheDecalleeVide(QPointF Depart, QPointF Arrivee)
{
    Item_Cond*  pCondition  (static_cast<Item_Cond*>(this->pItemDepart)) ;

    qreal       XDepart     (Depart.x() + TAILLE_H/2) ;
    qreal       YArrivee    (Arrivee.y() - TAILLE_V) ;

    Depart.setX(XDepart) ;
    Arrivee.setY(YArrivee) ;

    QPointF Intermediraire1  (this->pItemDepart->x() + (ESP_H/2.0) + (ESP_H * (pCondition->getValEcartementSortie())), Depart.y()) ;
    QPointF Intermediraire2  (this->pItemDepart->x() + (ESP_H/2.0) + (ESP_H * (pCondition->getValEcartementSortie())), Arrivee.y()) ;

    this->ListePoints.append(Depart);
    this->ListePoints.append(Intermediraire1);
    this->ListePoints.append(Intermediraire2);
    this->ListePoints.append(Arrivee);
}
