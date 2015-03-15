#ifndef F_BUZZER_H
#define F_BUZZER_H

class f_Buzzer;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"
#include "../Arduino.h"

namespace Ui {
class f_Buzzer;
}

class f_Buzzer : public f_WidgetInterne
{
    Q_OBJECT

    public:
            explicit                f_Buzzer(QWidget *parent = 0);
                    void            setTexteTitre(QString Titre) ;
                    void            Commande(QString sCommande) ;
            inline  TypeFenetreSup  getType() { return BUZZER ; }
            inline  QByteArray      SimulerCommande(QString /*Commande*/) { return QByteArray(); }
                                    ~f_Buzzer();

    protected :
            virtual void            keyPressEvent ( QKeyEvent * event ) ;                  //Event permettant de choisir l'action qui suit les touches du clavier

    private slots:
            void                    on_BtnJouer_clicked();
            void                    on_SpBFrequence_valueChanged(const QString &arg1);
            void                    on_SpBTemps_valueChanged(const QString &arg1);

    private:
            Ui::f_Buzzer *  ui;
            QString         sFrequence ;
            QString         sTemps ;
            QString         sCommandeBuzzer ;
};

#endif // F_BUZZER_H
