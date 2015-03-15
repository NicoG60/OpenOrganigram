#ifndef F_CAPTEURA_H
#define F_CAPTEURA_H

class f_CapteurA ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"
#include "../Arduino.h"

namespace Ui {
class f_CapteurA ;
}

class f_CapteurA : public f_WidgetInterne
{
    Q_OBJECT

    public :
        explicit                f_CapteurA(QWidget *parent = 0) ;
                void            setTexteTitre(QString Titre) ;
                QByteArray      SimulerCommande(QString Commande);
                void            RetourValeurPotan (QByteArray ValeurRetour) ;
                void            ImageLabel (QString sCheminImage) ;
        inline  TypeFenetreSup  getType() { return CAP_A ; }
                void            Commande(QString sCommande ) ;
                void            Convertir (int nConvers);
                                ~f_CapteurA() ;

    private slots:
                void            on_checkBox_toggled(bool checked);
                void            on_dial_valueChanged(int value);

    private :
                Ui::f_CapteurA *    ui ;
                int                 nConversion ;



};

#endif // F_CAPTEURA_H
