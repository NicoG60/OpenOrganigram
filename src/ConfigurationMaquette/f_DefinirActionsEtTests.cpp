//--------------------------------------------------------------------------------------------------
/** @file       f_DefinirActionsEtTests
 *  @brief      IHM Definir les actions et les tests des capteurs et des actionneurs
 *
 *  @author     N.Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-214
 *
 *  Classe de gestion de l'IHM pour définir les actions et les tests des capteurs et des actionneurs
 *  s'ouvrant à la suite d'une connexion sur le plan de câblage entre un capteur et la maquette.
 *
 */
//--------------------------------------------------------------------------------------------------
//=== Headers Standards ===
#include <QDebug>

//===   Headers persos  ===
#include "f_DefinirActionsEtTests.h"
#include "ui_f_DefinirActionsEtTests.h"
#include "DefinirActionsEtTests/ItemActionTest.h"
#include "DefinirActionsEtTests/f_EditerActionTest.h"

/**
 * Constructeur de la fenetre, charge la configuration du capteur
 * @param  Cap : Pointeur vers le capteur à modifier
 * @param  Conf : Pointeur vers le fichier de configuration en cours d'établissement
 * @param  parent : Ouvrir la fenêtre de l'interface graphique
*/
f_DefinirActionsEtTests::f_DefinirActionsEtTests(CapteurActionneur *Cap, QSettings* Conf, QWidget *parent) :
    QDialog     (parent),
    ui          (new Ui::f_DefinirActionsEtTests),
    Cap         (Cap),
    modelAction (new QStandardItemModel),
    modelTest   (new QStandardItemModel)
{
    //Mise en place de l'IHM
    this->ui->setupUi(this) ;

    Conf->beginGroup("MODULE");    //Récuperation de la liste des modules
    qDebug() << Conf->group();
    //Liste les modules
    QStringList ListeModules;
    QStringList ListeKeys(Conf->childKeys());

    for(register int i = 0; i < ListeKeys.length(); i++)
    {
        ListeModules.append(Conf->value(ListeKeys[i]).toString());
    }

    Conf->endGroup();

    //Mise en place de l'interface
    ui->CB_Module->addItems(ListeModules);
    ui->CB_Module->setCurrentText(Cap->getModule());
    ui->LE_Nom->setText(Cap->getTitre());
    ui->LB_Texte->setText(Cap->getNom());
    ui->LE_ARU->setText(Cap->getARU());

    QList<ActionArduino*>    ListeActions    = Cap->getListeActions();
    QList<TestArduino*>      ListeTests      = Cap->getListeTests();

    for(register int i = 0; i < ListeActions.length(); i++)
    {
        modelAction->appendRow(new ItemActionTest(ListeActions[i]));
    }

    for(register int i = 0; i < ListeTests.length(); i++)
    {
        modelTest->appendRow(new ItemActionTest(ListeTests[i]));
    }
    ui->LV_Action->setModel(modelAction);
    ui->LV_Test->setModel(modelTest);
}

/** Description détaillée de la méthode
 *  @pre    f_DefinirActionsEtTests(QWidget *parent) doit être lancée
 *  @post
 *  @param
 *  @retval
 *  @return
 *  @test
 *  @see    ui.
 */
f_DefinirActionsEtTests::~f_DefinirActionsEtTests()
{
    delete this->ui ;
}

void f_DefinirActionsEtTests::on_Btn_Ok_accepted()
{
    accept();
}

void f_DefinirActionsEtTests::on_LE_Nom_textChanged(const QString &arg1)
{
    Cap->setTitre(arg1);
}

void f_DefinirActionsEtTests::on_CB_Module_currentIndexChanged(const QString &arg1)
{
    Cap->setModule(arg1);
}

void f_DefinirActionsEtTests::on_LE_ARU_textChanged(const QString &arg1)
{
    Cap->setARU(arg1);
}

void f_DefinirActionsEtTests::on_Btn_Action_clicked()
{
    ActionArduino* NouvelleAction = new ActionArduino;

    NouvelleAction->bActive = true;

    f_EditerActionTest edit(NouvelleAction, this);

    if(edit.exec() == QDialog::Accepted)
    {
        Cap->AjouterAction(NouvelleAction);
        modelAction->appendRow(new ItemActionTest(NouvelleAction));
    }
}

void f_DefinirActionsEtTests::on_Btn_SupprAct_clicked()
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelAction->itemFromIndex(this->ui->LV_Action->currentIndex()));

    if(item != 0)
    {
        this->modelAction->removeRow(item->row());
        Cap->SupprAction(item->getAction());
    }
}

void f_DefinirActionsEtTests::on_Btn_Test_clicked()
{
    TestArduino* NouveauTest = new TestArduino;

    NouveauTest->bActive = true;

    f_EditerActionTest edit(NouveauTest, this);

    if(edit.exec() == QDialog::Accepted)
    {
        Cap->AjouterTest(NouveauTest);
        modelTest->appendRow(new ItemActionTest(NouveauTest));
    }
}

void f_DefinirActionsEtTests::on_Btn_SupprTst_clicked()
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelTest->itemFromIndex(this->ui->LV_Test->currentIndex()));

    if(item != 0)
    {
        this->modelTest->removeRow(item->row());
        Cap->SupprTest(item->getTest());
    }
}

void f_DefinirActionsEtTests::slot_itemActionChanged(QModelIndex index)
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelAction->itemFromIndex(index));

    item->getAction()->bActive = item->checkState();
}

void f_DefinirActionsEtTests::slot_itemTestChanged(QModelIndex index)
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelTest->itemFromIndex(index));

    item->getAction()->bActive = item->checkState();
}

void f_DefinirActionsEtTests::on_LV_Action_doubleClicked(const QModelIndex &index)
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelAction->itemFromIndex(index));

    if(item != 0)
    {
        f_EditerActionTest f_Modif(item->getAction());

        f_Modif.exec();

        item->refresh();
    }
}

void f_DefinirActionsEtTests::on_LV_Test_doubleClicked(const QModelIndex &index)
{
    ItemActionTest* item = static_cast<ItemActionTest*>(this->modelTest->itemFromIndex(index));

    if(item != 0)
    {
        f_EditerActionTest f_Modif(item->getTest());

        f_Modif.exec();

        item->refresh();
    }
}
