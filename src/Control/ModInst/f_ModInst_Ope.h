//  f_ModInst_Ope.h     1.0     04/03/14    N. Jarnoux
#ifndef F_MODINST_OPE_H
#define F_MODINST_OPE_H

class f_ModInst_Ope ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Ope.h"

namespace Ui {
class f_ModInst_Ope;
}

class f_ModInst_Ope : public QDialog
{
    Q_OBJECT

    public:
        explicit                    f_ModInst_Ope(Inst_Ope *   InstructionAModif, QWidget *parent = 0); //Constructeur
                                    ~f_ModInst_Ope();                                                   //Destructeur

                    DescOperation   getDescription() ;                                                  //Accesseur de description
    private:
        Inst_Ope *          InstructionAModif ;                                                         //Instruction à afficher
        Ui::f_ModInst_Ope * ui;                                                                         //Interface utilisateur

};

#endif // F_MODINST_OPE_H
