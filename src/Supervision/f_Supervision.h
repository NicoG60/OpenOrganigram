#ifndef F_SUPERVISION_H
#define F_SUPERVISION_H

class f_Supervision ;

//===STANTARD===//
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QList>
#include <QString>
#include <QTimer>
#include <QQueue>
#include <QSettings>

//===PERSONNEL===//
#include "f_SubWindow.h"
#include "./Interface/Arduino.h"


namespace Ui {
class f_Supervision ;
}

class f_Supervision : public QWidget
{
    Q_OBJECT

    public :
        explicit    f_Supervision   (Arduino *pControleur,QWidget *parent = 0);
                    void            EnvoyerDonneesSup(QString Commande);
                    void            OuvrirFichierINI(QString sChemin );
                    QByteArray      SimulerEnvoiDonnees(QByteArray Commande);
                    void            PurgerListeFenetre();
                                    ~f_Supervision();

    protected:
        virtual     void            showEvent(QShowEvent * e);
        virtual     void            hideEvent(QHideEvent *e);

    private slots:
                    void            RetourCommande(QByteArray ValeurRetour);
                    void            EmplacementSupervision(Qt::DockWidgetArea Loc);

                    void            on_BtAfficher_clicked() ;
                    void            on_TempsFinit ();
                    void            on_BtHorizontal_clicked();
                    void            on_BtVertical_clicked();

    private :
                    Ui::f_Supervision *     ui ;
                    QList<f_SubWindow *>    ListeFenetre ;
                    Arduino*                pArduino ;
                    QTimer                  CompteurSup ;
                    QQueue<QString>         FileCommande ;
                    int                     nIndiceFenetre ;
                    unsigned int            Compteur ;
                    bool                    bAttenteReponse ;
};

#endif // F_SUPERVISION_H
