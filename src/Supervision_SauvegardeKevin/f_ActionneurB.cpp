//-----------------------------------------------------------------------------------
/** @file       f_ActionneurB.cpp
 *  @brief      IHM Actionneur Bouton
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion de l'IHM d'un actionneur par action d'un bouton poussoir (LED)
 *
 *  Fabrication
 *
 *  @todo
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_ActionneurB.h"
#include "ui_f_ActionneurB.h"
#include "f_Supervision.h"
#include "./Interface/f_InterpreteurCommandes.h"
#include "./Interface/Arduino.h"

//=====   En-Têtes standards    =====
#include <QCloseEvent>
#include <QDebug>


/**  Constructeur par défaut d'un actionneur par bouton poussoir
 *
 **/
f_ActionneurB::f_ActionneurB(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_ActionneurB)

{
    this->ui->setupUi(this) ;
    this->ui->LbNomCarteB->setStyleSheet("color:white") ;  //Couleur du texte blanc
    //-----Création d'un bouton rond-------------------
    QRegion region(QRect(5,5,80,80),QRegion::Ellipse) ;
    this->ui->BtBoutonAction->setFixedSize(90,90) ;
    this->ui->BtBoutonAction->setMask(region) ;
#ifdef Q_OS_LINUX
    this->ui->LbNomCarteB->setFont(QFont("Comic Sans MS",10)) ;
#endif
}


/**  Destructeur d'un actionneur par bouton poussoir
 *
 **/
f_ActionneurB::~f_ActionneurB()
{
    delete ui ;
}

/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_ActionneurB::setTexteTitre(QString Titre)
{
    this->ui->LbNomCarteB->setText(Titre) ;                                         //On affiche le descriptif dans le QLabel LbNomCarteB
}

/**         Va permettre de simuler l'action sur d'une LED si il y a aucune maquette
 *          Elle retourne un QByteArray simulant une réponse de l'Arduino
 * @param   QString Commande, correspond à la commande envoyée
 **/
QByteArray f_ActionneurB::SimulerCommande(QString Commande)
{
    QByteArray Retour = "";

    if(Commande[3] == '1')                                                          //Si la commande se termine par un 1 (W131)
    {
        this->ui->BtBoutonAction->setChecked(true);                                 //On détecte l'action du bouton
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/Capture.bmp")) ;     //On change l'image du bouton
        this->setEtat(true);                                                        //On appel la fonction setEtat() en indiquant que le bouton est enfoncé
    }
    else if(Commande[3] == '0')                                                     //Si la commande se termine par un 0 (W130)
    {
        this->ui->BtBoutonAction->setChecked(false);                                //On détecte l'action du bouton
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/iconepower.bmp")) ;  //On change l'image du bouton
        this->setEtat(false);                                                       //On appel la fonction setEtat() en indiquant que le bouton est relaché
    }

    Retour = QString("DONE " + Commande).toStdString().c_str();                     //On assemble le retour de commande avec "DONE" avec la commande (Exemple : DONE W131)
    return Retour;                                                                  //On retourne le retour de commande
}

/**  Action de changer l'image du bouton lorsque celui ci est pressé
 *   et d'envoyer la commande à l'Arduino.
 **/
void f_ActionneurB::on_BtBoutonAction_toggled(bool checked)
{
    QString sCommandeAEnvoyer ="" ;                                                 //Déclaration de commande a envoyer
    sCommandeAEnvoyer="W" +this->CommandeEtat.right(2);                             //La commande se compose de "W" et des 2 caractères de droite de CommandeEtat correspondant au numéro de broche

    if(checked)                                                                     //Appui sur le bouton
    {
        this->EnvoyerDonneesW(sCommandeAEnvoyer+"1");                               //On envoie la commande en ajoutant 1 pour allumer la LED
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/Capture.bmp")) ;     //On met l'image correspondante
    }
    else
    {
        this->EnvoyerDonneesW(sCommandeAEnvoyer+"0");                               //On envoie la commande en ajoutant 0 pour éteindre la LED
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/iconepower.bmp")) ;  //On met l'image correspondate
    }
    this->setEtat(checked);                                                         //On appel setEtat() en indiquant l'état
}


/**         Elle récupère la valeur renvoyée par l'Arduino et change l'image de la LED
 *          et du bouton selon la valeur.
 * @param   CommandeRetour, est un QByteArray qui reçoit la valeur renvoyé par l'Arduino.
 **/
void f_ActionneurB::ChangementImage(QByteArray CommandeRetour)
{
    //qDebug () << "f_ActionneurB::RetourValeurLED - CommandeRetour" << CommandeRetour ;
    if (CommandeRetour=="VALUE=1")                                                  //Si le retour de commande est à 1
    {
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/Capture.bmp")) ;     //On met l'image correspondante du bouton indiquant que la LED allumée
        this->setEtat(true);                                                        //On appel setEtat() en indiquant que le LED est allumée
    }
    else if (CommandeRetour=="VALUE=0")
    {
        this->ui->BtBoutonAction->setIcon(QIcon (":/Images/Sup/iconepower.bmp")) ;  //On met l'image correspondante du bouton indiquant que la LED est éteinte
        this->setEtat(false);                                                       //On appel setEtat() en indiquant que la LED est éteinte
    }

}

/**         Elle affiche l'image correspondante à une LED soit allumée soit éteinte
 *@param    bool OnOff, est à true si la LED est allumée et à false si elle est éteinte
 **/
void f_ActionneurB::setEtat(bool OnOff)
{
    if(OnOff)
    {

        this->ui->Lb_ImgActionneurB->setPixmap(QPixmap(this->sChemin+"/photoOn.png")) ; //Photo de LED allumée
    }
    else
    {
        this->ui->Lb_ImgActionneurB->setPixmap(QPixmap(this->sChemin+"/photoOff.png")) ; //Photo de LED éteinte
    }
}

/**         Elle permet d'avoir accès au chemin permettant d'afficher l'image de la LED
 *@param    QString sCheminImage, Contient le chemin de fichier de l'image
 **/
void f_ActionneurB::ImageLabel(QString sCheminImage)
{
    this->sChemin=sCheminImage;                             //L'attribut sChemin récupère le chemin de fichier
    this->setEtat(this->ui->BtBoutonAction->isChecked());   //On appel setEtat() en détectant si le bouton est enfoncé ou relaché
}


/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_ActionneurB::Commande(QString sCommande)
{
    this->CommandeEtat=sCommande ;                          //L'attribut CommandeEtat récupère la commande
}
