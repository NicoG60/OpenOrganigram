//f_ChoixModules    1.0     14/12/14    N.Jarnoux
#ifndef F_CHOIXMODULES_H
#define F_CHOIXMODULES_H

class f_ChoixModules;

//=== Headers Standards ===
#include <QDialog>
#include <QSettings>
#include <QStandardItemModel>

namespace Ui {
class f_ChoixModules;
}

class f_ChoixModules : public QDialog
{
    Q_OBJECT

public:
    explicit f_ChoixModules(QSettings * Config, QWidget *parent = 0); //Constructeur
    ~f_ChoixModules();  //Destructeur

private slots:
    void on_Btn_Ajouter_clicked();  //Slot quand on click sur "ajouter"
    void on_Btn_Suppr_clicked();    //Slot quand on click sur "supprimer"

    void on_Btn_Suiv_clicked();     //Slot quand on click sur "suivant"

    void on_Btn_Prec_clicked();     //Slot quand on click sur "precedent"

private:
    void                TesterSuivant(); //Test l'activation du bouton suivant

    QSettings*          Config;         //Pointeur vers la conf

    QStandardItemModel* Model;          //Liste des modules
    Ui::f_ChoixModules *ui;             //IHM
};

#endif // F_CHOIXMODULES_H
