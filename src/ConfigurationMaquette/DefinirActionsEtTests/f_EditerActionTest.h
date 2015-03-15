//f_EditerActionTest.h    1.0     14/12/14    N.Jarnoux
#ifndef F_EDITERACTIONTEST_H
#define F_EDITERACTIONTEST_H

class f_EditerActionTest;

//=== Headers Standards ===
#include <QDialog>

//==== Headers  Persos ====
#include "../../TypeCommun.h"

namespace Ui {
class f_EditerActionTest;
}

class f_EditerActionTest : public QDialog
{
    Q_OBJECT

public:
    explicit f_EditerActionTest(ActionArduino*  Action, QWidget *parent = 0);
    explicit f_EditerActionTest(TestArduino*    Test, QWidget *parent = 0);
    ~f_EditerActionTest();

private slots:
    void on_LE_Nom_textChanged(const QString &arg1);
    void on_LE_Commade_textChanged(const QString &arg1);
    void on_CB_Activer_toggled(bool checked);

private:
    Ui::f_EditerActionTest *ui;
    ActionArduino*  Action;
    TestArduino*    Test;
};

#endif // F_EDITERACTIONTEST_H
