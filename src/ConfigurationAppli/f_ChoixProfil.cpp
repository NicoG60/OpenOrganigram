//------------------------------------------------------------------------------
/** @file       f_ChoixProfil.cpp
* @brief        Fichier de gestion du choix du profil utilisateur.
*
* @author       AUBRY Jonathan
* @author       STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
* @since        18/03/2014
* @version      1.0
* @date         18/03/2014
*
* Fichier source de la classe f_ChoixProfil, permettant de choisir un profil parmis ceux présent dans le fichier de profil.
*
* Fabrication   OpenOrganigram.pro
*
* @todo         Besoin de vérifier si le fichier d'utilisateur existe, puis lire à l'intérieur et y trouver les différents profils existant.
*
* @bug          <date du bug> - <CORRIGE> - <Intitulé précis du bug>
*/
//------------------------------------------------------------------------------
#include <QSettings>
#include <QMessageBox>
#include <QCryptographicHash>

#include "f_MainWindow.h"
#include "f_ChoixProfil.h"
#include "ui_f_ChoixProfil.h"


/** Constructeur de la fenêtre de choix du profil utilisateur
* @brief    f_ChoixProfil::f_ChoixProfil()
* @see      f_ChoixProfil
*/
f_ChoixProfil::f_ChoixProfil(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::f_ChoixProfil)
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QStringList ListeProfil ;

    ui->setupUi(this) ;
    this->setWindowTitle("Choix du profil utilisateur") ;

    ui->LE_MdP->setEchoMode(QLineEdit::Password) ;
    ui->Cb_Bx_Profil->setToolTip("Pour sélectionner le profil Eleve, vous pouvez ne pas rentrer son mot de passe.") ;

    ListeProfil = Utilisateur.childGroups() ;
    ui->Cb_Bx_Profil->addItems(ListeProfil) ;
}


/** Destructeur de la fenêtre de choix du profil utilisateur
* @brief    f_ChoixProfil::~f_ChoixProfil()
* @see      f_ChoixProfil
*/
f_ChoixProfil::~f_ChoixProfil()
{
    delete  ui ;
}


/** Validation du choix de profil utilisateur
* @brief    f_ChoixProfil::on_actionValider_accepted()
* @see      f_ChoixProfil
*/
void f_ChoixProfil::on_actionValider_accepted()
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QCryptographicHash  HashMDP (QCryptographicHash::Md5) ;
    QByteArray          ArrayHashMDP ;

    ArrayHashMDP = ui->LE_MdP->text().toLocal8Bit() ;
    HashMDP.addData(ArrayHashMDP) ;

    if (HashMDP.result() != Utilisateur.value(ui->Cb_Bx_Profil->currentText()+"/MdP"))
    {
        QMessageBox::critical(this, "Erreur !", "Mot de passe incorrect.") ;
        ui->LE_MdP->clear() ;
        ui->Lb_MdP->setStyleSheet("border: 2px solid red") ;
    }
    else
    {
        this->ProfilActif = ui->Cb_Bx_Profil->currentText() ;

        QMessageBox::information(this, "Changement de profil.", "Vous êtes maintenant connecté en tant que : " + this->ProfilActif) ;
        emit accept() ;
        f_ChoixProfil::close() ;
    }
}


/** Fermeture de la fenêtre de choix de profil
* @brief    f_ChoixProfil::on_actionValider_rejected()
* @see      f_ChoixProfil
*/
void f_ChoixProfil::on_actionValider_rejected()
{
    f_ChoixProfil::close() ;
}


/** Accesseur de lecture de l'attribut ProfilActif
* @brief    f_ChoixProfil::Get_ProfilActif()
* @see      f_ChoixProfil
*/
QString f_ChoixProfil::Get_ProfilActif()
{
    return (this->ProfilActif) ;
}
