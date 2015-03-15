// f_CreerProfil.h     1.0     25.02   Jonathan Aubry

#ifndef __CREERPROFIL_H
#define __CREERPROFIL_H


/*
* Déclaration anticipée de la classe
*/
class f_CreerProfil ;


/*
* Inclusion des fichiers EN-TÊTE
*/
#include <QDialog>


/*
* Déclaration de la classe
*/
namespace Ui {
class f_CreerProfil ;
}

class f_CreerProfil : public QDialog
{
    Q_OBJECT


    public :
        explicit f_CreerProfil(QWidget *parent = 0) ;  // Constructeur
        ~f_CreerProfil() ;    // Destructeur
        QString  Get_ProfilParDefaut() ;

    protected :

    private :
        Ui::f_CreerProfil *   ui ;
        QString               ProfilParDefaut ;

    private slots :
        void on_BtBx_Valider_accepted();
        void on_BtBx_Valider_rejected();
};

#endif // __CREERPROFIL_H
