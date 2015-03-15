//-------------------------------------------------------------------------------
/**
 * @file        Item_Boucle.cpp
 * @brief       Item de gestion de boucle
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        22/02/14
 *
 * Element graphique représentant une boucle
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
#include "Item_Boucle.h"
#include "../../Control/Inst/Inst_Boucle.h"

/**
 * Constructeur
 *
 * @brief   Item_ES::Item_ES(QGraphicsItem * pParent = 0)
 * @param   pParent     Pointeur vers l'objet QGraphicsItem (ou dérivé) parent.
 * @see     Item
 */
Item_Boucle::Item_Boucle(Instruction *   pInst, Item *pItemPrec, QGraphicsItem * pParent) :
    Item(pInst, pItemPrec, pParent)
{
    setTexte("Boucle") ;
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
void Item_Boucle::paint(QPainter *pPainter, const QStyleOptionGraphicsItem * /*option*/, QWidget * /*widget*/)
{
    this->GererSelection(pPainter) ;
    this->sTexte = this->pInstRepresentee->toString() ;



    //Définition du dégradé
    qreal           top             (this->rect.top()) ;
    qreal           left            (this->rect.left()) ;
    QPointF         PremierPoint    (0, top) ;
    QPointF         SecondPoint     (0, -top) ;
    QLinearGradient grad            (PremierPoint, SecondPoint) ;
    QColor          PremiereCouleur (0xFE, 0xFE, 0x98, 0xFF) ;
    QColor          SecondeCouleur  (0xC3, 0xC3, 0x67, 0xFF) ;
    grad.setColorAt(0, PremiereCouleur) ;
    grad.setColorAt(1, SecondeCouleur) ;

    //Définition du brush
    QBrush brush(grad);
    pPainter->setBrush(brush);

    //Définition du polygone
    QPolygonF   poly ;
    QPointF     Premier ;
    QPointF     Second ;
    QPointF     Troisieme ;
    QPointF     Quatrieme ;
    QPointF     Cinquieme ;
    QPointF     Sixieme ;

    Inst_Boucle* pInst_Boucle   (static_cast<Inst_Boucle *>(this->pInstRepresentee));

    //Dessin du polygon si c'est le premier à être afficher (début de boucle)
    if(pInst_Boucle->EstLePremier())
    {
        Premier.setX(left);
        Premier.setY(-top);

        Second.setX(left);
        Second.setY(0.5*top);

        Troisieme.setX(left - 0.5*top);
        Troisieme.setY(top);

        Quatrieme.setX(-(left - 0.5*top));
        Quatrieme.setY(top);

        Cinquieme.setX(-left);
        Cinquieme.setY(0.5*top);

        Sixieme.setX(-left);
        Sixieme.setY(-top);
    }
    else    //Sinon si c'est le second
    {
        Premier.setX(left);
        Premier.setY(top);

        Second.setX(left);
        Second.setY(-0.5*top);

        Troisieme.setX(left - 0.5*top);
        Troisieme.setY(-top);

        Quatrieme.setX(-(left - 0.5*top));
        Quatrieme.setY(-top);

        Cinquieme.setX(-left);
        Cinquieme.setY(-0.5*top);

        Sixieme.setX(-left);
        Sixieme.setY(top);
    }


    poly.append(Premier);
    poly.append(Second);
    poly.append(Troisieme);
    poly.append(Quatrieme);
    poly.append(Cinquieme);
    poly.append(Sixieme);

    //Dessin du poly
    pPainter->drawPolygon(poly);

    pPainter->setPen(QPen());
    pPainter->drawText(rect, sTexte, QTextOption(Qt::AlignCenter));
}
