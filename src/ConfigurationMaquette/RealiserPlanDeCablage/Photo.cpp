//--------------------------------------------------------------------------------------------------
/** @file       Photo
 *  @brief      Affichage de la photo de l'interface
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe affichant la photo de l'interface dans le plan de cablage
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QPainter>

//==== Headers  persos ====
#include "Photo.h"

/**
 * Constructeur
 * @brief Photo::Photo
 * @param Chemin : chemin vers la photo
 * @param parent : item parent
 */
Photo::Photo(QString Chemin, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    Chemin(Chemin)
{
}

/**
 * Dessine la photo dans l'éditeur de plan de câblage
 * @brief Photo::paint
 * @param painter
 */
void Photo::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/)
{
    painter->drawPixmap(0, 0, QPixmap(this->Chemin + "/photo.png"));
}

/**
 * Rectangle entourant la photo
 * @brief Photo::boundingRect
 * @return
 */
QRectF Photo::boundingRect() const
{
    return QRectF(0, 0, 621, 485);
}
