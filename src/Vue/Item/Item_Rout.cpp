//-------------------------------------------------------------------------------
/**
 * @file        Item_Rout.cpp
 * @brief       Item de routine
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Element graphique représentant un appel de routine.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout, juste là pour que ça compile
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QBrush>
#include <QPainter>
#include <QLinearGradient>
#include <QGraphicsView>

#include <QDebug>
#include <QMessageBox>

//=====   Headers Peros   =====
#include "Item_Rout.h"

/**
 * Constructeur
 *
 * @brief   Item_Rout::Item_Rout(QGraphicsItem *pParent)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Rout::Item_Rout(Instruction *   pInst, Item *pItemPrec, QGraphicsItem *pParent) :
    Item(pInst, pItemPrec, pParent)
{
    setTexte("Méthode");
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_Rout::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_Rout::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    //Définition du dégradé
    qreal           top             (this->rect.top()) ;
    qreal           left            (this->rect.left()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0xFF, 0xD5, 0x98, 0xFF) ;
    QColor          SecondeCouleur  (0xF3, 0xA6, 0x62, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur);
    grad.setColorAt(1, SecondeCouleur);

    //Définition du brush
    QBrush brush(grad);
    pPainter->setBrush(brush);

    //Dessin de l'item
    QPointF OriginePrem (left*(7.0/8.0), top) ;
    QPointF FinPrem     (left*(7.0/8.0), -top) ;
    QPointF OrigineSec  (-left*(7.0/8.0), top) ;
    QPointF FinSec      (-left*(7.0/8.0), -top) ;
    pPainter->drawRect(rect);
    pPainter->drawLine(OriginePrem, FinPrem);
    pPainter->drawLine(OrigineSec, FinSec);

    pPainter->setPen(QPen());
    pPainter->drawText(rect, sTexte, QTextOption(Qt::AlignCenter));
}
