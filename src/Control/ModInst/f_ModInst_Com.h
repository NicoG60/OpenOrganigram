//  f_ModInst_Com.h     1.0     01/04/14    N. Jarnoux
#ifndef F_MODINST_COM_H
#define F_MODINST_COM_H

class f_ModInst_Com ;

//===== Headers standards =====
#include <QDialog>

//=====   Headers Peros   =====
#include "../Inst/Inst_Com.h"

namespace Ui {
class f_ModInst_Com;
}

class f_ModInst_Com : public QDialog
{
    Q_OBJECT

    public:
        explicit    f_ModInst_Com(Inst_Com *  pInstructionAModif, QWidget *parent = 0); //Constructeur
                    ~f_ModInst_Com();                                                   //Destructeur

        DescCommentaire   getDescription() ;                                            //Accesseur de la description

    private:
        Inst_Com *  pInstructionAModif ;                                                //Istruction à afficher
        Ui::f_ModInst_Com *ui;                                                          //Interface Utilisateur
};

#endif // F_MODINST_COM_H
