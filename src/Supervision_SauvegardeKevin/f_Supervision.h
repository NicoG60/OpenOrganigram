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
#include "f_ActionneurB.h"
#include "./Interface/Arduino.h"
#include "f_SubWindow.h"
#include "f_CapteurN.h"
#include "F_Moteur.h"
#include "f_lcd.h"


namespace Ui {
class f_Supervision ;
}

class f_Supervision : public QWidget
{
    Q_OBJECT

    public :
        explicit    f_Supervision   (Arduino *pControleur,QWidget *parent = 0);
                    void            CreerActionneurB (QString sNom, unsigned int nNumBroche, QString sCommande1, QString sCheminImage) ;
                    void            CreerActionneurG (QString sNom, unsigned int nNumBroche, QString sCommande1) ;
                    void            CreerMoteur (QString sNom, QString nNumBroche, QString sTypeMoteur, QString sCheminImage) ;
                    void            CreerCapteurA (QString sNom, unsigned int nNumBroche, QString sCommande1, QString sCheminImage, int nConversion) ;
                    void            CreerCapteurN (QString sNom, unsigned int nNumBroche, QString sCommande1, QString sCheminImage) ;
                    void            EnvoyerDonneesSup(QString Commande);
                    void            CreerFenetreDeBase (QString sNom, QString sType,unsigned int nNumBroche, QString sCommande1, QString sCheminImage,QString sTexte1,QString sCode1,QString sTexte2,QString sCode2);
                    void            CreerBuzzer(QString sNom,unsigned int nNumBroche, QString sCommande1);
                    void            CreerTelecommande(QString sNom,unsigned int nNumBroche, QString sCommande1);
                    void            CreerLCD(QString sNom, QString, unsigned int);
                    void            OuvrirFichierINI(QString sChemin );
                    QByteArray      SimulerEnvoiDonnees(QByteArray Commande);
                    void            PurgerListeFenetre();
                                    ~f_Supervision();

    protected:
        virtual     void            showEvent(QShowEvent * e);
        virtual     void            hideEvent(QHideEvent *e);

    public slots:
                    void            RetourCommande(QByteArray ValeurRetour);
                    void            EmplacementSupervision(Qt::DockWidgetArea Loc);

    private slots :
                    void            on_BtAfficher_clicked() ;
                    void            on_BtMasquer_clicked() ;
                    void            on_TempsFinit ();
                    void            on_BtHorizontal_clicked();
                    void            on_BtVertical_clicked();

    private :
                    Ui::f_Supervision *     ui ;
                    QList<f_SubWindow *>    ListeFenetre ;
                    Arduino*                pConnecterArduino ;
                    QTimer *                pCompteurSup ;
                    QQueue<QString>         FileCommande ;
                    int                     nIndiceFenetre ;
                    unsigned int            Compteur ;
                    bool                    bAttenteReponse ;
                    bool                    bLCD ;
};

#endif // F_SUPERVISION_H
