//-----------------------------------------------------------------------------------
/** @file       f_FenetreDeBase.cpp
 *  @brief      IHM Standard
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      8 Avril 2014
 *  @version    1.0
 *  @date       22 Avril 2014
 *
 *  Classe de gestion de l'IHM d'une fenêtre stantard
 *
 *  Fabrication
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_FenetreDeBase.h"
#include "ui_f_FenetreDeBase.h"
#include <QDebug>


/**  Constructeur par défaut de la classe f_FenetreDeBase
 *
 **/
f_FenetreDeBase::f_FenetreDeBase(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_FenetreDeBase)
{
    this->ui->setupUi(this);
    this->ui->LbNomCarteDeBase->setStyleSheet("color:white") ;  //Texte écrit en blanc pour le nom du capteur
#ifdef Q_OS_LINUX
    this->ui->LbNomCarteDeBase->setFont(QFont("Comic Sans MS",10)) ;
    this->ui->QlEEtatFDB->setFont(QFont("Comic Sans MS",8)) ;
#endif
}


/**  Destructeur de la classe f_FenetreDeBase
 *
 **/
f_FenetreDeBase::~f_FenetreDeBase()
{
    delete ui;
}

/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_FenetreDeBase::setTexteTitre(QString Titre)
{
    this->ui->LbNomCarteDeBase->setText(Titre);       //On affiche le descriptif dans le QLabel LbNomCarteDeBase
}


/**         Elle permet d'avoir accès au chemin permettant d'afficher l'image du bon capteur
 *@param    QString sCheminImage, Contient le chemin de fichier de l'image
 **/
void f_FenetreDeBase::ImageLabel(QString sCheminImage)
{
    if(this->sType=="SONAR")                           //Si c'est capteur ultrason on fait rien
    {

    }
    else
    {
        this->ui->Lb_ImgfenetreDeBase->setPixmap(QPixmap(sCheminImage+"/photo.png"));   //Autrement on affiche l'image du capteur
    }
}

/**         Elle récupère la commande
 *@param    QString sCommande, contient la commande correspondante
 **/
void f_FenetreDeBase::Commande(QString sCommande)
{
    this->CommandeEtat=sCommande;
}

/**         Elle permet de connaître le type de capteur
 *@param    QString sType, contient le type de capteur
 **/
void f_FenetreDeBase::TypeSONAR(QString sType)
{
    this->sType =sType ;                                                                        //On récupère le type SONAR pour l'attribuer à sType
    if(sType=="SONAR")                                                                          //Si c'est bien un SONAR
    {
        this->ui->Lb_ImgfenetreDeBase->setPixmap(QPixmap("./IOCard/SONAR1_ECHO/Sonar.png"));    //On affiche l'image d'un SONAR
        this->ui->QlEEtatFDB->hide();                                                           //On cache la zone de texte
        this->ui->lcdNumberFDB->setGeometry(55,150,130,50);                                     //Positionnement du LCD
        this->ui->TxEValeurDB->setGeometry(55,130,130,18);                                      //Positionnement du textEdit
        this->ui->Lb_ImgfenetreDeBase->setGeometry(77,45,100,82);                               //Positionnement de l'image du SONAR
    #ifdef Q_OS_LINUX
        this->ui->LbNomCarteDeBase->setFont(QFont("Comic Sans MS",9));
    #endif
    }
}


/**         Elle récupère la valeur renvoyée par l'Arduino et affiche le texte en correspondance
 *          selon le capteur.
 * @param   ValeurRetour, est un QByteArray qui reçoit la valeur renvoyé par l'Arduino.
 **/
void f_FenetreDeBase::RetourValeurFDB(QByteArray ValeurRetour)
{
    QString sValeurRetour ="";
    sValeurRetour =ValeurRetour ;                       //On récupère la valeur dans un QString
    sValeurRetour=ValeurRetour.mid(6) ;                 //On sectionne le retour à partir du 6ème caractères
    if(sValeurRetour==this->sCode1)                     //Si la valeur de retour correspond à celle du fichier INI
    {
        this->ui->QlEEtatFDB->setText(this->sTexte1);   //On affiche le texte correspondant à cette valeur
    }
    if(sValeurRetour==this->sCode2)                     //Si la deuxième valeur de retour correspond à celle du fichier INI
    {
        this->ui->QlEEtatFDB->setText(this->sTexte2);   //On affiche le texte correspondant à cette valeur
    }

    this->ui->lcdNumberFDB->display(sValeurRetour);     //On affiche la valeur dans le LCD

}


/**         Elle récupère les deux commandes possibles ainsi que le texte en correspondance à chaque commande.
 *          Exemple pour un fin de course : sCode1=1 assimilé à sTexte1=Fin de course atteint
 *                                          scode2=0 assimilé à sTexte2=Fin de course relaché
 * @param   ValeurRetour, est un QByteArray qui reçoit la valeur renvoyé par l'Arduino.
 **/
void f_FenetreDeBase::Affichage(QString sTexte1, QString sCode1, QString sTexte2, QString sCode2)
{
    this->sTexte1=sTexte1;                                  //L'attribut sTexte1 reçoit le premier texte du fichier INI
    this->sTexte2=sTexte2;                                  //L'attribut sTexte2 reçoit le deuxième texte du fichier INI
    for (register int i =0; i<this->sTexte1.length();i++)   //Pour l'intégralité de sTexte1
    {
        this->sTexte1[i]=this->sTexte1[i].toUpper();        //On met le texte en majuscule
    }
    for (register int i =0; i<this->sTexte2.length();i++)   //Pour l'intégralité de sTexte1
    {
        this->sTexte2[i]=this->sTexte2[i].toUpper();        //On met le texte en majuscule
    }
    this->sCode1=sCode1;                                    //L'attribut sCode1 reçoit le premier code (associé à sTexte1)
    this->sCode2=sCode2;                                    //L'attribut sCode2 reçoit le deuxième code (associé à sTexte2)
}
