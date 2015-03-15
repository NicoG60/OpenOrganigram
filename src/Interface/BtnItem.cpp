//-------------------------------------------------------------------------------
/**
 * @file        BtnItem.cpp
 * @brief       Bouton pour inserer des éléments graphiques d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Bouton pour inserer des éléments grpahiques d'organigramme. Ce bouton est hérité du QPushButton
 * classique pour une gestion plus poussée des événements et pour déclencher correctement le drag'n drop.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Définir les MIME Data pour le drag'n drop => Binaire
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QMimeData>
#include <QDrag>
#include <QApplication>

//=====   Headers Peros   =====
#include "BtnItem.h"

/**
 * Constructeur du bouton, construit avec le bon texte et la bonne icône en fonction du type
 *
 * @brief   BtnItem::BtnItem(TypeCommande Type, QWidget *   parent)
 * @param   Type            Type d'item à afficher
 * @param   f_Principale    Pointeur vers la fenetre principale
 * @param   pParent         Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     TypeCommande
 */
BtnItem::BtnItem(TypeCommande Type, f_MainWindow* f_Principale, QWidget *   parent) :
    QPushButton (parent),
    Type        (Type),
    f_Principale(f_Principale)
{
    QString sNomFichier;
    QString sTexte;
    switch(this->Type)
    {
    case OPE:
        sNomFichier = "iconop.png" ;
        sTexte      = "Opération" ;
        break;

    case COND:
        sNomFichier = "iconcond.png";
        sTexte      = "Condition";
        break;

    case ES:
        sNomFichier = "icones.png" ;
        sTexte      = "Entrée/Sortie" ;
        break;

    case ROUT:
        sNomFichier = "iconmeth.png" ;
        sTexte      = "Routine" ;
        break;

    case TEMPO:
        sNomFichier = "icontemp.png" ;
        sTexte      = "Temporisation" ;
        break;

    case COM:
        sNomFichier = "iconcom.png" ;
        sTexte      = "Commentaire" ;
        break;

    case BOUCLE:
        sNomFichier = "iconboucle.png" ;
        sTexte      = "Boucle" ;

    default:
        break;
    }

    QIcon IconItem  (":/Images/Items/" + sNomFichier) ;

    this->setIcon(IconItem);
    this->setIconSize(QSize(24, 24));
    this->setText(sTexte);
    this->setStyleSheet("text-align: left;");
    this->setMinimumSize(32, 32);
    this->setMaximumSize(65535, 32);
}

/**
 * Gestion de l'évenement d'appuis sur le bouton, à chaque clic, on sauvegardera la position de la
 * souris pour déclencher le drag'n drop plus tard
 *
 * @brief   BtnItem::mousePressEvent(QMouseEvent *  pEvent)
 * @param   pEvent  Evenement de souris
 * @see     QMouseEvent
 */
void BtnItem::mousePressEvent(QMouseEvent * pEvent)
{
    if(pEvent->button() == Qt::LeftButton)
    {
        this->PosDebutDrag = pEvent->pos() ;
    }
    QPushButton::mousePressEvent(pEvent) ;

}

/**
 * Gestion de l'évenement de déplacement de la souris avec clic maintenu. On va calculer la distance
 * entre la position du clic et la position actuelle de la souris et on déclenche le drag'n drop
 * si cette distance atteint une certaine valeur
 *
 * @brief   BtnItem::mouseMoveEvent(QMouseEvent *  pEvent)
 * @param   pEvent  Evenement de souris
 * @see     QMouseEvent
 */
void BtnItem::mouseMoveEvent(QMouseEvent *  pEvent)
{
    QPoint Vecteur  (pEvent->pos() - this->PosDebutDrag) ;
    int Distance    (Vecteur.manhattanLength()) ;

    if(Distance > QApplication::startDragDistance())
    {
        QDrag* drag = new QDrag(this);

        QSize   TailleImageDrag (32, 32) ;
        QIcon   IconBouton      (this->icon()) ;
        QPixmap ImageDrag       (IconBouton.pixmap(TailleImageDrag)) ;

        drag->setPixmap(ImageDrag);
        QMimeData * mime    (new QMimeData);
        QVariant    TypeItem((int)this->Type);
        mime->setText(QString("NouvelItem ")+TypeItem.toChar());

        drag->setMimeData(mime);

        drag->exec();
        setDown(false);
    }
    else
    {
        QPushButton::mouseMoveEvent(pEvent);
    }
}

/**
 * Lors du relachement de la souris, on déclenche la fonction de click sur un bouton dans la fenetre principale
 * et on remet le bouton à létat haut
 *
 * @brief BtnItem::mouseReleaseEvent
 * @param pEvent
 */
void BtnItem::mouseReleaseEvent(QMouseEvent * pEvent)
{
    pEvent->accept();
    this->f_Principale->Click_BtnItem(this->Type);
    setDown(false);
}
