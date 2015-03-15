//f_DefinirActionsEtTests.h  1.0    2014-02-21  N.Jarnoux
#ifndef f_DefinirActionsEtTests_H
#define f_DefinirActionsEtTests_H

class f_DefinirActionsEtTests ;

//=== Headers standards ===
#include <QSettings>
#include <QDialog>
#include <QStandardItemModel>

//==== Headers  persos ====
#include "RealiserPlanDeCablage/CapteurActionneur.h"

namespace Ui {
class f_DefinirActionsEtTests ;
}

class f_DefinirActionsEtTests : public QDialog
{
    Q_OBJECT

    public:
            explicit f_DefinirActionsEtTests(CapteurActionneur* Cap, QSettings* Conf, QWidget *parent = 0) ; // Constructeur
            ~f_DefinirActionsEtTests() ;        // Destructeur

private slots:
    void on_Btn_Ok_accepted();

    void on_LE_Nom_textChanged(const QString &arg1);

    void on_CB_Module_currentIndexChanged(const QString &arg1);

    void on_LE_ARU_textChanged(const QString &arg1);

    void on_Btn_Action_clicked();

    void on_Btn_SupprAct_clicked();

    void on_Btn_Test_clicked();

    void on_Btn_SupprTst_clicked();

    void slot_itemActionChanged(QModelIndex index);
    void slot_itemTestChanged(QModelIndex index);

    void on_LV_Action_doubleClicked(const QModelIndex &index);

    void on_LV_Test_doubleClicked(const QModelIndex &index);

private:
            Ui::f_DefinirActionsEtTests *ui ;   // Interface graphique
            CapteurActionneur* Cap;             //Représentation du capteur
            QStandardItemModel* modelAction;    //Modele de la liste d'actions
            QStandardItemModel* modelTest;      //Modele de la liste de tests
};

#endif // f_DefinirActionsEtTests_H
