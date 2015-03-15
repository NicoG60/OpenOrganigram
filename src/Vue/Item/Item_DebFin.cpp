//-------------------------------------------------------------------------------
/**
 * @file        Item_DebFin.cpp
 * @brief       Item de début ou de fin d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Element graphique représentant le début ou la fin d'un organigramme.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Nothing
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
#include "Item_DebFin.h"

/**
 * Constructeur
 *
 * @brief   Item_DebFin::Item_DebFin(bool bDebut = true, QGraphicsItem *    pParent = 0)
 * @param   bDebut      Indique si c'est le début ou la fin.
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_DebFin::Item_DebFin(Instruction *   pInst, Item *pItemPrec, QGraphicsItem *    pParent) :
    Item(pInst, pItemPrec, pParent)
{
    this->setTexte(pInstRepresentee->toString());
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
void Item_DebFin::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    //Définition du dégradé dans l'item
    qreal           top             (this->rect.top()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0xDC, 0xA5, 0xFF, 0xFF) ;
    QColor          SecondeCouleur  (0xD0, 0x86, 0xFE, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    //Définition du brush pour appliquer le dégradé
    QBrush  brush   (grad) ;

    //Dessin de l'item
    pPainter->setBrush(brush) ;
    pPainter->drawRoundedRect(rect, -top, -top) ;

    pPainter->setPen(QPen());
    pPainter->drawText(rect, sTexte, QTextOption(Qt::AlignCenter)) ;
}

/**
 * Ignore l'événement de menu contextuel car on ne peut pas modifier cet élément
 *
 * @brief   Item_DebFin::contextMenuEvent(QGraphicsSceneContextMenuEvent *   pEvent)
 * @param   pEvent    L'événement concerné
 * @see     Item, Item::contextMenuEvent(QGraphicsSceneContextMenuEvent *   pEvent)
 */
void Item_DebFin::contextMenuEvent(QGraphicsSceneContextMenuEvent *   pEvent)
{
    pEvent->ignore();
}

/**
 * Ignore l'événement de double clic car on ne peut pas modifier cet élément
 *
 * @brief   Item_DebFin::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *    pEvent)
 * @param   pEvent    L'événement concerné
 * @see     Item, Item::mouseDoubleClickEvent(QGraphicsSceneContextMenuEvent *   pEvent)
 */
void Item_DebFin::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *    pEvent)
{
    pEvent->ignore();
}
