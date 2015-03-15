//-----------------------------------------------------------------------------------
/** @file       f_CapteurA.cpp
 *  @brief      IHM capteur analogique
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion de l'IHM d'un capteur analogique
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
#include "f_CapteurA.h"
#include "ui_f_CapteurA.h"

//=====   En-Têtes standards    =====
#include <QDebug>
#include <math.h>


/**  Constructeur par défaut d'un capteur analogique
 *
 **/
f_CapteurA::f_CapteurA(QWidget *parent) : // Constructeur
    f_WidgetInterne(parent),
    ui(new Ui::f_CapteurA),
    nConversion(0)
{
    this->ui->setupUi(this) ;
    this->ui->LbNomCarteA->setStyleSheet("color:white") ;  //Texte de couleur blanche pour le nom du capteur
#ifdef Q_OS_LINUX
    this->ui->LbNomCarteA->setFont(QFont("Comic Sans MS",10)) ;
    this->ui->checkBox->setFont(QFont("Comic Sans MS",7));
    this->ui->checkBox->setGeometry(140,127,95,17);
    this->ui->checkBox->setText("Mode 10 bits");
#endif
}


/**  Destructeur d'un capteur analogique
 *
 **/
f_CapteurA::~f_CapteurA() // Destructeur
{
    this->nConversion=0;
    delete ui ;
}


/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_CapteurA::setTexteTitre(QString Titre)
{
    this->ui->LbNomCarteA->setText(Titre) ;    //On affiche le descriptif dans le QLabel LbNomCarteA
}

/**         Va permettre de simuler un capteur analogique si il y a aucune maquette
 *          Elle retourne un QByteArray simulant une réponse de l'Arduino
 * @param   QString Commande, correspond à la commande envoyée
 **/
QByteArray f_CapteurA::SimulerCommande(QString Commande)
{
    QByteArray Retour   ("VALUE=");                             //Retour de base prend "VALUE=" pour simuler une réponse de l'Arduino
    if(Commande[0] == 'A')                                      //Si le premier caractère correspond à "A" mode 8 bits
    {
        if(this->ui->checkBox->isChecked())                             //Si la checkBox (8/10 bits) est cochée
        {
            Retour += QVariant(this->ui->dial->value() / 2).toString();
        }
        else
        {
            Retour += QVariant(this->ui->dial->value()).toString();
        }
    }
    else if(Commande[0] == 'a')
    {
        if(this->ui->checkBox->isChecked())
        {
            Retour += QVariant(this->ui->dial->value()).toString();
        }
        else
        {
            Retour += QVariant(this->ui->dial->value()*2).toString();
        }
    }

    return Retour;
}

/**         Reçoit la valeur de retour de l'Arduino et gère l'affichage
 * @param   QByteArray ValeurRetour, est la valeur du capteur
 **/
