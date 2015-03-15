#ifndef F_CAPTEURN_H
#define F_CAPTEURN_H

class f_CapteurN ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"
#include "f_Supervision.h"

namespace Ui {
class f_CapteurN ;
}

class f_CapteurN : public f_WidgetInterne
{
    Q_OBJECT

    public :
        explicit                f_CapteurN(QWidget *parent = 0) ;
                void            setTexteTitre(QString Titre) ;
                QByteArray      SimulerCommande(QString);
                void            ChangementEtat(QByteArray Etat) ;
                void            ImageLabel (QString sCheminImage) ;
                void            Commande(QString sCommande);
        inline  TypeFenetreSup  getType() { return CAP_N ; }
                                ~f_CapteurN() ;

    protected :
                Ui::f_CapteurN *    ui ;

    private slots:
                void            on_BtnBoutonON_toggled(bool checked);

};

#endif // F_CAPTEURN_H
