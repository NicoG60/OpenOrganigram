//  f_ModInst_Tempo.h    1.0     01/04/14    N. Jarnoux
#ifndef F_MODINST_TEMPO_H
#define F_MODINST_TEMPO_H

class f_ModInst_Tempo ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Tempo.h"

namespace Ui {
class f_ModInst_Tempo;
}

class f_ModInst_Tempo : public QDialog
{
    Q_OBJECT

    public:
        explicit    f_ModInst_Tempo(Inst_Tempo *   pInstructionAModifier, QWidget *parent = 0); //Constructeur
                    ~f_ModInst_Tempo();                                                         //Destructeur

        DescTempo   getDescription() ;                                                          //Accesseur description

    private:
        Inst_Tempo * pInstructionAModifier ;    //Instruction à afficher
        Ui::f_ModInst_Tempo *ui;                //Interface utilisateur
};

#endif // F_MODINST_TEMPO_H
