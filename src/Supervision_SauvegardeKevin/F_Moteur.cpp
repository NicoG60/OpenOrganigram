//-----------------------------------------------------------------------------------
/** @file       f_Moteur.cpp
 *  @brief      IHM Moteur PWM et DIR
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      21 Mai 2014
 *  @version    1.0
 *  @date       21 Mai 2014
 *
 *  Classe de gestion de l'IHM d'un moteur PWM ou DIR
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
#include "F_Moteur.h"
#include "ui_F_Moteur.h"

//=====   En-Têtes standards    =====
#include <QDebug>

F_Moteur::F_Moteur(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::F_Moteur)
{
    ui->setupUi(this);
    this->ui->LbNomMoteur->setStyleSheet("color:white") ;                                                  //Texte écrit en blanc pour le nom du moteur
    QObject::connect(this->ui->dial, SIGNAL(valueChanged(int)), this->ui->lcdNumber, SLOT(display(int))) ; // Change Valeur de l'affichage lorsque la valeur du bouton de graduation est changé
#ifdef Q_OS_LINUX
    this->ui->LbNomMoteur->setFont(QFont("Comic Sans MS",9)) ;
    this->ui->CheckBoxARU->setFont(QFont("Comic Sans MS",8)) ;
    this->ui->LbNomMoteur->setGeometry(-20,20,290,26) ;
#endif
}

F_Moteur::~F_Moteur()
{
    delete ui;
}

/**      Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void F_Moteur::setTexteTitre(QString Titre)
{
    this->ui->LbNomMoteur->setText(Titre) ;    //On affiche le descriptif dans le QLabel LbNomMoteur
}

QByteArray F_Moteur::SimulerCommande(QString Commande)
{
    QByteArray Retour="";

    if(this->sTypeMoteur == "MOT1_DIR" || this->sTypeMoteur == "MOT2_DIR" || this->sTypeMoteur == "MOTEUR")
    {

    }
    else if(this->sTypeMoteur == "MOT1_PWM" || this->sTypeMoteur == "MOT2_PWM" || this->sTypeMoteur == "MOTEUR")
    {
        if(Commande[0] == 'M')
        {
            this->ui->lcdNumber->display(Commande.right(3));
            this->ui->dial->setValue(Commande.right(3).toInt());
            Retour = QString("DONE " + Commande).toStdString().c_str();
        }
    }

    return Retour;
}


/**         Elle permet d'avoir accès au chemin permettant d'afficher l'image du bon moteur
 *@param    QString sCheminImage, Contient le chemin de fichier de l'image
 **/
void F_Moteur::ImageLabel(QString sCheminImage)
{
    this->ui->Lb_ImgMoteur->setPixmap(QPixmap(sCheminImage+"/photo.png")); //On affiche la bonne image de moteur selon chemin reçu en paramètre
}


/**         Slot Privé : Récupérer vitesse du moteur choisi avec le dial
 * @param   int value, va nous permettre de récupérer la vitesse entrée par l'utilisateur
 **/
void F_Moteur::on_dial_valueChanged(int value)
{
    QString sBroche = "M"+this->sBrocheMoteur.left(2) ;     //SBroche se compose de "M" + le numéro de broche
    QString sValeur = QString::number(value);               //On ajoute la vitesse sélectionnée dans un QString
    if (sValeur.length()==1)                                //Si compris entre 0 et 9
    {
        sValeur="00"+ sValeur ;                             //On ajoute "00" avant la valeur afin de faire 3 caractères
    }
    else if (sValeur.length()==2)                           //Si compris entre 10 et 99
    {
        sValeur="0"+sValeur ;                               //On ajoute "0" avant la valeur afin de faire 3 caractères
    }
    QString sCommande = sBroche + sValeur ;                 //On assemble "M" + broche + vitesse
    qDebug () << "Test" << sCommande ;
    do
    {
        this->EnvoyerDonneesW(sCommande);                   //On envoie la commande à l'Arduino lorsque la taille est bien égale à 6 caractères
        this->sCommandeSauvegarde=sCommande;                //On sauvegarde la dernière commande

    }while(!sCommande.length()==6) ;
}


/**         Elle permet d'avoir accès au numéro de broche du moteur
 *@param    QString sBroche, Contient le numéro de broche
 **/
