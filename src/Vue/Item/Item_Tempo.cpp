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
#include "Item_Tempo.h"

/**
 * Constructeur
 *
 * @brief   Item_Tempo::Item_Tempo(QGraphicsItem * pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Tempo::Item_Tempo(Instruction *   pInst, Item *pItemPrec, QGraphicsItem * pParent) :
    Item(pInst, pItemPrec, pParent)
{
    setTexte("2ms") ;
}

/**
 * Décrit le façon de dessiner l'élément sur la scène
 *
 * @brief   Item_Tempo::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget)
 * @param   pPainter    le pinceau servant à dessiner sur la scène
 * @param   option      Option sur le dessin (pas utile dans notre cas)
 * @param   widget      Pointer vers le widget sur lequel l'item sera peint (pas utile dans notre cas)
 * @see     Item
 */
void Item_Tempo::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;

    //Définition du dégradé
    qreal           top             (this->rect.top()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0x5E, 0xC1, 0xFF, 0xFF) ;
    QColor          SecondeCouleur  (0x22, 0xAA, 0xFF, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    //Définition du brush
    QBrush brush(grad) ;
    pPainter->setBrush(brush) ;

    //Dessin de l'item
    qreal   Rayon           (-top) ;
    QPointF Centre          (0, 0) ;
    QRectF  RectTexte       (top, top, -2*top, -2*top) ;

    pPainter->drawEllipse(Centre, Rayon, Rayon) ;

    pPainter->setPen(QPen());
    pPainter->drawText(RectTexte, sTexte, QTextOption(Qt::AlignCenter)) ;
}
