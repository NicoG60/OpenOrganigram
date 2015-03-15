//------------------------------------------------------------------------------
/** @file       f_ModifierProfil.cpp
* @brief        Fichier de gestion de modification d'un profil utilisateur.
*
* @author       AUBRY Jonathan
* @author       STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
* @since        04/04/2014
* @version      1.0
* @date         04/04/2014
*
* Fichier source de la classe f_ModifierProfil, permettant de modifier un profil parmis ceux présent dans le fichier de profil.
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

#include "f_ModifierProfil.h"
#include "ui_f_ModifierProfil.h"


/** Constructeur de la fenêtre de modification du profil utilisateur
* @brief    f_ModifierProfil::f_ModifierProfil()
* @see      f_ModifierProfil
*/
f_ModifierProfil::f_ModifierProfil(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::f_ModifierProfil)
{
    ui->setupUi(this) ;
    ui->Fr_NvMdP->hide() ;

    this->setWindowTitle("Modifier un profil utilisateur") ;
}


/** Destructeur de la fenêtre de modification du profil utilisateur
* @brief    f_ModifierProfil::~f_ModifierProfil()
* @see      f_ModifierProfil
*/
f_ModifierProfil::~f_ModifierProfil()
{
    delete ui;
}


/** Annulation de la modification de profil
* @brief    f_ModifierProfil::on_BtBx_Valider_rejected()
* @see      f_ModifierProfil
*/
void f_ModifierProfil::on_BtBx_Valider_rejected()
{
    f_ModifierProfil::close() ;
}


/** Accepte les modifications de profil
* @brief    f_ModifierProfil::on_BtBx_Valider_accepted()
* @see      f_ModifierProfil
*/
void f_ModifierProfil::on_BtBx_Valider_accepted()
{
    QSettings    Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;
    QCryptographicHash  HashMdP (QCryptographicHash::Md5) ;
    QByteArray          ArrayMdP ;

    if (ui->RBt_NConfig->isChecked())
    {
        Utilisateur.remove(ProfilAModifier + "/Config") ;
        Utilisateur.setValue(ProfilAModifier + "/Config", "0") ;
    }
    else
    {
        Utilisateur.remove(ProfilAModifier + "/Config") ;
        Utilisateur.setValue(ProfilAModifier + "/Config", "1") ;
    }

    if (ui->RBt_NGestionProfil->isChecked())
    {
        Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
        Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "0") ;
    }
    else
    {
        Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
        Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "1") ;
    }

    if (ui->RBt_NProfilDefaut->isChecked())
    {
        Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
        Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "0") ;
    }
    else
    {
        Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
        Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "1") ;
    }

    if (ui->CkBx_NormeOrga->isChecked())
    {
        Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
        Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "1") ;
    }
    else
    {
        Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
        Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "0") ;
    }

    if (ui->LE_AncienMdP->isModified())
    {
        if (ui->RBt_NConfig->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/Config") ;
            Utilisateur.setValue(ProfilAModifier + "/Config", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/Config") ;
            Utilisateur.setValue(ProfilAModifier + "/Config", "1") ;
        }

        if (ui->RBt_NGestionProfil->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
            Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
            Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "1") ;
        }

        if (ui->RBt_NProfilDefaut->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
            Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
            Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "1") ;
        }

        if (ui->CkBx_NormeOrga->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
            Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "1") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
            Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "0") ;
        }

        if (ui->LE_NvMdP->text() == ui->LE_ConfirmMdP->text())
        {
            ArrayMdP = ui->LE_AncienMdP->text().toLocal8Bit() ;
            HashMdP.addData(ArrayMdP) ;

            if (HashMdP.result() == Utilisateur.value(ProfilAModifier+"/MdP"))
            {
                ArrayMdP.remove(0, ArrayMdP.size()) ;
                HashMdP.reset() ;
                ArrayMdP = ui->LE_NvMdP->text().toLocal8Bit() ;
                HashMdP.addData(ArrayMdP) ;

                Utilisateur.remove(ProfilAModifier+"/MdP") ;
                Utilisateur.setValue(ProfilAModifier+"/MdP", HashMdP.result()) ;

                QMessageBox::information(this, "Profil modifié.", "Le profil à été correctement modifié.") ;
                f_ModifierProfil::close() ;
            }
            else
            {
                ui->LE_AncienMdP->clear() ;
                ui->LE_AncienMdP->setStyleSheet("border: 2px solid red") ;

                QMessageBox::critical(this, "Profil à modifié.", "L'ancien mot de passe est incorrect.") ;
            }
        }
        else
        {
            ui->LE_ConfirmMdP->clear() ;
            ui->LE_ConfirmMdP->setStyleSheet("border: 2px solid red") ;
            ui->LE_NvMdP->clear() ;
            ui->LE_NvMdP->setStyleSheet("border: 2px solid red") ;

            QMessageBox::critical(this, "Profil à modifié.", "Les mots de passes sont différents.") ;
        }
    }
    else
    {
        if (ui->RBt_NConfig->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/Config") ;
            Utilisateur.setValue(ProfilAModifier + "/Config", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/Config") ;
            Utilisateur.setValue(ProfilAModifier + "/Config", "1") ;
        }

        if (ui->RBt_NGestionProfil->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
            Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/GestionProfil") ;
            Utilisateur.setValue(ProfilAModifier + "/GestionProfil", "1") ;
        }

        if (ui->RBt_NProfilDefaut->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
            Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "0") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/ProfilDefaut") ;
            Utilisateur.setValue(ProfilAModifier + "/ProfilDefaut", "1") ;
        }

        if (ui->CkBx_NormeOrga->isChecked())
        {
            Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
            Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "1") ;
        }
        else
        {
            Utilisateur.remove(ProfilAModifier + "/NvlleNormeAffichage") ;
            Utilisateur.setValue(ProfilAModifier + "/NvlleNormeAffichage", "0") ;
        }

        QMessageBox::information(this, "Profil modifié.", "Le profil à été correctement modifié.") ;
        f_ModifierProfil::close() ;
    }
}


