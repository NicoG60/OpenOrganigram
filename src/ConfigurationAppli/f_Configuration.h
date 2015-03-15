// Configuration.h     1.0     25.02   Jonathan Aubry

#ifndef __CONFIGURATION_H
#define __CONFIGURATION_H


/*
* Déclaration anticipée de la classe
*/
class f_Configuration ;


/*
* Inclusion des fichiers EN-TÊTE
*/
#include <QDialog>


/*
* Déclaration de la classe
*/
namespace Ui {
class f_Configuration ;
}

class f_Configuration : public QDialog
{
    Q_OBJECT


    public :
        // Constructeur
        explicit f_Configuration(QWidget *parent = 0) ;

        // Destructeur
        ~f_Configuration() ;

        // Retourne le QString correspondant au numéro de port
        QString Get_NumeroPort() ;

    protected :


    private :

        Ui::f_Configuration * ui ;

        //Contient le numéro de port enregistrer par l'utilisateur
        QString NumeroPort ;


    private slots :
        // Est réaliser lors de l'appui sur le bouton de rejet
        void on_Bt_Validation_rejected() ;

        // Méthode de validation
        void on_Bt_Validation_accepted();
};

#endif // __CONFIGURATION_H
