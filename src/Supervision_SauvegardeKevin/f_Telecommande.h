#ifndef F_TELECOMMANDE_H
#define F_TELECOMMANDE_H

class f_Telecommande;

//===STANDARD===//
#include <QByteArray>

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"


namespace Ui {
class f_Telecommande;
}

class f_Telecommande : public f_WidgetInterne
{
    Q_OBJECT

    public:
        explicit f_Telecommande     (QWidget *parent = 0);
        inline   TypeFenetreSup     getType() { return TELECOMMANDE ; }
                 void               setTexteTitre(QString Titre) ;
        inline   QByteArray         SimulerCommande(QString /*Commande*/) { return QByteArray(); }
                 void               ChangementBouton (QByteArray CommandeRetour) ;
                 void               Commande(QString sCommande) ;
                                    ~f_Telecommande();

    private:
                QPixmap             BrutTransparent;
                Ui::f_Telecommande *ui;
};

#endif // F_TELECOMMANDE_H
