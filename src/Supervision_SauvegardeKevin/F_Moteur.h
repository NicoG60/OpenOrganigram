#ifndef F_MOTEUR_H
#define F_MOTEUR_H

class F_Moteur ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"

namespace Ui {
class F_Moteur;
}

class F_Moteur : public f_WidgetInterne
{
    Q_OBJECT

    public:
        explicit F_Moteur       (QWidget *parent = 0);
        inline   TypeFenetreSup getType() { return MOTEUR ; }
        inline   QString        getsBrocheMoteur() { return sBrocheMoteur ; }
                 void           setTexteTitre(QString Titre) ;
                 QByteArray     SimulerCommande(QString Commande);
                 void           ImageLabel(QString sCheminImage);
                 void           DefinirBroche(QString nBroche);
                 void           DefinirTypeMoteur(QString sTypeMot) ;
                                ~F_Moteur();

    private slots:
                 void           on_dial_valueChanged(int value);
                 void           on_BtnDroite_clicked(bool checked);
                 void           on_BtnGauche_clicked(bool checked);
                 void           on_CheckBoxARU_toggled(bool checked);

    private:
                 Ui::F_Moteur * ui;
                 QString        sCommande ;
                 QString        sTypeMoteur ;
                 QString        sBrocheMoteur;
                 QString        sCommandeSauvegarde ;
};

#endif // F_MOTEUR_H
