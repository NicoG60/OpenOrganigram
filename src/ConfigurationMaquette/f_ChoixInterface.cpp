//--------------------------------------------------------------------------------------------------
/** @file       f_ChoixInterface
 *  @brief      IHM Pour choisir les cartes d'interfaces installée sur le montage
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-14-12
 *  @version    1.0
 *  @date       2014-14-12
 *
 *  Classe d'IHM permettant de configurer les interfaces installée sur la maquette
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers standards ===
#include <QMessageBox>
#include <QDebug>

//==== Headers  persos ====
#include "f_ChoixInterface.h"
#include "ui_f_ChoixInterface.h"

/**
 * Constructeur, Liste les interfaces installable par rapport au controleur choisi
 * @brief f_ChoixInterface::f_ChoixInterface
 * @param Config : Pointeur vers la config
 * @param parent : Widget parent
 */
f_ChoixInterface::f_ChoixInterface(QSettings *Config, QStringList ListeInterfacesInstall, QWidget *parent) :
    QDialog     (parent),
    Config      (Config),
    ModelDispo  (new QStandardItemModel),
    ModelInst   (new QStandardItemModel),
    ui          (new Ui::f_ChoixInterface)
{
    //Mise en place de l'IHM
    ui->setupUi(this);

    //Récuperation des interfaces possible en fonction du controleur choisi
    QString     NomControleur       (this->Config->value("IDENTIFICATION/Board").toString());
    QSettings   ConfControleur      ("./Controleurs/" + NomControleur + "/config.ini", QSettings::IniFormat);

    //Liste des groupes pour detecter s'il s'agit d'une édition de plan existant
    QStringList ListeInterfaceConfig;
    if(ListeInterfacesInstall.isEmpty())
        ListeInterfaceConfig = this->Config->childGroups();
    else
        ListeInterfaceConfig = ListeInterfacesInstall;

    //Si c'est exclusive
    this->Exclusive = ConfControleur.value("Exclusive").toBool();

    //Liste des interfaces installable
    QStringList ListeInterface(ConfControleur.value("Interfaces").toStringList());

    //Boucle de répartition entre les interfaces installée et les interfaces dispos
    for(register int i = 0; i < ListeInterface.length(); i++)
    {
        QStandardItem* NouvelItem(new QStandardItem(ListeInterface[i]));

        bool SortieBoucle(false);

        for(register int j = 0; j < ListeInterfaceConfig.length() && !SortieBoucle; j++)
        {
            QString InterfaceCourante (ListeInterfaceConfig[j].split('-').at(0));

            qDebug() << InterfaceCourante;

            if(InterfaceCourante.toUpper() == ListeInterface[i].toUpper())
            {
                this->ModelInst->appendRow(NouvelItem);
                SortieBoucle = true;
            }
        }

        if(!SortieBoucle)
        {
            this->ModelDispo->appendRow(NouvelItem);
        }
    }

    //Mise en place des models
    this->ui->LV_Dispo->setModel(this->ModelDispo);
    this->ui->LV_Inst->setModel(this->ModelInst);
}

/**
 * Destruteur
 * @brief f_ChoixInterface::~f_ChoixInterface
 */
f_ChoixInterface::~f_ChoixInterface()
{
    delete ModelDispo;
    delete ModelInst;
    delete ui;
}

/**
 * Slot quand on click sur une des insterfaces dans la liste des disponibles, on charge la photos et on active le Btn "installer"
 * @brief f_ChoixInterface::on_LV_Dispo_clicked
 * @param index : index de l'élément clické
 */
void f_ChoixInterface::on_LV_Dispo_clicked(const QModelIndex &index)
{
    QPixmap ImageLabel  ("./Interfaces/" + index.data().toString() + "/photo.png");

    this->ui->LB_Apercu->setPixmap(ImageLabel.scaled(this->ui->LB_Apercu->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    this->ui->Btn_Installer->setEnabled(true);
}

/**
 * Slot quand on click sur installer, fait passer l'interface sélectionnée d'une liste à l'autre.
 * @brief f_ChoixInterface::on_Btn_Installer_clicked
 */
void f_ChoixInterface::on_Btn_Installer_clicked()
{
    QModelIndex ItemSelect   (this->ui->LV_Dispo->currentIndex());

    QStandardItem* DeplacementItem(new QStandardItem(ItemSelect.data().toString()));

    this->ModelDispo->removeRow(ItemSelect.row());
    this->ModelInst->appendRow(DeplacementItem);
}

/**
 * Slot quand on click sur supprimer une interface
 * @brief f_ChoixInterface::on_Btn_supprimer_clicked
 */
void f_ChoixInterface::on_Btn_supprimer_clicked()
{
    QModelIndex ItemSelect   (this->ui->LV_Inst->currentIndex());

    QStandardItem* DeplacementItem(new QStandardItem(ItemSelect.data().toString()));

    this->ModelInst->removeRow(ItemSelect.row());
    this->ModelDispo->appendRow(DeplacementItem);
}

/**
 * Slot quand on selectionne une interface installée
 * @brief f_ChoixInterface::on_LV_Inst_clicked
 */
void f_ChoixInterface::on_LV_Inst_clicked(const QModelIndex &/*index*/)
{
    this->ui->Btn_supprimer->setEnabled(true);
}

/**
 * Slot quand on click sur suivant, Enregistre la liste des interfaces installée dans l'argument qui va bien
 * @brief f_ChoixInterface::on_Btn_Suiv_clicked
 */
void f_ChoixInterface::on_Btn_Suiv_clicked()
{
    this->ListeInterfaceInstall.clear();

    QList<QStandardItem*>   ListeItemInst (this->ModelInst->takeColumn(0));

    for(register int i = 0; i < ListeItemInst.length(); i++)
    {
        ListeInterfaceInstall.append(ListeItemInst[i]->data(Qt::DisplayRole).toString());
    }

    ListeInterfaceInstall.prepend(this->Config->value("IDENTIFICATION/Board").toString());

    this->accept();
}

/**
 * Slot quand on click sur precedent
 * @brief f_ChoixInterface::on_Btn_Prec_clicked
 */
void f_ChoixInterface::on_Btn_Prec_clicked()
{
    this->done(2);
}
