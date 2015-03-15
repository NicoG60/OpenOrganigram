//-----------------------------------------------------------------------------------
/** @file       f_SubWindow.cpp
 *  @brief      IHM Définition fenêtres filles
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion des fenêtres filles de la zone MDI
 *
 *  Fabrication
 *
 *  @todo       Il faut coder la classe
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_SubWindow.h"
#include "f_Supervision.h"

//=====   En-Têtes standards    =====
#include <QIcon>



/**  Constructeur par défaut
 *
 **/
f_SubWindow::f_SubWindow(QWidget *parent) :
    QMdiSubWindow(parent)
{
    QPalette ImageDeFond = this->palette() ;

    ImageDeFond.setBrush(QPalette::Window, QBrush(QPixmap(":/Images/Sup/metal-2.bmp"))) ;   //On charge l'image de fond
    this->setPalette(ImageDeFond) ;                                                         //On applique cette image aux subwindow
    this->setWindowIcon(QIcon(NOM_ICONE_MDI)) ;                                             //On applique un icone
    this->setFixedSize(250,250) ;                                                           //On fixe la taille de la subwindow
    this->setWindowFlags(Qt::Tool |Qt::WindowMinimizeButtonHint) ;                          //Affiche la croix et le bouton de diminution
}


/**
 * @param   QCloseEvent* e, va permettre d'ignoner l'action sur la croix
 *
 **/
void f_SubWindow::closeEvent(QCloseEvent* e)
{
    e->ignore() ;                                   // On ignore l'action de la croix
    this->setVisible(false) ;                       // Rend invisible la fenêtre
}


/**
 * @param   Retourne le type de la fenêtre contenue dans la subwindow.
 *
 **/
TypeFenetreSup f_SubWindow::getType()
{
    return this->getWidget()->getType() ;           //On retourne le type de fenêtre
}

/** Permet de remonter l'envoi de commande à f_Supervision
**/
void f_SubWindow::EnvoyerDonneesSubW(QString Commande)
{
    QObject * ZoneMDI (this->parent()->parent()) ;
    qobject_cast<f_Supervision *>(ZoneMDI->parent())->EnvoyerDonneesSup(Commande);
}
