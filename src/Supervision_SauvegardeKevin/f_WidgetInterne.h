#ifndef F_WIDGETINTERNE_H
#define F_WIDGETINTERNE_H

class f_WidgetInterne;

//===STANDARD===//
#include <QWidget>

//===PERSONNEL===//
#include "../TypeCommun.h"

class f_WidgetInterne : public QWidget
{
    Q_OBJECT

    public :
        explicit    f_WidgetInterne     (QWidget *parent = 0);
        virtual     TypeFenetreSup      getType() = 0 ;
        virtual     void                setTexteTitre(QString Titre) = 0 ;
        virtual     QByteArray          SimulerCommande(QString Commande) = 0;
                    void                EnvoyerDonneesW(QString Commande) ;
        inline      QString             getCommandeEtat()    { return CommandeEtat ; }
                    void                setCommandeEtat(QString CommandeEtat) ;

    protected :
                    QString             CommandeEtat ;
};

#endif // F_WIDGETINTERNE_H
