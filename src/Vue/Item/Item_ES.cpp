//-------------------------------------------------------------------------------
/**
 * @file        Item_ES.cpp
 * @brief       Item de gestion d'entrée/sortie
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        22/02/14
 *
 * Element graphique représentant une action sur une entrée/sortie de la carte
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
#include "Item_ES.h"

/**
 * Constructeur
 *
 * @brief   Item_ES::Item_ES(QGraphicsItem * pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_ES::Item_ES(Instruction *   pInst, Item *pItemPrec, QGraphicsItem * pParent) :
    Item(pInst, pItemPrec, pParent)
{
    setTexte("Entrée/Sortie") ;
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_ES::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_ES::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    //Définition du dégradé
    qreal           top             (this->rect.top()) ;
    qreal           left            (this->rect.left()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0xFE, 0x98, 0x98, 0xFF) ;
    QColor          SecondeCouleur  (0xBA, 0x62, 0x62, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    //Définition du brush
    QBrush brush(grad);
    pPainter->setBrush(brush);

    //Définition du polygone
    QPolygonF   poly ;
    QPointF     Premier     (left*(7.0/8.0), top) ;
    QPointF     Second      (-left, top) ;
    QPointF     Troisieme   (-left*(7.0/8.0), -top) ;
    QPointF     Quatrieme   (left,-top) ;

    poly.append(Premier);
    poly.append(Second);
    poly.append(Troisieme);
    poly.append(Quatrieme);

    //Dessin du poly
    pPainter->drawPolygon(poly);

    pPainter->setPen(QPen());
    pPainter->drawText(rect, sTexte, QTextOption(Qt::AlignCenter));
}
