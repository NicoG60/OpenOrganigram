//-----------------------------------------------------------------------------------
/** @file       f_ActionneurG.cpp
 *  @brief      IHM Actionneur Graduation
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion de l'IHM d'un servo moteur
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
#include "f_ActionneurG.h"
#include "ui_f_ActionneurG.h"

//=====   En-Têtes standards    =====
#include <QDebug>

/**  Constructeur par défaut d'un actionneur par graduation
 *
 **/
f_ActionneurG::f_ActionneurG(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_ActionneurG)
{
    this->ui->setupUi(this) ;
    this->ui->LbNomCarteG->setStyleSheet("color:white") ;                                                                //Couleur du texte blanc
    this->ui->LbImageServo->setPixmap(QPixmap("./IOCard/SERVO1/photo.png"));                                             //Image du Servo
    QObject::connect(this->ui->dialValeur, SIGNAL(valueChanged(int)),this->ui->lcdNumberG, SLOT(display(int))) ;         // Change Valeur de l'affichage lorsque la valeur du bouton de graduation est changé
    QObject::connect(this->ui->SBxspinBoxValeur, SIGNAL(valueChanged(int)), this->ui->lcdNumberG, SLOT(display(int))) ;  // Change Valeur de l'affichage lorsque la valeur du la spinBox est changée
    QObject::connect(this->ui->SBxspinBoxValeur, SIGNAL(valueChanged(int)), this->ui->dialValeur, SLOT(setValue(int))) ; // Change la position du bouton de graduation lorsque la SpinBox est changée
    QObject::connect(this->ui->dialValeur,SIGNAL(valueChanged(int)),this->ui->SBxspinBoxValeur,SLOT(setValue(int))) ;    // Change la valeur du spinBox si la valeur du dial change
#ifdef Q_OS_LINUX
    this->ui->LbNomCarteG->setFont(QFont("Comic Sans MS",10)) ;
#endif
}


/**  Destructeur d'un actionneur par graduation
 *
 **/
f_ActionneurG::~f_ActionneurG()
{
    delete ui ;
}


/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_ActionneurG::setTexteTitre(QString Titre)
{
    this->ui->LbNomCarteG->setText(Titre) ;                 //On affiche le descriptif dans le QLabel LbNomCarteG
}


/**         Va permettre de simuler un servo si il y a aucune maquette
 *          Elle retourne un QByteArray simulant une réponse de l'Arduino
 * @param   QString Commande, correspond à la commande envoyée
 **/
QByteArray f_ActionneurG::SimulerCommande(QString Commande)
{
    QByteArray Retour= "";

    this->ui->dialValeur->setValue(Commande.right(3).toInt());

    if(Commande[0] == 'S')
    {
        Retour = "SERVO";

        QString Broche  (Commande.mid(1, 2));

        if(Broche[0] == '0')
        {
            Retour += Broche[1].toLatin1();
        }
        else
        {
            Retour += Broche;
        }

        Retour += "=" + (QVariant(Commande.right(3).toInt()).toString());
    }

    return Retour;
}

/**         Slot Privé : Envoie la commande récupérée dans la SpinBox
 * @param   int arg1, va nous permettre de récupérer la valeur entrée par l'utilisateur
 **/
void f_ActionneurG::on_SBxspinBoxValeur_valueChanged(int arg1)
{
    QString sBroche = this->sCommandeActG ;            //sBroche prend la commande et la valeur de la broche (S + Broche)
    QString sValeur = QString::number(arg1);           //sValeur prend la valeur entrée par l'utilisateur
    if (sValeur.length()==1)                           //Si la valeur entrée par l'utilisateur contient seulement un chiffre
    {
        sValeur="00"+ sValeur ;                        //On ajoute deux 0 pour compléter la commande
    }
    else if (sValeur.length()==2)                      //Si la valeur entrée par l'utilisateur contient seulement deux chiffres
    {
        sValeur="0"+sValeur ;                          //On ajoute un 0 pour compléter la commande
    }
    QString sCommande = sBroche + sValeur ;            //On compose la commande avec la lettre (S) + le numéro de broche + la valeur saisie par l'utilisateur

    do
    {
        this->EnvoyerDonneesW(sCommande);              //Une fois que la taille de la commande est bien de 6 on envoie la commande à l'Arduino

    }while(!sCommande.length()==6) ;
}


/**         Slot Privé : Envoie la commande choisie dans avec le dial
 * @param   int value, va nous permettre de récupérer la valeur du dial
 **/
void f_ActionneurG::on_dialValeur_valueChanged(int value)
{
    QString sBroche = this->sCommandeActG ;             //Même système que le précédent mais avec le dial
    QString sValeur = QString::number(value);
    if (sValeur.length()==1)
    {
        sValeur="00"+ sValeur ;
    }
    else if (sValeur.length()==2)
    {
        sValeur="0"+sValeur ;
    }
    QString sCommande = sBroche + sValeur ;

    do
    {
        this->EnvoyerDonneesW(sCommande);

    }while(!sCommande.length()==6) ;
}


/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_ActionneurG::Commande(QString sCommande)
{
    this->sCommandeActG=sCommande ;
}