void f_CapteurA::RetourValeurPotan(QByteArray ValeurRetour)
{

    //qDebug () << "f_CapteurA::RetourValeurPotan - ValeurRetour" << ValeurRetour ;
    QString sValeurRetour ="";

    if(ValeurRetour.left(5) == "VALUE")
    {
        sValeurRetour=ValeurRetour.mid(6) ;                 //On sectionne la commande après VALUE=
        this->ui->dial->setValue(sValeurRetour.toInt());    //On met le dial a la valeur de retour
    }
    else
    {
        sValeurRetour =ValeurRetour ;
    }

    if(this->nConversion==1)                                        //Si c'est un capteur de température
    {
        int Valeur =0;
        Valeur =sValeurRetour.toInt() ;                             //Valeur prend la valeur du QString sValeurRetour

        int ValeurFinale    (0) ;
        ValeurFinale = ((-0.525641025*Valeur)+116.0897436) ;        //Formule pour trouver la température

        this->ui->lcdNumberA->display(ValeurFinale) ;               //On affiche la température dans le LCD
        this->ui->lcdNumberA->setGeometry(65,150,90,50);            //Positionnement du LCD
        this->ui->checkBox->hide();                                 //On cache la checkBox du mode 10 bits (uniquement pour potentiomètre)
        this->ui->LbUnite->setText(" EN ° C");                      //On affiche l'unité (° Celcius)
        this->ui->TxEValeurCompteurA->setGeometry(55,123,130,22);   //Positionnement de textEdit
    }
    else if (this->nConversion==2)                                  //Si c'est un capteur de luminosité
    {
        int Valeur =0;
        Valeur =sValeurRetour.toInt() ;                             //Valeur prend la valeur du QString sValeurRetour

        int ValeurFinale =0;
        ValeurFinale =(1517.288685*exp(Valeur*(-0.026954115)));     //Formule pour trouver la luminosité

        this->ui->lcdNumberA->display(ValeurFinale) ;               //On affiche la luminosité dans le LCD
        this->ui->lcdNumberA->setGeometry(65,150,90,50);            //Positionnement du LCD
        this->ui->LbUnite->setText(" EN LUX");                      //On affiche l'unité (LUX)
        this->ui->checkBox->hide();                                 //On cache la checkBox du mode 10 bits (uniquement pour potentiomètre)
        this->ui->TxEValeurCompteurA->setGeometry(55,123,130,22);   //Positionnement du textEdit
    }
    else if (this->nConversion==0)                                  //Si c'est un capteur analogique sans conversion (Ex: Potentiomètre)
    {
        this->ui->TxEValeurCompteurA->setGeometry(10,123,130,22);   //Positionnement du textEdit
        this->ui->lcdNumberA->display(sValeurRetour);               //On affiche la valeur retournée par l'Arduino
        this->ui->lcdNumberA->setGeometry(65,150,90,50);            //Positionnement du LCD
    }
}

/**         Elle permet d'avoir accès au chemin permettant d'afficher l''image du bon capteur analogique
 *@param    QString sCheminImage, Contient le chemin de fichier de l'image
 **/
void f_CapteurA::ImageLabel(QString sCheminImage)
{
    this->ui->Lb_ImgCapteurA->setPixmap(QPixmap(sCheminImage+"/photo.png"));  //Affiche l'image du capteur analogique en fonction du chemin reçu
}


/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_CapteurA::Commande(QString sCommande)
{
    this->CommandeEtat=sCommande;

}

/**         Slot Privé : CheckBox détermine mode de lecture (6 ou 10 bits)
 * @param   bool checked, permet de connaître le mode de lecture
 **/
void f_CapteurA::on_checkBox_toggled(bool checked)
{
    if(checked)                                             //Si on sélectionne la checkbox
    {
        this->CommandeEtat="a"+CommandeEtat.right(2);       //Mode 10 bits, la commande est "a" + Broche
        this->ui->dial->setMaximum(1023);                   //Valeur max 1023
    }
    else
    {
        this->CommandeEtat="A"+CommandeEtat.right(2);       //Mode 8 bits, la commande est "A" + Broche
        this->ui->dial->setMaximum(255);                    //Valeur max 255
    }
}

/**         Permet de savoir le type de capteur analogique
 * @param   int nConvers, permet de récupérer un int qui diférrencie les capteurs analogiques
 **/
void f_CapteurA::Convertir (int nConvers)
{
    this->nConversion=nConvers ;            //nConversion récupère la valeur correspondant au capteur analogique connecté
}


/**         Slot Privé : dial permettant de faire varier la valeur en mode simulation
 * @param   int value, est la valeur choisie par l'utilisateur avec le dial
 **/
void f_CapteurA::on_dial_valueChanged(int value)
{
    this->RetourValeurPotan(QByteArray(QVariant(value).toString().toStdString().c_str())); //On appel RetourValeurPotan() avec la valeur choisie par l'utilisateur à l'aide du dial
}
