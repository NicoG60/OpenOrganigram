//  f_SaisieNom.h      1.0     21/02/14     N.Jarnoux
#ifndef F_SAISIENOM_H
#define F_SAISIENOM_H

class f_SaisieNom ;

//===== Headers standards =====
#include <QDialog>

//=====  Headers  persos  =====

namespace Ui {
class f_SaisieNom;
}

class f_SaisieNom : public QDialog
{
    Q_OBJECT

public:
    explicit    f_SaisieNom(QWidget *  pParent = 0) ;                                   //Constructeur par défaut
                f_SaisieNom(QString sTitre, QString sTexte, QWidget *  pParent = 0) ;   //Constructeur avec des valeur de texte et de titre particulier
                ~f_SaisieNom() ;                                                        //Destructeur

    QString     getSaisie();                                                            //Retourne le nom saisie

private:
    Ui::f_SaisieNom *   ui;
};

#endif // F_SAISIENOM_H
