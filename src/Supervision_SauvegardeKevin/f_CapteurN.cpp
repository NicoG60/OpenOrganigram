//-----------------------------------------------------------------------------------
/** @file       f_CapteurN.cpp
 *  @brief      IHM Capteur Numérique
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion de l'IHM d'un capteur numérique
 *
 *  Fabrication
 *
 *  @todo       Ouverture de plusieurs capteurs numériques différents
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_CapteurN.h"
#include "ui_f_CapteurN.h"

//=====   En-Têtes standards    =====
#include <QScrollArea>
#include <QDebug>


/**  Constructeur par défaut d'un bouton poussoir
 *
 **/
f_CapteurN::f_CapteurN(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_CapteurN)
{
    this->ui->setupUi(this) ;
    this->ui->LbNomCarteN->setStyleSheet("color:white") ;           //Texte écrit en blanc pour le nom du capteur
#ifdef Q_OS_LINUX
    this->ui->LbNomCarteN->setFont(QFont("Comic Sans MS",10)) ;
#endif
}


/**  Destructeur d'un capteur numérique
 *
 **/
f_CapteurN::~f_CapteurN()
{
    delete ui ;
}


/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_CapteurN::setTexteTitre(QString Titre)
{
    this->ui->LbNomCarteN->setText(Titre) ;     //On affiche le descriptif dans le QLabel LbNomCarteN
}

/**         Va permettre de simuler l'action d'un bouton poussoir si il y a aucune maquette
 *          Elle retourne un QByteArray simulant une réponse de l'Arduino
 * @param   QString Commande, correspond à la commande envoyée
 **/
QByteArray  f_CapteurN::SimulerCommande(QString)
{
    QByteArray Retour = "";
    if(this->ui->BtnBoutonON->isChecked())          //Si l'image bouton est enfoncé
    {
        Retour = "VALUE=0";                         //On simule un retour "VALUE=0"
    }
    else                                            //Si l'image bouton est relaché
    {
        Retour = "VALUE=1";                         //On simule un retour "VALUE=1"
    }
    return Retour;                                  //On retourne ce résultat
}


/**         Slot Privé : Change l'image du bouton en fonction de son état
 * @param   bool checked, permet de connaître l'état du bouton
 **/
void f_CapteurN::on_BtnBoutonON_toggled(bool checked)
{
    if(checked)                                                         //Si le bouton est enfoncé
    {
        this->ui->BtnBoutonON->setIcon(QIcon (":/Images/Sup/ON.png")) ; //on affiche l'image correspondante
        this->ui->BtnBoutonON->setGeometry(85,110,71,94);
    }
    else                                                                //Si il est relaché
    {
        this->ui->BtnBoutonON->setIcon(QIcon (":/Images/Sup/OFF.png")) ; //on affiche l'image correspondante
        this->ui->BtnBoutonON->setGeometry(85,110,71,100);
    }
}

/**         Gère le changement de l'affichage en fonction de la valeur du capteur
 * @param   QByteArray Etat, retourne la valeur du capteur
 **/
void f_CapteurN::ChangementEtat(QByteArray Etat)
{
    if (Etat=="VALUE=1")                                //Retour de l'Arduino "VALUE=1"
    {
        this->ui->BtnBoutonON->setChecked(false);       //On relache le bouton
    }
    else if (Etat=="VALUE=0")                           //Retour de l'Arduino "VALUE=0"
    {
        this->ui->BtnBoutonON->setChecked(true);        //On enfonce le bouton
    }
}


/**         Elle permet d'avoir accès au chemin permettant d'afficher l'image du bon bouton poussoir
 *@param    QString sCheminImage, Contient le chemin de fichier de l'image
 **/
void f_CapteurN::ImageLabel(QString sCheminImage)
{
    this->ui->Lb_ImgCapteurN->setPixmap(QPixmap(sCheminImage+"/photo.png"));   //Affiche l'image du bouton en fonction du chemin reçu
}


/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_CapteurN::Commande(QString sCommande)
{
    this->CommandeEtat=sCommande ;

}
