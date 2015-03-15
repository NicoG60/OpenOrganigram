//-------------------------------------------------------------------------------
/**
 * @file        Item_Com.cpp
 * @brief       Item de commentaire
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       22/02/14
 * @version     1.0
 * @date        22/02/14
 *
 * Element graphique représentant un commentaire
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
#include "Item_Com.h"

/**
 * Constructeur
 *
 * @brief   Item_Com::Item_Com(QGraphicsItem * pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Com::Item_Com(Instruction *   pInst, Item *pItemPrec, QGraphicsItem * pParent) :
    Item(pInst, pItemPrec, pParent)
{
    setTexte("Commentaire") ;
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_Com::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_Com::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    QPen PointillePen (pPainter->pen()) ;
    PointillePen.setStyle(Qt::DashLine);
    pPainter->setPen(PointillePen);
    pPainter->drawRect(this->rect);

    pPainter->setPen(QPen());
    pPainter->drawText(this->rect, this->sTexte, QTextOption(Qt::AlignCenter));
}
