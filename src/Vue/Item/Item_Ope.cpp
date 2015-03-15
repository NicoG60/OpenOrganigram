//-------------------------------------------------------------------------------
/**
 * @file        Item_Ope.cpp
 * @brief       Item de d'operation d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Element graphique représentant une opération sur une variable
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout, simplement là pour que ça compile
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
#include "Item_Ope.h"

/**
 * Constructeur
 *
 * @brief   Item_Ope::Item_Ope(QGraphicsItem *  pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Ope::Item_Ope(Instruction *   pInst, Item *pItemPrec, QGraphicsItem *  pParent) :
    Item(pInst, pItemPrec, pParent)
{
    this->setTexte("Opération");
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_Ope::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_Ope::paint(QPainter *pPainter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    //Définition du dégradé dans l'item
    qreal           top             (this->rect.top()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0x98, 0xFE, 0x98, 0xFF) ;
    QColor          SecondeCouleur  (0x62, 0xBA, 0x62, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    //Définition du brush pour appliquer le dégradé
    QBrush brush    (grad) ;

    //Dessin de l'item
    pPainter->setBrush(brush);
    pPainter->drawRect(rect);

    pPainter->setPen(QPen());
    pPainter->drawText(rect, sTexte, QTextOption(Qt::AlignCenter));
}
