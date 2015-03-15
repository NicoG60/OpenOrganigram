// f_ChoixProfil.h     1.0     18.03   Jonathan Aubry

#ifndef __ChoixProfil_H
#define __ChoixProfil_H


/*
* Déclaration anticipée de la classe
*/
class f_ChoixProfil ;


/*
* Inclusion des fichiers EN-TÊTE
*/
#include <QDialog>


/*
* Déclaration de la classe
*/
namespace Ui {
class f_ChoixProfil ;
}

class f_ChoixProfil : public QDialog
{
    Q_OBJECT


    public :
        explicit f_ChoixProfil(QWidget *parent = 0) ;
        ~f_ChoixProfil() ;
        QString  Get_ProfilActif() ;

    protected :

    private :
        Ui::f_ChoixProfil * ui ;
        QString ProfilActif ;

    private slots :
        void     on_actionValider_accepted() ;
        void     on_actionValider_rejected() ;

};

#endif // __ChoixProfil_H
