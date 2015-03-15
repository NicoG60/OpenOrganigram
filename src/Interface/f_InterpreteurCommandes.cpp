//-------------------------------------------------------------------------------
/**
 * @file        f_InterpreteurCommandes.cpp
 * @brief       Affichage de toutes les données contenues dans la classe Arduino
 *
 * @author      K.BATARD, N.JARNOUX
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       18/03/14
 * @version     1.0
 * @date        03/03/15
 *
 * Cette classe va permettre d'afficher l'interpréteur de commande avec comme
 * possibilité de lire les données de l'Arduino, d'envoyer des commandes, d'afficher
 * l'état de connection de l'Arduino et son temps d'activité.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------
//=====   Têtes Personnels   =====
#include "f_InterpreteurCommandes.h"
#include "ui_f_InterpreteurCommandes.h"

//===== Têtes standards =====
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QKeyEvent>
#include <QScrollBar>


/**
 *      Constructeur de la classe Lecture
 **/
f_InterpreteurCommandes::f_InterpreteurCommandes(QWidget *parent) :
    QDialog             (parent),
    ui                  (new Ui::f_InterpreteurCommandes),
    indexRetour         (0)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);        //Affiche uniquement le bouton de fermeture
}


/**      Destructeur de la classe Lecture
 *
 **/
f_InterpreteurCommandes::~f_InterpreteurCommandes()
{
    delete ui;
}

/**         Elle permet d'insérer dans la zone de reception des données, les données lues de l'Arduino
 * @param   QByteArray DonneesRecus, contient les données de lecture.
 **/
void f_InterpreteurCommandes::RecevoirDonneesL(QByteArray DonneesRecues)
{
    ui->QBoxReception->insertPlainText(DonneesRecues);      //Affichage des données lues dans la zone de reception
    ui->QBoxReception->insertPlainText("\n");
    ui->QBoxReception->verticalScrollBar()->setSliderPosition(ui->QBoxReception->verticalScrollBar()->maximum());

}


/**      Elle se lance lorsque le bouton "Valider" est préssé
 **/
void f_InterpreteurCommandes::on_BoutonValider_clicked()
{
    QString Caractere = this->ui->QTeEmission->text();      //On stocke dans Caractere, les commandes entrées par l'utilisateur
    this->sSauvegardeCommande.prepend(Caractere);                    //On sauvegarde la commande dans une autre variable
    this->indexRetour = 0;
    emit EnvoyerDonneesL(Caractere, GEN);                  //On émet le signal pour envoyer des données avec en paramètre la commande et l'émetteur
    this->ui->QTeEmission->clear();                         //On efface la zone de texte d'émission
}

/**      Elle gère l'action des touches clavier renseignées
 **/
void f_InterpreteurCommandes::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:                                                    //Bouton Flèche du haut
        if(!this->sSauvegardeCommande.isEmpty())
        {
            this->ui->QTeEmission->setText(this->sSauvegardeCommande[indexRetour]);      //On affiche l'ancienne commande tapée
            if(indexRetour < this->sSauvegardeCommande.size()-1)
                indexRetour++;
        }
        break;
    case Qt::Key_Down:
        if(!this->sSauvegardeCommande.isEmpty())
        {
            this->ui->QTeEmission->setText(this->sSauvegardeCommande[indexRetour]);      //On affiche l'ancienne commande tapée
            if(indexRetour > 0)
                indexRetour--;
        }
        break;
    case Qt::Key_Return:                                                //Bouton Entrée
        this->on_BoutonValider_clicked();                               //On lance la méthode Bouton_clicked
        break;
    case Qt::Key_Enter :                                                //Bonton Entr
        this->on_BoutonValider_clicked();                               //On lance la méthode Bouton_clicked
        break;

    default:
        QWidget::keyPressEvent(event);
    }
}

/**         Elle permet de changer l'état des defines selon l'état de la maquette
 * @param   int nEtat, correspond au defines présent dans TYPECOMMUN_H, QString sCom correspond au numéro de port de l'Arduino
 **/
void f_InterpreteurCommandes::ArduinoConnect(int nEtat, QString sCOM)
{
    switch(nEtat)                                                                   //Selon l'état
    {
    case CONNECTE:                                                                  //Cas où l'Arduino est connecté
        ui->LabelNomPort->setText("ARDUINO PRÉSENT SUR LE PORT " + sCOM);           //On affiche dans l'interpréteur la connectivité de l'Arduino et sur quel port
        this->ui->QTeEmission->setFocus();                                          // Place le curseur immédiatement dans la zone de texte
        this->ui->LabelUptime->show();
        break;

    case DECONNECTE:                                                                //Cas où l'Arduino est déconnecté
        ui->LabelNomPort->setText("ARDUINO NON PRÉSENT");                           //On affiche dans l'interpréteur la non connectivité de l'Arduino
        this->ui->QTeEmission->setFocus();                                          // Place le curseur immédiatement dans la zone de texte
        this->ui->LabelUptime->hide();
        break;

    case PORT_FERME:                                                                //Cas où l'Arduino est connecté mais le port fermé
        ui->LabelNomPort->setText("ARDUINO PRÉSENT MAIS PORT " + sCOM + " FERMÉ");  //On affiche dans l'interpréteur la connectivité de l'Arduino mais avec un port fermé
        this->ui->QTeEmission->setFocus();                                          // Place le curseur immédiatement dans la zone de texte
        this->ui->LabelUptime->show();
        break;
    default:
        break;
    }
}

/**         Elle permet d'afficher le temps de connection de l'Arduino
 * @param   QTime Incrementation,valeur du compteur
 **/
void f_InterpreteurCommandes::Signal_TempsL(QTime Incrementation)
{
    if(Incrementation.isValid())                                                            //Si le temps est valide
    {
        this->ui->LabelUptime->setText("DEPUIS " + Incrementation.toString("hh:mm:ss"));    //On affiche dans l'interpréteur la durée de connectivité
    }
}
