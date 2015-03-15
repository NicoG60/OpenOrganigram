//--------------------------------------------------------------------------------------------------
/** @file       f_EditerActionTest
 *  @brief      IHM d'édition d'une action ou d'un test
 *
 *  @author     N. Jarnoux
 *  @author     BTS IRIS, Lycée Nicolas APPPERT, ORVAULT (FRANCE)
 *  @since      2014-12-14
 *  @version    1.0
 *  @date       2014-12-14
 *
 *  IHM d'édition d'une action ou d'un test
 *
 */
//--------------------------------------------------------------------------------------------------
//==== Headers  Persos ====
#include "f_EditerActionTest.h"
#include "ui_f_EditerActionTest.h"

/**
 * Constructeur pour une action
 * @brief f_EditerActionTest::f_EditerActionTest
 * @param Action
 * @param parent
 */
f_EditerActionTest::f_EditerActionTest(ActionArduino *Action, QWidget *parent) :
    QDialog (parent),
    ui      (new Ui::f_EditerActionTest),
    Action  (Action),
    Test    (0)
{
    this->ui->setupUi(this);

    this->ui->LE_Nom->setText(Action->sNomAction);
    this->ui->LE_Commade->setText(Action->sCommande);
    this->ui->CB_Activer->setChecked(Action->bActive);
}

/**
 * Construteur pour un test
 * @brief f_EditerActionTest::f_EditerActionTest
 * @param Test
 * @param parent
 */
f_EditerActionTest::f_EditerActionTest(TestArduino*    Test, QWidget *parent) :
    QDialog (parent),
    ui      (new Ui::f_EditerActionTest),
    Action  (0),
    Test    (Test)
{
    ui->setupUi(this);

    this->ui->LE_Nom->setText(Test->sNomTest);
    this->ui->LE_Commade->setText(Test->sCommande);
    this->ui->CB_Activer->setChecked(Test->bActive);
}

f_EditerActionTest::~f_EditerActionTest()
{
    delete ui;
}

void f_EditerActionTest::on_LE_Nom_textChanged(const QString &arg1)
{
    if(Test != 0)
        this->Test->sNomTest = arg1;
    else if(Action != 0)
        this->Action->sNomAction = arg1;
}

void f_EditerActionTest::on_LE_Commade_textChanged(const QString &arg1)
{
    if(Test != 0)
        this->Test->sCommande = arg1;
    else if(Action != 0)
        this->Action->sCommande = arg1;
}

void f_EditerActionTest::on_CB_Activer_toggled(bool checked)
{
    if(Test != 0)
        this->Test->bActive = checked;
    else if(Action != 0)
        this->Action->bActive = checked;
}
