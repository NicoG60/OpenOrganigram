#ifndef F_SUBWINDOW_H
#define F_SUBWINDOW_H

class f_SubWindow;

//===STANDARD===//
#include <QCloseEvent>
#include <QMdiSubWindow>

//===PERSONNEL===//
#include "../TypeCommun.h"
#include "f_WidgetInterne.h"

#define NOM_ICONE_MDI   ":/Images/Sup/oeil.bmp"

class f_SubWindow : public QMdiSubWindow
{
    Q_OBJECT

    public :
        explicit    f_SubWindow         (QWidget *parent = 0) ;
                    TypeFenetreSup      getType() ;
        inline      f_WidgetInterne*    getWidget() { return qobject_cast<f_WidgetInterne*>(this->widget()) ; }
                    void                EnvoyerDonneesSubW(QString Commande) ;

    protected :
                    void                closeEvent(QCloseEvent* e) ;
};

#endif // F_SUBWINDOW_H
