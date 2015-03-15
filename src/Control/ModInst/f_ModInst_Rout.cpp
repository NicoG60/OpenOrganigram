//-------------------------------------------------------------------------------
/**
 * @file        f_ModInst_Route.cpp
 * @brief       Fenetre de modification d'une insturction
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Classe affichant une interface de modification des propriété d'une instruction "Appel de routine"
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tester si la classe est fonctionnelle
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====

//=====   Headers Peros   =====
#include "f_ModInst_Rout.h"
#include "ui_f_ModInst_Rout.h"
#include "../../Interface/f_SaisieNom.h"

/**
 * Constructeur
 *
 * @brief f_ModInst_Rout::f_ModInst_Rout
 * @param pInstructionAModif
 * @param parent
 */
f_ModInst_Rout::f_ModInst_Rout(Inst_Rout * pInstructionAModif, QWidget *parent) :
    QDialog(parent),
    pInstructionAModif(pInstructionAModif),
    ui(new Ui::f_ModInst_Rout)
{
    ui->setupUi(this);

    QStandardItemModel *    pModel          (new QStandardItemModel);
    this->ui->Liste->setModel(pModel) ;

    this->pManager = this->pInstructionAModif->getRoutine()->getManagerParent() ;

    this->Raffraichir();
}

/**
 * Destructeur
 *
 * @brief f_ModInst_Rout::~f_ModInst_Rout
 */
f_ModInst_Rout::~f_ModInst_Rout()
{
    delete ui;
}

/**
 * Renvois la description de l'appel de routine modifié
 *
 * @brief f_ModInst_Rout::getDescription
 * @return La description
 */
DescRoutine f_ModInst_Rout::getDescription()
{
    DescRoutine Retour ;

    Retour.nIdRoutine   = this->ui->Liste->currentIndex().data(Qt::UserRole).toInt() ;
    Retour.sNom         = this->pManager->getListeRoutines().at(Retour.nIdRoutine)->getNom() ;

    return Retour ;
}

/**
 * Ferme la fenetre en selectionnant automatiquement la routine sur laquelle on a cliqué sauf si c'est la routine principale
 *
 * @brief f_ModInst_Rout::on_Liste_doubleClicked
 * @param index
 */
void f_ModInst_Rout::on_Liste_doubleClicked(const QModelIndex &index)
{
    int ValData (index.data(Qt::UserRole).toInt());

    if(ValData != 0)
    {
        this->accept();
    }
}

/**
 * Supprime la routine sélectionnée
 *
 * @brief f_ModInst_Rout::on_Btn_Suppr_clicked
 */
void f_ModInst_Rout::on_Btn_Suppr_clicked()
{
    int IdRoutine   (this->ui->Liste->currentIndex().data(Qt::UserRole).toInt()) ;

    this->pManager->SupprimerRoutine(IdRoutine);

    this->Raffraichir();
}

/**
 * Ajoute une nouvelle routine
 *
 * @brief f_ModInst_Rout::on_Btn_Ajout_clicked
 */
void f_ModInst_Rout::on_Btn_Ajout_clicked()
{
    f_SaisieNom SaisieNom("Ajout d'une routine", "Nom de la routine", this) ;

    SaisieNom.exec();

    this->pManager->AjouterRoutine(SaisieNom.getSaisie());

    this->Raffraichir();
}

/**
 * Raffraichit la liste des routines
 *
 * @brief f_ModInst_Rout::Raffraichir
 */
void f_ModInst_Rout::Raffraichir()
{
    QList<Routine*>         ListeRoutines   (this->pManager->getListeRoutines()) ;
    QStandardItemModel *    pModele         (qobject_cast<QStandardItemModel*>(this->ui->Liste->model())) ;

    pModele->clear();

    for(register int i = 1; i < ListeRoutines.length(); i++) //On part de 1 pour ne pas afficher la méthode principale
    {
        QStandardItem*  NouvelItem  (new QStandardItem(ListeRoutines[i]->getNom())) ;
        QVariant        UserData    (i);
        NouvelItem->setData(UserData, Qt::UserRole);

        pModele->appendRow(NouvelItem) ;
    }
}
