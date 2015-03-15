#ifndef F_WIDGETINTERNE_H
#define F_WIDGETINTERNE_H

class f_WidgetInterne;

//===STANDARD===//
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

//===PERSONNEL===//
#include "../TypeCommun.h"

class f_WidgetInterne : public QWidget
{
    Q_OBJECT

    public :
        explicit    f_WidgetInterne     (QString IOCard, QString Broche, QWidget *parent = 0);
        virtual     TypeFenetreSup      getType() = 0 ;
        virtual     QString             getCommandeSupervision() = 0;
        virtual     void                RetourCommande(QByteArray Retour) = 0;
        inline      QString             getTitre() { return LB_Titre->text(); }
        virtual     QByteArray          SimulerCommande(QString Commande) = 0;
        virtual     void                Superviser() = 0;
                    void                EnvoyerDonneesW(QString Commande) ;

    protected :
                    //Interface
                    QVBoxLayout*    LayoutV;
                    QLabel*         LB_Image;
                    QLabel*         LB_Titre;

                    QString         IOCard;
                    QString         Broche;
};

#endif // F_WIDGETINTERNE_H