/** Accesseur d'écriture de l'attribut ProfilAModifier
* @brief    f_ModifierProfil::Set_ProfilAModifier(QString ProfilAModifier)
* @see      f_ModifierProfil
*/
void f_ModifierProfil::Set_ProfilAModifier(QString ProfilAModifier)
{
    this->ProfilAModifier = ProfilAModifier ;
}


/** Paramètre la fenêtre de modification de profil
* @brief    f_ModifierProfil::ModifierProfil()
* @see      f_ModifierProfil
*/
void f_ModifierProfil::ModifierProfil()
{
    QSettings   Utilisateur ("C:\\Users\\Jonathan\\Documents\\Projet\\openorganigram\\Utilisateur.ini", QSettings::IniFormat) ;

    if (Utilisateur.value(ProfilAModifier + "/ProfilDefaut").toString() == "1")
    {
        ui->RBt_OProfilDefaut->setChecked(true) ;
    }
    else
    {
        ui->RBt_OProfilDefaut->setDisabled(true) ;
        ui->RBt_NProfilDefaut->setChecked(true) ;
        ui->RBt_OProfilDefaut->setToolTip("Il existe déjà un profil par défaut !") ;
    }

    if (Utilisateur.value(ProfilAModifier + "/Config").toString() == "1")
    {
        ui->RBt_OConfig->setChecked(true) ;
    }
    else
    {
        ui->RBt_NConfig->setChecked(true) ;
    }

    if (Utilisateur.value(ProfilAModifier + "/GestionProfil").toString() == "1")
    {
        ui->RBt_OGestionProfil->setChecked(true) ;
    }
    else
    {
        ui->RBt_NGestionProfil->setChecked(true) ;
    }

    if (Utilisateur.value(ProfilAModifier + "/NvlleNormeAffichage").toString() == "1")
    {
        ui->CkBx_NormeOrga->setChecked(true) ;
    }
    else
    {
        ui->CkBx_NormeOrga->setChecked(false) ;
    }
}


void f_ModifierProfil::on_LE_AncienMdP_textChanged()
{
    ui->Fr_NvMdP->show() ;
}
