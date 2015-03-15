//------------------------------------------------------------------------------
/** @file       Configuration.cpp
* @brief        Fichier de gestion de la fonction de configuration de l'application.
*
* @author       AUBRY Jonathan
* @author       STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
* @since        21/02/2014
* @version      1.0
* @date         21/02/2014
*
* Fichier source de la classe Configuration. Permet la création de l'interface et la réalisation de différentes tâches comme le changement de mot de passe ou la configuration des paramètres de configuration avec l'arduino.
*
* Fabrication   OpenOrganigram.pro
*
* @todo         Besoin de compléter les derniers champs de l'IHM ainsi que coder la classe Configuration.
*
* @bug          <date du bug> - <CORRIGE> - <Intitulé précis du bug>
*/
//------------------------------------------------------------------------------

#include <QDialog>

#include "f_Configuration.h"
#include "ui_f_Configuration.h"



/** Constructeur de la fenêtre de configuration
* @pre      L'utilisateur ai cliqué sur le menu "Configuration" de la fenêtre principale
* @see      f_Configuration::f_Configuration(), ui, setupUi
*/
f_Configuration::f_Configuration(QWidget *  parent) :
    QDialog (parent),
    ui      (new Ui::f_Configuration)
{
    ui->setupUi(this) ;
}


/** Destructeur de la fenêtre de configuration
* @pre      Que la fenêtre de configuration ai été lancée
* @see      f_Configuration::~f_Configuration(), ui
*/
f_Configuration::~f_Configuration()
{
    delete  ui ;
}


/** Méthode d'annulation
* @pre      Il faut que l'utilisateur soit entrain de configurer les paramètres de l'application
* @see      f_Configuration::on_Bt_Validation_rejected() ;
*/
void f_Configuration::on_Bt_Validation_rejected()
{
    this->close() ;
}


/** Méthode de validation
* @pre      Il faut que l'utilisateur soit entrain de configurer les paramètres de l'application
* @see      f_Configuration::on_Bt_Validation_accepted() ;
*/
void f_Configuration::on_Bt_Validation_accepted()
{
    NumeroPort = ui->LE_PortServeur->text() ;
}


/** Méthode de retour du numéro de port serveur
* @pre      Il faut que l'utilisateur soit entrain de configurer les paramètres de l'application
* @see      f_Configuration::Get_NumeroPort() ;
*/
QString f_Configuration::Get_NumeroPort()
{
    return this->NumeroPort ;
}
