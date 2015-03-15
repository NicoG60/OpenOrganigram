#include "f_Moteur.h"
#include "ui_f_Moteur.h"

f_Moteur::f_Moteur(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::f_Moteur)
{
    ui->setupUi(this);
}

f_Moteur::~f_Moteur()
{
    delete ui;
}
