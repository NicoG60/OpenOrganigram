// f_ChoixInterface 1.0     14/12/14    N.Jarnoux
#ifndef F_CHOIXINTERFACE_H
#define F_CHOIXINTERFACE_H

class f_ChoixInterface;

//=== Headers standards ===
#include <QDialog>
#include <QSettings>
#include <QStandardItem>

namespace Ui {
class f_ChoixInterface;
}

class f_ChoixInterface : public QDialog
{
    Q_OBJECT

public:
    explicit f_ChoixInterface(QSettings* Config, QStringList ListeInterfacesInstall, QWidget *parent = 0); //Contructeur
    ~f_ChoixInterface();    //Destructeur

    inline QStringList  getListeInterfaceInstall() { return ListeInterfaceInstall ; } //Accesseur de la liste des interfaces installées

private slots:
    void on_LV_Dispo_clicked(const QModelIndex &index); //Slot quand on click sur une interface dispo

    void on_Btn_Installer_clicked();                    //Slot quand on click sur "installer"

    void on_Btn_supprimer_clicked();                    //Slot quand on click sur "supprimer"

    void on_LV_Inst_clicked(const QModelIndex &);       //Slot quand on click sur une interface installée

    void on_Btn_Suiv_clicked();                         //Slot quand on click sur suivant

    void on_Btn_Prec_clicked();                         //Slot quand on click sur précédent

private:

    QSettings*              Config;                 //Pointeur vers le fichier de config
    bool                    Exclusive;              //Si on ne peut installer qu'une seule instance d'une interface
    QStandardItemModel*     ModelDispo;             //Model d'interface dispo
    QStandardItemModel*     ModelInst;              //Model d'interface installée
    QStringList             ListeInterfaceInstall;  //Liste des interfaces installée
    Ui::f_ChoixInterface *  ui;                     //IHM
};

#endif // F_CHOIXINTERFACE_H
