//-------------------------------------------------------------------------------
/**
 * @file        Item_Cond.cpp
 * @brief       Item de condition
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Element graphique représentant une condition dans l'organigramme
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QGraphicsView>
#include <QRectF>

#include <QDebug>
#include <QMessageBox>

//=====   Headers Peros   =====
#include "Item_Cond.h"

/**
 * Constructeur
 *
 * @brief   Item_Cond::Item_Cond(QGraphicsItem *    pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Cond::Item_Cond(Instruction *   pInst, Item *pItemPrec, QGraphicsItem *    pParent) :
    Item                    (pInst, pItemPrec, pParent),
    nValEcartementSortie    (0)
{
    setTexte("Condition") ;
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_DebFin::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_Cond::paint(QPainter * pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    /*if(this->sTexte.length() > 21)
    {
        this->sTexte = this->sTexte.left(9) + "..." + this->sTexte.right(9);
    }*/

    QFont font;
    font.setPointSizeF(7.5);

    pPainter->setFont(font);

    //Définition du dégradé dans l'item
    qreal           top             (rect.top()) ;
    qreal           left            (rect.left()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0xFE, 0xFE, 0x98, 0xFF) ;
    QColor          SecondeCouleur  (0xC3, 0xC3, 0x67, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur);
    grad.setColorAt(1, SecondeCouleur);

    //Définition du brush pour appliquer le dégradé
    QBrush brush(grad);
    pPainter->setBrush(brush);

    //Définition du polygone
    QPolygonF poly;
    QPointF PointHaut   (0, top)   ;
    QPointF PointGauche (left, 0)  ;
    QPointF PointBas    (0, -top)  ;
    QPointF PointDroite (-left, 0) ;

    poly.append(PointHaut);
    poly.append(PointGauche);
    poly.append(PointBas);
    poly.append(PointDroite);

    //Dessin du polygone

    pPainter->drawPolygon(poly);

    QRectF ZoneTexte(-0.75*(TAILLE_H/2), -0.5*(TAILLE_V/2), 2*(0.75*(TAILLE_H/2)), 2*(0.5*(TAILLE_V/2)));

    pPainter->setPen(QPen());
    pPainter->drawText(ZoneTexte, sTexte, QTextOption(Qt::AlignCenter));

    //Dessin du rond de négation
    qreal   x               (-left - 5) ;
    QBrush  BrushBlanc      (Qt::white) ;
    QPointF CentreCercle    (x, 0) ;

    pPainter->setBrush(BrushBlanc) ;
    pPainter->drawEllipse(CentreCercle, 5, 5);

    pPainter->setFont(QFont("Arial", 8));
    pPainter->drawText(-left - 20, -10, "Faux");
    pPainter->drawText(-25, -top + 10, "Vrai");
}

/**
 * Définit l'écartement à prendre en compte pour la sortie décalée
 * @brief Item_Cond::setValEcartementSortie
 * @param nValEcartementSortie
 */
void Item_Cond::setValEcartementSortie(unsigned int nValEcartementSortie )
{
    if (nValEcartementSortie >= 1)
    {
        this->nValEcartementSortie = nValEcartementSortie ;
    }
}
