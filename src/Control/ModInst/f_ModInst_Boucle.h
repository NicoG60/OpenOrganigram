//  f_ModInst_Boucle.h    1.0     01/04/14    N.Jarnoux
#ifndef F_MODINST_BOUCLE_H
#define F_MODINST_BOUCLE_H

class f_ModInst_Boucle ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Boucle.h"
#include "f_ModInst_Cond.h"

namespace Ui {
class f_ModInst_Boucle;
}

class f_ModInst_Boucle : public QDialog
{
    Q_OBJECT

    public:
        explicit    f_ModInst_Boucle(Inst_Boucle * pInstructionAModifier, QWidget *parent = 0); //Constructeur
                    ~f_ModInst_Boucle();                                                      //Destructeur

        DescBoucle  getDescription() ;                                                      //Accesseur de la description

    private:
        Inst_Boucle *           pInstructionAModifier ;                                         //Instruction à afficher

        f_ModInst_Cond *        pf_ModInst_Cond;                                                //Pointeur vers l'interface de modification d'une condition
        Ui::f_ModInst_Boucle *  ui;                                                             //Interface utilisateur
};

#endif // F_MODINST_BOUCLE_H
