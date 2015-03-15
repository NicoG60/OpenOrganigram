//-----------------------------------------------------------------------------------
/** @file       f_Telecommande.cpp
 *  @brief      IHM Capteur Numérique
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      26 Avril 2014
 *  @version    1.0
 *  @date       26 Avril 2014
 *
 *  Classe de gestion de l'IHM de la télécommande
 *
 *  Fabrication
 *
 *  @todo       /
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_Telecommande.h"
#include "ui_f_Telecommande.h"

/**  Constructeur par défaut de la classe f_Telecommande
 *
 **/
f_Telecommande::f_Telecommande(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_Telecommande)
{
    ui->setupUi(this);
    this->ui->LbNomTelecommande->setStyleSheet("color:white");

#ifdef Q_OS_LINUX
    this->ui->LbNomTelecommande->setFont(QFont("Comic Sans MS",10)) ;
#endif
}


/**  destructeur par défaut de la classe f_Telecommande
 *
 **/
f_Telecommande::~f_Telecommande()
{
    delete ui;
}

/**  Permet d'interpréter le retour de commande, affichant le bouton enclenché
 * @param CommandeRetour, reçoit le retour de commande de l'Arduino
 **/
void f_Telecommande::ChangementBouton (QByteArray CommandeRetour)
{
    CommandeRetour=CommandeRetour.mid(6);                                                   //On enlève le "VALUE=" pour récupérer seulement la donnée

    this->ui->LbImageTelecommande->setPixmap(QPixmap("./IOCard/IRF1/brutT.png"));
    if(CommandeRetour=="FFA25D")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/onoff.png"));

    }
    else if (CommandeRetour=="FF629D")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/mode.png"));
    }
    else if (CommandeRetour=="FFE21D")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/mute.png"));
    }
    else if (CommandeRetour=="FF22DD")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/backward.png"));
    }
    else if (CommandeRetour=="FF02FD")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/forward.png"));
    }
    else if (CommandeRetour=="FFC23D")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/playpause.png"));
    }
    else if (CommandeRetour=="FFE01F")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/moins.png"));
    }
    else if (CommandeRetour=="FFA857")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/plus.png"));
    }
    else if (CommandeRetour=="FF906F")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/eq.png"));
    }
    else if (CommandeRetour=="FF6897")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/zero.png"));
    }
    else if (CommandeRetour=="FF9867")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/cent.png"));
    }
    else if (CommandeRetour=="FFB04F")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/return.png"));
    }
    else if (CommandeRetour=="FF30CF")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/un.png"));
    }
    else if (CommandeRetour=="FF18E7")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/deux.png"));
    }
    else if (CommandeRetour=="FF7A85")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/trois.png"));
    }
    else if (CommandeRetour=="FF10EF")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/quatre.png"));
    }
    else if (CommandeRetour=="FF38C7")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/cinq.png"));
    }
    else if (CommandeRetour=="FF5AA5")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/six.png"));
    }
    else if (CommandeRetour=="FF42BD")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/sept.png"));
    }
    else if (CommandeRetour=="FF4AB5")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/huit.png"));
    }
    else if (CommandeRetour=="FF52AD")
    {
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap("./IOCard/IRF1/neuf.png"));
    }
    else if (CommandeRetour=="FFFFFF")
    {
    }
    else if (CommandeRetour=="-1")
    {
        this->ui->LbImageTelecommande->setPixmap(QPixmap("./IOCard/IRF1/brut.png"));
        this->ui->LbBoutonTelecommande->setPixmap(QPixmap());
    }
    this->EnvoyerDonneesW("J");
}

/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_Telecommande::setTexteTitre(QString Titre)
{
    this->ui->LbNomTelecommande->setText(Titre);   //On affiche le descriptif dans le QLabel LbNomTelecommande
}


/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_Telecommande::Commande(QString sCommande)
{
    this->CommandeEtat=sCommande ;
}
