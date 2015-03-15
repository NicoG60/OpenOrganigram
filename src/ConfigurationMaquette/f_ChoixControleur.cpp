//--------------------------------------------------------------------------------------------------
/** @file       f_ChoixControleur
 *  @brief      IHM Pour choisir le type de controleur du montage
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-14-12
 *  @version    1.0
 *  @date       2014-14-12
 *
 *  Classe d'IHM proposant le choix de la carte controleur utilisée dans la maquette.
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>

//==== Headers  persos ====
#include "f_ChoixControleur.h"
#include "ui_f_ChoixControleur.h"

/**
 * Constructeur, Charge la liste des controleurs disponibles et les propose dans l'IHM
 * @brief f_ChoixControleur::f_ChoixControleur
 * @param Config : Le fichier de config à éditer
 * @param parent : widget parent
 */
f_ChoixControleur::f_ChoixControleur(QSettings* Config, QWidget *parent) :
    QDialog (parent),
    Config  (Config),
    ui      (new Ui::f_ChoixControleur)
{
    //Mise en place de l'IHM
    this->ui->setupUi(this);

    //Scan du dossier contenant les controleurs
    QDir DirControl("./Controleurs");
    QFileInfoList ListeDir (DirControl.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot));

    //Listing de son contenu
    for(register int i = 0; i < ListeDir.length(); i++)
    {
        //Si c'est un dossier
        if(ListeDir[i].isDir())
        {
            //On va regarder ce q'il y a dedans
            QDir RepertoireCourant("./Controleurs/" + ListeDir[i].completeBaseName());
            QFileInfoList ListeFichier (RepertoireCourant.entryInfoList());
            for(register int i = 0; i < ListeFichier.length(); i++)
            {
                //Si c'est un fichier et que son extension est .ini
                if(ListeFichier[i].isFile() && ListeFichier[i].suffix() == "ini")
                {
                    //On lit ce fichier et on récupere son nom
                    QSettings Config(ListeFichier[i].filePath(), QSettings::IniFormat);
                    this->ui->CB_Controleur->addItem(Config.value("Nom").toString());
                }
            }
        }
    }

    //On charge la config courante si jamais il s'agit d'une édition de plan existant
    QString ConfigCourante  (this->Config->value("IDENTIFICATION/Board", "").toString());
    if(!ConfigCourante.isEmpty())
    {
        this->ui->CB_Controleur->setCurrentText(ConfigCourante);
    }

    ConfigCourante = this->Config->value("IDENTIFICATION/Nom", "").toString();
    if(!ConfigCourante.isEmpty())
    {
        this->ui->LE_NomProjet->setText(ConfigCourante);
    }

    //Chargement de la photo
    this->ui->LB_Controleur->setPixmap(QPixmap("./Controleurs/" + this->ui->CB_Controleur->currentText() + "/photo.png"));
}

/**
 * Destructeur
 * @brief f_ChoixControleur::~f_ChoixControleur
 */
f_ChoixControleur::~f_ChoixControleur()
{
    delete ui;
}

/**
 * Slot quand on selectionne un controleur dans le menu déroulant, la photo est chargée
 * @brief f_ChoixControleur::on_CB_Controleur_currentIndexChanged
 * @param arg1 : le texte choisi dans la liste
 */
void f_ChoixControleur::on_CB_Controleur_currentIndexChanged(const QString &arg1)
{
    this->ui->LB_Controleur->setPixmap(QPixmap("./Controleurs/" + arg1 + "/photo.png"));
}

/**
 * Quand on click sur le bouton suivant, les modifications sont enregistrées dans la config
 * @brief f_ChoixControleur::on_Btn_Suiv_clicked
 */
void f_ChoixControleur::on_Btn_Suiv_clicked()
{
    if(!this->ui->LE_NomProjet->text().isEmpty())
    {
        this->Config->beginGroup("IDENTIFICATION");
        this->Config->setValue("Id", QVariant("000"));
        this->Config->setValue("Nom", QVariant(this->ui->LE_NomProjet->text()));
        this->Config->setValue("Board", QVariant(this->ui->CB_Controleur->currentText()));
        this->Config->setValue("Version", QVariant(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")));
        this->Config->endGroup();

        this->Config->sync();

        this->accept();
    }
    else
    {
        QMessageBox::information(this, "Erreur", "Merci d'entrer un nom de projet", QMessageBox::Ok);
    }
}

/**
 * Slot quand on écrit dans le champ "nom", s'il est vide, on ne peut pas cliquer sur suivant.
 * @brief f_ChoixControleur::on_LE_NomProjet_textChanged
 * @param arg1 : le texte dans le champ
 */
void f_ChoixControleur::on_LE_NomProjet_textChanged(const QString &arg1)
{
    this->ui->Btn_Suiv->setEnabled(!arg1.isEmpty());
}
