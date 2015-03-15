//  f_ModInst_Rout.h    1.0     20/03/14    N. Jarnoux
#ifndef F_MODINST_ROUT_H
#define F_MODINST_ROUT_H

class f_ModInst_Rout ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Rout.h"

namespace Ui {
class f_ModInst_Rout;
}

class f_ModInst_Rout : public QDialog
{
    Q_OBJECT

    public:
        explicit        f_ModInst_Rout(Inst_Rout * pInstructionAModif, QWidget *parent = 0);    //Constructeur
                        ~f_ModInst_Rout();                                                      //Destructeur

            DescRoutine getDescription() ;                                                      //Accesseur de description

    private:
        Inst_Rout *             pInstructionAModif ;    //Instruction à modifier
        ModuleManager *         pManager ;              //Manager de module contenant l'instrction
        Ui::f_ModInst_Rout *    ui;                     //Interface utilisateur

        void    Raffraichir() ;                         //Raffraichit la liste des methodes

    private slots:
        void on_Liste_doubleClicked(const QModelIndex &index);  //Quand on double clic dans la liste
        void on_Btn_Suppr_clicked();                            //Quand on clic sur le bouton supprimer
        void on_Btn_Ajout_clicked();                            //Quand on clic sur le bouton ajouter
};

#endif // F_MODINST_ROUT_H
