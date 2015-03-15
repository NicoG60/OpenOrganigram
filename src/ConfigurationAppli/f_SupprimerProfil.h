// f_SupprimerProfil.h     1.0     29.03   Jonathan Aubry

#ifndef __SUPPRIMERPROFIL_H
#define __SUPPRIMERPROFIL_H

/*
* Déclaration anticipée de la classe
*/
class f_SupprimerProfil ;


/*
* Inclusion des fichiers EN-TÊTE
*/
#include <QDialog>


/*
* Déclaration de la classe
*/
namespace Ui {
class f_SupprimerProfil ;
}

class f_SupprimerProfil : public QDialog
{
    Q_OBJECT

    public :
        explicit f_SupprimerProfil(QWidget *parent = 0) ;
        ~f_SupprimerProfil() ;

    protected :


    private :
        Ui::f_SupprimerProfil * ui ;


    private slots :
        void    on_Bt_Bx_Valider_accepted();
        void    on_Bt_Bx_Valider_rejected();
};

#endif // __SUPPRIMERPROFIL_H
