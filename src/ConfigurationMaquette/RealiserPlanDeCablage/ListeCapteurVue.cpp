//--------------------------------------------------------------------------------------------------
/** @file       ListeCapteurVue
 *  @brief      Heritage d'un liste standard pour gestion de drag'n drop
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe héritant d'un QListView standard pour gerer le drag'n drop
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include "ListeCapteurVue.h"

/**
 * Constructeur, Configure les derniers parametre de la listes
 * @brief ListeCapteurVue::ListeCapteurVue
 * @param parent : widget parent
 */
ListeCapteurVue::ListeCapteurVue(QWidget *parent) :
    QListView(parent)
{
    this->setIconSize(QSize(48, 48));
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/**
 * Evenement d'appuis sur un bouton de souris
 * @brief ListeCapteurVue::mousePressEvent
 * @param pEvent : l'Evenement
 */
void ListeCapteurVue::mousePressEvent(QMouseEvent *   pEvent)
{
    //Si c'est un appuis sur le click gauche, on sauvegarde sa position
    if(pEvent->button() == Qt::LeftButton)
    {
        this->PosDebutDrag = pEvent->pos() ;
    }
    QListView::mousePressEvent(pEvent) ;
}

/**
 * Evenement de déplacement de la souris
 * @brief ListeCapteurVue::mouseMoveEvent
 * @param pEvent : l'evenement
 */
void ListeCapteurVue::mouseMoveEvent(QMouseEvent *    pEvent)
{
    //Calcul les coordonnée du vecteur entre le point d'origine et la position actuelle, puis de sa longueur
    QPoint Vecteur  (pEvent->pos() - this->PosDebutDrag) ;
    int Distance    (Vecteur.manhattanLength()) ;

    //Si la distance est satisfaisante pour commencer un drag
    if(Distance > QApplication::startDragDistance())
    {
        //On commence le drag.
        QDrag* drag = new QDrag(this);

        QStandardItem* ItemDrag (static_cast<QStandardItemModel*>(this->model())->itemFromIndex(this->currentIndex()));
        QSize   TailleImageDrag (32, 32) ;
        QIcon   IconBouton      (ItemDrag->icon()) ;
        QPixmap ImageDrag       (IconBouton.pixmap(TailleImageDrag)) ;

        drag->setPixmap(ImageDrag);
        QMimeData * mime    (new QMimeData);
        mime->setText("Cap " + ItemDrag->data(Qt::UserRole).toString());

        drag->setMimeData(mime);

        drag->exec();
    }
    else
    {
        QListView::mouseMoveEvent(pEvent);
    }
}

