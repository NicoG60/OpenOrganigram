#ifndef F_ACTIONNEURB_H
#define F_ACTIONNEURB_H

class f_ActionneurB ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"


namespace Ui {
class f_ActionneurB ;
}

class f_ActionneurB : public f_WidgetInterne
{
    Q_OBJECT

    public :
            explicit        f_ActionneurB(QWidget *parent = 0) ;
            void            setTexteTitre(QString Titre) ;
            QByteArray      SimulerCommande(QString Commande);
            void            setEtat(bool OnOff) ;
            void            ChangementImage (QByteArray CommandeRetour) ;
            void            ImageLabel(QString sCheminImage) ;
            void            Commande(QString sCommande) ;
     inline TypeFenetreSup  getType() { return ACT_B ; }
                            ~f_ActionneurB() ;


    private slots :
            void            on_BtBoutonAction_toggled(bool checked);

    protected :
            Ui::f_ActionneurB * ui ;
            QString             sChemin ;
};

#endif // F_ACTIONNEURB_H
