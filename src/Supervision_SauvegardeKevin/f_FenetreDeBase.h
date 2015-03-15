#ifndef F_FENETREDEBASE_H
#define F_FENETREDEBASE_H

class f_FenetreDeBase ;

//===PERSONNEL===//
#include "f_WidgetInterne.h"
#include "../TypeCommun.h"


namespace Ui {
class f_FenetreDeBase;
}

class f_FenetreDeBase : public f_WidgetInterne
{
    Q_OBJECT

    public:
        explicit                f_FenetreDeBase(QWidget *parent = 0);
        inline  TypeFenetreSup  getType() { return AUTRE ; }
                void            setTexteTitre(QString Titre) ;
        inline  QByteArray      SimulerCommande(QString /*Commande*/) { return QByteArray(); }
            void                RetourValeurFDB (QByteArray ValeurRetour) ;
            void                ImageLabel(QString sCheminImage);
            void                Commande(QString sCommande) ;
            void                Affichage(QString sTexte1, QString sCode1,QString sTexte2, QString sCode2) ;
            void                TypeSONAR(QString sType) ;
                                ~f_FenetreDeBase();

    private:
            Ui::f_FenetreDeBase* ui;
            QString              sTexte1 ;
            QString              sTexte2 ;
            QString              sCode1 ;
            QString              sCode2 ;
            QString              sType ;
};

#endif // F_FENETREDEBASE_H
