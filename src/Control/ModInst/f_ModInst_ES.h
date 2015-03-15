//  f_ModInst_ES.h  1.0     20/03/14    N. Jarnoux
#ifndef F_MODINST_ES_H
#define F_MODINST_ES_H

class f_ModInst_ES ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_ES.h"

namespace Ui {
class f_ModInst_ES;
}

class f_ModInst_ES : public QDialog
{
    Q_OBJECT
    
    public:
        explicit    f_ModInst_ES(Inst_ES * pInstructionAModif, QWidget *parent = 0);//Constructeur
                    ~f_ModInst_ES();                                                //Destructeur

        DescES      getDescription() ;                                              //Ascesseur de description

private slots:
        void on_Grp_Liste_toggled(bool arg1);       //Exclusion des deux groupbox
        void on_Grp_Commande_toggled(bool arg1);    //|

private:
        Inst_ES *           pInstructionAModif ;                                    //Instruction représentée
        Ui::f_ModInst_ES *  ui;                                                     //Interface utilisateur
};

#endif // F_MODINST_ES_H