void F_Moteur::DefinirBroche(QString sBroche)
{
    this->sBrocheMoteur=sBroche ;                                       //On récupère les numéros de broches

}

/**         Elle permet de définir le type de moteur
 *@param    QString sTypeMot, Contient le chemin de fichier de l'image
 **/
void F_Moteur::DefinirTypeMoteur(QString sTypeMot)
{
    this->sTypeMoteur=sTypeMot ;                                        //On indique le type de moteur à sTypeMoteur
    if (this->sTypeMoteur=="MOT1_DIR" || this->sTypeMoteur=="MOT2_DIR")
    {
        this->ui->dial->hide();                                         //On cache le dial
        this->ui->lcdNumber->setGeometry(50,150,131,41);                //Positionnement du LCD
        this->ui->TxEValeurMot->setGeometry(50,130,131,18);             //Positionnement du qTextEdit
    }
    else if (this->sTypeMoteur=="MOT1_PWM" || this->sTypeMoteur=="MOT2_PWM")
    {
        this->ui->BtnDroite->hide();                                    //On cache le bouton directionnel droit
        this->ui->BtnGauche->hide();                                    //On cache le bouton directionnel gauche
        this->ui->dial->setGeometry(10,120,70,70);                      //Positionnement du dial
        this->ui->lcdNumber->setGeometry(95,155,131,41);                //Positionnement du LCD
        this->ui->TxEValeurMot->setGeometry(95,135,131,18);             //Positionnement du qTextEdit
    }
    else
    {
        this->ui->lcdNumber->setGeometry(50,150,131,41);                //Positionnement du LCD
        this->ui->TxEValeurMot->setGeometry(50,130,131,18);             //Positionnement du qTextEdit
        this->ui->Lb_ImgMoteur->setGeometry(25,45,102,69);              //Positionnement de l'image
        this->ui->dial->setGeometry(160,47,65,65);                      //Positionnement du dial
        this->ui->CheckBoxARU->setGeometry(60,192,121,17);              //Positionnement de la checkBox
        this->ui->CheckBoxARU->show();                                  //On affiche la checkBox

    }
}


/**         Slot Privé : Permet de changer de sens à l'aide d'un bouton
 * @param   bool checked, va nous permettre de récupérer l'état du bouton
 **/
void F_Moteur::on_BtnDroite_clicked(bool checked)
{
    if(!checked)
    {
        QString sCommDroite = "W" + this->sBrocheMoteur.right(2) ;      //On ajoute à "W" les deux dernier caractères
        this->EnvoyerDonneesW(sCommDroite+"1");                         //On envoie la commande pour modifier le sens
    }
}


/**         Slot Privé : Permet de changer de sens à l'aide d'un bouton
 * @param   bool checked, va nous permettre de récupérer l'état du bouton
 **/
void F_Moteur::on_BtnGauche_clicked(bool checked)
{
    if(!checked)
    {
        QString sCommGauche = "W" + this->sBrocheMoteur.right(2) ;      //On ajoute à "W" les deux dernier caractères
        this->EnvoyerDonneesW(sCommGauche+"0");                         //On envoie la commande pour modifier le sens
    }
}

/**         Slot Privé : Arrêt d'urgence à partir de la checkBox
 * @param   bool checked, va nous permettre de récupérer l'état de la checkBox
 **/
void F_Moteur::on_CheckBoxARU_toggled(bool checked)
{
    if(checked)
    {
        this->EnvoyerDonneesW("M"+this->sBrocheMoteur.left(2)+"000");   //Commande d'arrêt d'urgence
        this->ui->dial->setEnabled(false);                              //On rend invisible le dial
        this->ui->BtnDroite->setEnabled(false);                         //On rend invisible le bouton de droit
        this->ui->BtnGauche->setEnabled(false);                         //On rend invisible le bouton de gauche
    }
    else
    {
        this->EnvoyerDonneesW(this->sCommandeSauvegarde);               //On envoie la dernière commande sauvegardée
        this->ui->dial->setEnabled(true);                               //On rend visible le dial
        this->ui->BtnDroite->setEnabled(true);                          //On rend visible le bouton de droit
        this->ui->BtnGauche->setEnabled(true);                          //On rend visible le bouton de gauche
    }
}
