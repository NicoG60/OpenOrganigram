#ifndef F_ACTIONNEURG_H
#define F_ACTIONNEURG_H

class f_ActionneurG ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"

namespace Ui {
class f_ActionneurG ;
}

class f_ActionneurG : public f_WidgetInterne
{
    Q_OBJECT

    public :
            explicit            f_ActionneurG(QWidget *parent = 0) ;
            void                setTexteTitre(QString Titre) ;
            QByteArray          SimulerCommande(QString Commande);
            void                RetourDeValeur(QByteArray CommandeRetour) ;
            void                Commande(QString sCommande);
     inline TypeFenetreSup      getType() { return ACT_G ; }
                                ~f_ActionneurG() ;

    private slots:
            void                on_SBxspinBoxValeur_valueChanged(int arg1);
            void                on_dialValeur_valueChanged(int value);

    private :
            Ui::f_ActionneurG *     ui ;
            QString                 sCommandeActG ;
};

#endif // F_ACTIONNEURG_H
