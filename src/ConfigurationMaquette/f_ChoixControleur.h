//f_ChoixControler  1.0     14/12/14    N.Jarnoux
#ifndef F_CHOIXCONTROLEUR_H
#define F_CHOIXCONTROLEUR_H

class f_ChoixControleur;

//=== Headers Standars ===
#include <QDialog>
#include <QSettings>

namespace Ui {
class f_ChoixControleur;
}

class f_ChoixControleur : public QDialog
{
    Q_OBJECT

public:
    explicit f_ChoixControleur(QSettings* Config, QWidget *parent = 0); //Constructeur
    ~f_ChoixControleur(); //Destructeur


private slots:
    void on_CB_Controleur_currentIndexChanged(const QString &arg1); //Slot liste controleur dispo

    void on_Btn_Suiv_clicked(); //Click sur le suivant

    void on_LE_NomProjet_textChanged(const QString &arg1); //Nom du projet

private:
    QSettings* Config; //Fichier de config;
    Ui::f_ChoixControleur *ui;
};

#endif // F_CHOIXCONTROLEUR_H
