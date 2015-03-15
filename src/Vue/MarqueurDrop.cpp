//-------------------------------------------------------------------------------
/**
 * @file        MarqueurDrop.cpp
 * @brief       Représentation d'un élément grpahique
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Représente le marqueur qui se déplacera sur l'organigramme pour indiqué qu'un drop est possible à tel ou tel endroit.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QLinearGradient>
#include <QPainter>

//=====   Headers Peros   =====
#include "MarqueurDrop.h"

/**
 * Constructeur
 * @brief MarqueurDrop::MarqueurDrop
 * @param pParent
 */
MarqueurDrop::MarqueurDrop(QGraphicsItem *  pParent):
    QGraphicsItem(pParent)
{

}

/**
 * Paint l'item sur la vue
 * @brief MarqueurDrop::paint
 * @param painter
 */
void MarqueurDrop::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPointF         PremierPoint    (0, -5) ;
    QPointF         SecondPoint     (0, 5) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0x00, 0xE5, 0x00, 0xFF) ;
    QColor          SecondeCouleur  (0x00, 0xCE, 0x00, 0xFF) ;

    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    QBrush  brush   (grad);

    painter->setBrush(brush) ;

    painter->drawEllipse(this->boundingRect());
}

/**
 * Retourne le rectangle d'affichage
 * @brief MarqueurDrop::boundingRect
 * @return
 */
QRectF MarqueurDrop::boundingRect() const
{
    QRectF  rect    (-5, -5, 10, 10) ;

    return rect ;
}
