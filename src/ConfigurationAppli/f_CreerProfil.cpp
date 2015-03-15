//------------------------------------------------------------------------------
/** @file       f_CreerProfil.cpp
* @brief        Fichier de gestion de la création de nouveau profil utilisateur.
*
* @author       AUBRY Jonathan
* @author       STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
* @since        21/02/2014
* @version      1.0
* @date         21/02/2014
*
* Fichier source de la classe f_CreerProfil, permettant de créer un nouveau profil utilisateur et l'inscrivant dans un fichier texte.
*
* Fabrication   OpenOrganigram.pro
*
* @todo         Besoin de créer le fichier texte reçevant les informations sur les profils ainsi que la gestion de création du profil (par défaut, mot de passe..).
*
* @bug          <date du bug> - <CORRIGE> - <Intitulé précis du bug>
*/
//------------------------------------------------------------------------------

#include <QSettings>
#include <QMessageBox>
#include <QCryptographicHash>

#include "f_CreerProfil.h"
#include "ui_f_CreerProfil.h"



/** Constructeur de la fenêtre de création d'un nouveau profil
* @brief    f_CreerProfil::f_CreerProfil()
* @see      f_CreerProfil
*/

f_CreerProfil::f_CreerProfil(QWidget *  parent) :
    QDialog (parent),
    ui      (new Ui::f_CreerProfil)
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QStringList ListeProfil ;
    QString     Keys ;

    ui->setupUi(this) ;
    ui->Lb_NomProfil->setFocus() ;
    ui->Lb_Config->setToolTip("Autorise ou non l'accès aux configurations de l'application.") ;
    ui->Lb_ProfilDefaut->setToolTip("Définit le profil comme profil par défaut de l'application.") ;
    ui->Lb_Gestion->setToolTip("Autorise ou non la création d'un nouveau profil.") ;
    ui->RBt_NConfig->setChecked(true) ;
    ui->RBt_NDefaut->setChecked(true) ;
    ui->RBt_NProfil->setChecked(true) ;

    this->setWindowTitle("Créer un nouveau profil") ;

    ListeProfil = Utilisateur.childGroups() ;

    for (register int i = 0 ; i < ListeProfil.size() ; i++)
    {
        Keys = Utilisateur.value(ListeProfil[i] + "/Defaut").toString() ;

        if (Keys == "1")
        {
            ui->RBt_ODefaut->setDisabled(true) ;
            this->ProfilParDefaut = ListeProfil[i] ;
        }
    }
}


/** Destructeur de la fenêtre de création d'un nouveau profil
* @brief    f_CreerProfil::~f_CreerProfil()
* @see      f_CreerProfil
*/

f_CreerProfil::~f_CreerProfil()
{
    delete  ui ;
}


/** Ajout d'un nouveau profil utilisateur
* @brief    f_CreerProfil::on_BtBx_Valider_accepted()
* @see      f_CreerProfil
*/

void f_CreerProfil::on_BtBx_Valider_accepted()
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QCryptographicHash  HashMDP (QCryptographicHash::Md5) ;
    QByteArray          ArrayHashMDP ;

    if  ( !(ui->LE_Mdp->isModified()) ||
          !(ui->LE_NomProfil->isModified()))
    {
        QMessageBox::critical(this, "Erreur de création.", "Veuillez remplir toutes les informations pour créer un nouveau profil.") ;
    }
    else
    {
        ArrayHashMDP = ui->LE_Mdp->text().toLocal8Bit() ;
        HashMDP.addData(ArrayHashMDP) ;
        Utilisateur.setValue(ui->LE_NomProfil->text() + "/MdP", HashMDP.result()) ;

        if (ui->RBt_NDefaut->isChecked())
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/ProfilDefaut", "0") ;
        }
        else
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/ProfilDefaut", "1") ;
        }

        if (ui->RBt_NConfig->isChecked())
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/Config", "0") ;
        }
        else
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/Config", "1") ;
        }

        if (ui->RBt_NProfil->isChecked())
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/GestionProfil", "0") ;
        }
        else
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/GestionProfil", "1") ;
        }

        if (ui->CkBx_NormeOrga->isChecked())
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/NvlleNormeAffichage", "1") ;
        }
        else
        {
            Utilisateur.setValue(ui->LE_NomProfil->text() + "/NvlleNormeAffichage", "0") ;
        }

        QMessageBox::information(this, "Création d'un nouveau profil.", "Un nouveau profil a été créé avec succès.") ;
    }

    f_CreerProfil::close() ;
}


/** Fermeture de la fenêtre
* @brief    f_CreerProfil::on_BtBx_Valider_rejected()
* @see      f_CreerProfil
*/
void f_CreerProfil::on_BtBx_Valider_rejected()
{
    f_CreerProfil::close() ;
}


/** Retourne l'attribut ProfilParDefaut
* @brief    f_CreerProfil::Get_ProfilParDefaut()
* @see      f_CreerProfil
*/
QString f_CreerProfil::Get_ProfilParDefaut()
{
    return (this->ProfilParDefaut) ;
}
