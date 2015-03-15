//--------------------------------------------------------------------------------------------------
/** @file       f_ChoixModules
 *  @brief      IHM d'ajout et suppressionde modules
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  Classe d'ajout et suppression de modules au sein du projet.
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers Standards ===
#include <QMessageBox>
#include <QDebug>

//==== Headers  persos ====
#include "f_ChoixModules.h"
#include "ui_f_ChoixModules.h"

/**
 * Constructeur, charge les modulen existant en cas de modif d'un plan existant
 * @brief f_ChoixModules::f_ChoixModules
 * @param Config : pointeur vers la config
 * @param parent : widget parent
 */
f_ChoixModules::f_ChoixModules(QSettings *Config, QWidget *parent) :
    QDialog (parent),
    Config  (Config),
    Model   (new QStandardItemModel),
    ui      (new Ui::f_ChoixModules)
{
    //Miste en place de l'interface
    ui->setupUi(this);

    this->ui->listView->setModel(this->Model);

    //Chargement des modules existant
    this->Config->beginGroup("MODULE");
    qDebug() << this->Config->group();
    QStringList ListeClefs  (this->Config->childKeys());
    qDebug() << ListeClefs;

    for(register int i = 0; i < ListeClefs.length(); i++)
    {
        this->Model->appendRow(new QStandardItem(this->Config->value(ListeClefs[i]).toString()));
    }

    this->Config->endGroup();

    this->TesterSuivant();
}

/**
 * Destructeur
 * @brief f_ChoixModules::~f_ChoixModules
 */
f_ChoixModules::~f_ChoixModules()
{
    delete ui;
}

/**
 * Slot quand on click sur ajouter, ajoute un module
 * @brief f_ChoixModules::on_Btn_Ajouter_clicked
 */
void f_ChoixModules::on_Btn_Ajouter_clicked()
{
    QStandardItem* NouvelItem = new QStandardItem("Nouveau Module");
    this->Model->appendRow(NouvelItem);
    this->ui->listView->edit(this->Model->indexFromItem(NouvelItem));

    this->TesterSuivant();
}

/**
 * Slot quand on click sur supprimer : supprimer le module selectionné
 * @brief f_ChoixModules::on_Btn_Suppr_clicked
 */
void f_ChoixModules::on_Btn_Suppr_clicked()
{
    if(QMessageBox::question(this, "Suppression", "Voules-vous supprimer le module sélectionné?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        this->Model->removeRow(this->ui->listView->currentIndex().row());
    }

    this->TesterSuivant();
}

/**
 * Test l'activation de suivant
 * @brief f_ChoixModules::TesterSuivant
 */
void f_ChoixModules::TesterSuivant()
{
    if(this->Model->rowCount() == 0)
    {
        this->ui->Btn_Suiv->setEnabled(false);
    }
    else
    {
        this->ui->Btn_Suiv->setEnabled(true);
    }
}

/**
 * Quand on click sur suivant, Enregistre les modification dans la config
 * @brief f_ChoixModules::on_Btn_Suiv_clicked
 */
void f_ChoixModules::on_Btn_Suiv_clicked()
{
    QList<QStandardItem*> ListeModules  (this->Model->takeColumn(0));

    this->Config->beginGroup("MODULE");
    qDebug() << this->Config->group();

    for(register int i = 0; i < ListeModules.length(); i++)
    {
        QString Clef("m" + QVariant(i+1).toString());

        this->Config->setValue(Clef, ListeModules[i]->text());
        qDebug() << Clef << " : " << ListeModules[i]->text();
    }
    this->Config->endGroup();

    this->accept();
}

/**
 * Quand on click sur precedent
 * @brief f_ChoixModules::on_Btn_Prec_clicked
 */
void f_ChoixModules::on_Btn_Prec_clicked()
{
    this->done(2);
}
