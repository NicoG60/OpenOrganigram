//-----------------------------------------------------------------------------------
/** @file       f_Buzzer.cpp
 *  @brief      IHM Buzzer
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      15 Avril 2014
 *  @version    1.0
 *  @date       30 Avril 2014
 *
 *  Classe de gestion de l'IHM du Buzzer
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
#include "f_Buzzer.h"
#include "ui_f_Buzzer.h"

//=====   En-Têtes standards    =====
#include <QDebug>
#include <QKeyEvent>


/**  Constructeur par défaut de la classe f_Buzzer
 *
 **/
f_Buzzer::f_Buzzer(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_Buzzer),
    sFrequence(""),
    sTemps(""),
    sCommandeBuzzer("")
{
    ui->setupUi(this);
    this->ui->LbNomBuzzer->setStyleSheet("color:white");
    this->ui->LbImageBuzzer->setPixmap(QPixmap("./IOCard/BUZZER1/photo.png"));
#ifdef Q_OS_LINUX
    this->ui->LbNomBuzzer->setFont(QFont("Comic Sans MS",10)) ;
#endif
}


/**  Destructeur de la classe f_Buzzer
 *
 **/
f_Buzzer::~f_Buzzer()
{
    this->sFrequence="" ;
    this->sTemps= "" ;
    this->sCommandeBuzzer= "" ;
    delete ui;
}


/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_Buzzer::setTexteTitre(QString Titre)
{
    this->ui->LbNomBuzzer->setText(Titre);         //On affiche le descriptif dans le QLabel LbNomBuzzer
}



/**         Range la valeur saisie dans la spinbox dans sTemps
 * @param   const QString &arg1, est la valeur en ms que l'utilisateur a entré
 **/
void f_Buzzer::on_SpBTemps_valueChanged(const QString &arg1)
{
    if(arg1.length()==4)            //Si le temps est compris entre 1000 et 9999
    {
      this->sTemps=arg1 ;           //sTemps prend la valeur entrée
    }
    else if (arg1.length()==3)      //Si le temps est compris entre 100 et 999
    {
        this->sTemps="0"+arg1 ;     //sTemps prend la valeur "0" + celle entrée par l'utilisateur
    }
    else if (arg1.length()==2)      //Si le temps est compris entre 10 et 99
    {
        this->sTemps="00"+arg1 ;    //sTemps prend la valeur "00" + celle entrée par l'utilisateur
    }
    else if (arg1.length()==1)      //Si le temps est compris entre 0 et 9
    {
        this->sTemps="000"+arg1 ;   //sTemps prend la valeur "000" + celle entrée par l'utilisateur
    }

}

/**         Range la valeur saisie dans la spinbox dans sFrequence
 * @param   const QString &arg1, est la valeur en Hz que l'utilisateur a entré
 **/
void f_Buzzer::on_SpBFrequence_valueChanged(const QString &arg1)
{
    if(arg1.length()==4)                //Si la fréquence est comprise entre 1000 et 9999
    {
      this->sFrequence=arg1 ;           //sFrequence prend la valeur entrée
    }
    else if (arg1.length()==3)          //Si la fréquence est comprise entre 100 et 999
    {
        this->sFrequence="0"+arg1 ;     //sFrequence prend la valeur "0" + celle entrée par l'utilisateur
    }
    else if (arg1.length()==2)          //Si la fréquence est comprise entre 10 et 99
    {
        this->sFrequence="00"+arg1 ;    //sFrequence prend la valeur "00" + celle entrée par l'utilisateur
    }
    else if (arg1.length()==1)          //Si la fréquence est comprise entre 0 et 9
    {
        this->sFrequence="000"+arg1 ;   //sFrequence prend la valeur "000" + celle entré par l'utilisateur
    }

}

/**      Forme la commande a envoyer en fonction des données entrées par l'utilisateur
 *       et l'envoi à l'Arduino.
 **/
void f_Buzzer::on_BtnJouer_clicked()
{
    QString sCommandeBuz=sFrequence+sTemps ;                    //On forme la commande en associant la fréquence et le temps
    this->EnvoyerDonneesW(this->sCommandeBuzzer+sCommandeBuz);  //On envoie la commande complète (O + Broche + Fréquence + Temps) à l'Arduino
}

/**      Elle gère l'action des touches clavier renseignées
 **/
void f_Buzzer::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Return :                                               //Bouton Entrée
        this->on_BtnJouer_clicked();                                    //On lance la méthode Bouton_clicked
        break;
    case Qt::Key_Enter :                                                //Bonton Entr (Pavée numérique)
        this->on_BtnJouer_clicked();                                    //On lance la méthode Bouton_clicked
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}



/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_Buzzer::Commande(QString sCommande)
{
    this->sCommandeBuzzer=sCommande ;
}
