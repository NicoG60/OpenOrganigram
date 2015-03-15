#ifndef F_LCD_H
#define F_LCD_H

class f_LCD ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"


namespace Ui {
class f_LCD;
}

class f_LCD : public f_WidgetInterne
{
    Q_OBJECT

    public:
        explicit f_LCD              (QWidget *parent = 0);
        inline   TypeFenetreSup     getType() { return LCD ; }
                 void               setTexteTitre(QString Titre) ;
                 QByteArray         SimulerCommande(QString Commande);
                 void               Commande(QString sCommande) ;
                                    ~f_LCD();

    private slots:
                void                on_BtnRAZ_clicked();
                void                on_BtnEnvoyer_clicked();
                void                on_CheckBoxPrecis_toggled(bool checked);
                void                on_BtnValeurPrecise_clicked(bool checked);
                void                on_SpinBoxColonne_valueChanged(QString arg1);
                void                on_SpinBoxLigne_valueChanged(QString arg1);

    protected :
        virtual void                keyPressEvent ( QKeyEvent * event ) ;                  //Event permettant de choisir l'action qui suit les touches du clavier

    private:
                Ui::f_LCD *         ui;
                QString             sCommandeLCD ;
                QString             sColonne ;
                QString             sLigne ;
};

#endif // F_LCD_H
