//-----------------------------------------------------------------------------------
/** @file       f_LCD.cpp
 *  @brief      IHM Standard
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      24 Mai 2014
 *  @version    1.0
 *  @date       24 Mai 2014
 *
 *  Classe de gestion de l'IHM de l'écran LCD en I2C
 *
 *  Fabrication
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_lcd.h"
#include "ui_f_lcd.h"
#include <QDebug>


/**  Constructeur par défaut de la classe f_LCD
 *
 **/
f_LCD::f_LCD(QWidget *parent) :
    f_WidgetInterne(parent),
    ui(new Ui::f_LCD)
{
    ui->setupUi(this);
    this->ui->Lb_ImgLCD->setPixmap(QPixmap("./IOCard/LCD1/photo.png"));  //Affichage de l'image du LCD
    this->ui->LbNomLCD->setStyleSheet("color:white") ;                   //Texte écrit en blanc pour le nom LCD
    this->ui->SpinBoxLigne->setEnabled(false);      //On désactive la visibilité de la spinBox Ligne
    this->ui->BtnValeurPrecise->setEnabled(false);  //On déactive la visibilité du bouton Positionner
    this->ui->SpinBoxColonne->setEnabled(false);    //On désactive la visibilité de la spinBox Colonne
    this->ui->BtnEnvoyer->setEnabled(true);         //On active la visibilité du bouton Envoyer

#ifdef Q_OS_LINUX
    this->ui->BtnEnvoyer->setFont(QFont("Comic Sans MS",8));
    this->ui->SpinBoxColonne->setGeometry(130,130,35,22);
    this->ui->SpinBoxLigne->setGeometry(180,130,35,22);
    this->ui->BtnValeurPrecise->setFont(QFont("Comic Sans MS",8));
    this->ui->BtnRAZ->setFont(QFont("Comic Sans MS",8));
    this->ui->LbNomLCD->setFont(QFont("Comic Sans MS",10));
#endif

}


/**  Destructeur par défaut de la classe f_LCD
 *
 **/
f_LCD::~f_LCD()
{


    delete ui;
}

/**         Va permettre de modifier le titre par la suite
 * @param   QString Titre, va nous permettre de modifier le titre
 **/
void f_LCD::setTexteTitre(QString Titre)
{
    this->ui->LbNomLCD->setText(Titre);    //On affiche le descriptif dans le QLabel LbNomLCD
}

/**         Va permettre de simuler un LCD si il y a aucune maquette
 *          Elle retourne un QByteArray simulant une réponse de l'Arduino
 * @param   QString Commande, correspond à la commande envoyée
 **/
QByteArray f_LCD::SimulerCommande(QString Commande)
{
    return QByteArray(QString("DONE " + Commande.left(2)).toStdString().c_str()); //On retourne "DONE" + avec les deux premier caractères de la commande
}


/**         Slot Privé : Supprime l'affichage de l'écran
 **/
void f_LCD::on_BtnRAZ_clicked()
{
    this->EnvoyerDonneesW("LC");        //On envoie la commande "LC" pour supprimer l'écran
}


/**         Slot Privé : Envoie la commande contenant le texte saisi par l'utilisateur
 **/
void f_LCD::on_BtnEnvoyer_clicked()
{
    QString sCaractere = this->ui->pTxETexte->toPlainText();    //On récupère dans un QString le texte entré par l'utilisateur
    qDebug () <<"sCaractere" << sCaractere ;
    this->EnvoyerDonneesW("LW"+sCaractere+"|");                 //On envoie "LW" + le texte +"|" au LCD pour afficher le texte
    this->ui->pTxETexte->clear();                               //On efface le qTextEdit
}


/**         Slot Privé : Détecte si la checkBox est préssée ou pas, et affiche selon le cas
 * @param   bool checked, va nous permettre savoir l'état de la checkBox
 **/
void f_LCD::on_CheckBoxPrecis_toggled(bool checked)
{
    if(checked)                                         //Si la checkBox est enfoncé
    {
        this->ui->SpinBoxLigne->setEnabled(true);       //On active la visibilité de la spinBox Ligne
        this->ui->BtnValeurPrecise->setEnabled(true);   //On active la visibilité du bouton Positionner
        this->ui->SpinBoxColonne->setEnabled(true);     //On active la visibilité de la spinBox Colonne
        this->ui->BtnEnvoyer->setEnabled(false);        //On désactive la visibilité du bouton Envoyer
    }
    else
    {
        this->ui->SpinBoxLigne->setEnabled(false);      //On désactive la visibilité de la spinBox Ligne
        this->ui->BtnValeurPrecise->setEnabled(false);  //On déactive la visibilité du bouton Positionner
        this->ui->SpinBoxColonne->setEnabled(false);    //On désactive la visibilité de la spinBox Colonne
        this->ui->BtnEnvoyer->setEnabled(true);         //On active la visibilité du bouton Envoyer
    }
}

/**         Slot Privé : Détecte si le bouton Positionner est appuyé ou pas
 * @param   bool checked, va nous permettre savoir l'état du bouton
 **/
void f_LCD::on_BtnValeurPrecise_clicked(bool checked)
{
    if(!checked)
    {
        if(this->sLigne=="")                                //Si sLigne est vide
        {
            this->sLigne="0";                               //On lui donne la valeur "0"
        }
        QString sCaractere =this->sColonne+this->sLigne;    //Assemble le QString sColonne et sLigne
        this->ui->BtnEnvoyer->setEnabled(true);             //On active la visibilité du bouton Envoyer
        this->EnvoyerDonneesW("LG"+sCaractere);             //On envoie la commande "LG" + colonne + Ligne à l'Arduino pour positionner le curseur
    }
    else
    {
        this->ui->BtnEnvoyer->setEnabled(false);            //On désactive la visibilité du bouton Envoyer
    }
}

/**         Slot Privé : récupère la valeur SpinBoxColonne
 * @param   QString arg1, va nous permettre de savoir la colonne souhaitée
 **/
void f_LCD::on_SpinBoxColonne_valueChanged(QString arg1)
{
    if (arg1.length()==1)   //Si la longueur est de 1 caractère (entre 0 et 9)
    {
        arg1="0"+ arg1 ;    //On ajoute un 0 car la commande sColonne doit contenir deux caractères
    }
    this->sColonne=arg1;    //On donne à sColonne la colonne souhaitée par l'utilisateur
}


/**         Slot Privé : récupère la valeur SpinBoxLigne
 * @param   QString arg1, va nous permettre de savoir la ligne souhaitée
 **/
void f_LCD::on_SpinBoxLigne_valueChanged(QString arg1)
{
    if (arg1.length()==1)   //Si la longueur est de 1 caractère (entre 0 et 9)
    {
        this->sLigne=arg1;  //On donne à sLigne la ligne souhaitée par l'utilisateur
    }
}


/** Description détaillée de la méthode
 * @pre     Elle gère l'action des touches clavier renseignées
 **/
void f_LCD::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
     case Qt::Key_Return:                                               //Bouton Entrée
        this->on_BtnEnvoyer_clicked();                                  //On lance la méthode Bouton_clicked
        break;
    case Qt::Key_Enter :                                                //Bonton Entr
        this->on_BtnEnvoyer_clicked();                                  //On lance la méthode Bouton_clicked
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}
