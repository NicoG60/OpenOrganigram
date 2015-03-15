// f_ModInst_Cond.h     1.0     19/03/14  N. Jarnoux
#ifndef F_MODINST_COND_H
#define F_MODINST_COND_H

class f_ModInst_Cond ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Cond.h"

namespace Ui {
class f_ModInst_Cond ;
}

class f_ModInst_Cond : public QDialog
{
    Q_OBJECT

    public:
        explicit        f_ModInst_Cond(Inst_Cond * InstructionAModif, QWidget *parent = 0); //Constructeur Normal
                        f_ModInst_Cond(DescCondition Description, ModuleManager *Manager, QWidget * parent = 0);    //Constructeur avec description (pour les boucle)
                        ~f_ModInst_Cond();                                                  //Destructeur

        DescCondition   getDescription() ;                                                  //Accesseur de description

private slots:
        void on_Grp_CondLog_toggled(bool arg1);

        void on_Grp_CondMat_toggled(bool arg1);

private:
        Inst_Cond *             InstructionAModif ;                                         //Instruction à afficher
        ModuleManager *         Manager ;
        Ui::f_ModInst_Cond *    ui;                                                         //Interface utilisateur
};

#endif // F_MODINST_COND_H
