//------------------------------------------------------------------------------
/** @file       f_SupprimerProfil.cpp
* @brief        Fichier de gestion de suppression d'un profil utilisateur.
*
* @author       AUBRY Jonathan
* @author       STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
* @since        28/03/2014
* @version      1.0
* @date         28/03/2014
*
* Fichier source de la classe f_SupprimerProfil, permettant de supprimer un profil parmis ceux présent dans le fichier de profil.
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
#include <QInputDialog>
#include <QCryptographicHash>

#include "f_SupprimerProfil.h"
#include "ui_f_SupprimerProfil.h"

/** Constructeur de la fenêtre de suppression du profil utilisateur
* @brief    f_SupprimerProfil::f_SupprimerProfil
* @see      f_SupprimerProfil
*/
f_SupprimerProfil::f_SupprimerProfil(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::f_SupprimerProfil)
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QStringList ListeProfil ;

    ListeProfil = Utilisateur.childGroups() ;

    ui->setupUi(this) ;
    ui->LE_MdP->setEchoMode(QLineEdit::Password) ;
    ui->LE_MDPAdmin->setEchoMode(QLineEdit::Password) ;
    ui->Cb_Bx_Profil->addItems(ListeProfil) ;

    this->setWindowTitle("Supprimer un profil utilisateur") ;
}


/** Destructeur de la fenêtre de suppression d'un profil utilisateur
* @brief    f_SupprimerProfil::~f_SupprimerProfil()
* @see      f_SupprimerProfil
*/

f_SupprimerProfil::~f_SupprimerProfil()
{
    delete ui ;
}


/** Slot confirmant la suppression d'un profil utilisateur
* @brief    f_SupprimerProfil::on_Bt_Bx_Valider_accepted()
* @see      f_SupprimerProfil
*/

void f_SupprimerProfil::on_Bt_Bx_Valider_accepted()
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QCryptographicHash  HashMDPAdmin (QCryptographicHash::Md5) ;
    QCryptographicHash  HashMDPProfil (QCryptographicHash::Md5) ;
    QByteArray          ArrayMDPAdmin ;
    QByteArray          ArrayMDPProfil ;

    ArrayMDPAdmin = ui->LE_MDPAdmin->text().toLocal8Bit() ;
    ArrayMDPProfil = ui->LE_MdP->text().toLocal8Bit() ;
    HashMDPAdmin.addData(ArrayMDPAdmin) ;
    HashMDPProfil.addData(ArrayMDPProfil) ;

    if (HashMDPAdmin.result() == Utilisateur.value("Administrateur/MdP") &&
        HashMDPProfil.result() == Utilisateur.value(ui->Cb_Bx_Profil->currentText()+"/MdP"))
    {
        if (QInputDialog::getText(this, "Suppression d'un profil.", "Écrivez SUPPRIMER pour supprimer le profil.") == "SUPPRIMER")
        {
            Utilisateur.remove(ui->Cb_Bx_Profil->currentText()) ;
            QMessageBox::information(this, "Profil supprimé.", "Le profil à été correctement supprimé.") ;
            f_SupprimerProfil::close() ;
        }
        else
        {
            QMessageBox::critical(this, "Suppression annulée.", "Suppression du profil annulée.") ;
            f_SupprimerProfil::close() ;
        }
    }
    else
    {
        if (HashMDPProfil.result() != Utilisateur.value(ui->Cb_Bx_Profil->currentText()+"/MdP"))
        {
            QMessageBox::critical(this, "Suppression annulée.", "Le mot de passe du profil est incorrect.") ;
            ui->LE_MdP->clear() ;
            ui->LE_MdP->setStyleSheet("border: 2px solid red") ;
        }
        else
        {
            QMessageBox::critical(this, "Suppression annulée.", "Le mot de passe admin est incorrect.") ;
            ui->LE_MDPAdmin->clear() ;
            ui->LE_MDPAdmin->setStyleSheet("border: 2px solid red") ;
        }
    }
}


/** Slot de fermeture de la fenêtre
* @brief    f_SupprimerProfil::on_Bt_Bx_Valider_rejected()
* @see      f_SupprimerProfil
*/

void f_SupprimerProfil::on_Bt_Bx_Valider_rejected()
{
    f_SupprimerProfil::close() ;
}
