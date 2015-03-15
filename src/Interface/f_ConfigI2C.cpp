#include "f_ConfigI2C.h"
#include "ui_f_ConfigI2C.h"

f_ConfigI2C::f_ConfigI2C(Arduino *pArduino, QWidget *parent) :
    QDialog(parent),
    pArduino(pArduino),
    ui(new Ui::f_ConfigI2C)
{
    ui->setupUi(this);
}

f_ConfigI2C::~f_ConfigI2C()
{
    delete ui;
}

void f_ConfigI2C::on_Btn_Reprogrammer_clicked()
{
    QString Commande("LS");

    if(this->ui->groupBox->isChecked())
    {
        Commande += "1";
    }
    else
    {
        Commande += "0";
    }

    Commande += this->ui->Car->text().rightJustified(2, '0', true);
    Commande += this->ui->Ligne->text().rightJustified(1, '0', true);
    Commande += this->ui->Addr->text().rightJustified(3, '0', true);

    this->pArduino->EnvoyerDonnees(Commande, GEN);
}
