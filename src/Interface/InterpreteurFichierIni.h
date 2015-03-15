//  InterpreteurFichierIni.h       1.0     03/06/14     N.Jarnoux
#ifndef INTERPRETEURFICHIERINI_H
#define INTERPRETEURFICHIERINI_H

class InterpreteurFichierIni ;

//===== Headers standards =====
#include <QDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QShowEvent>
#include <QTimer>

//=====   Headers persos  =====
#include "Arduino.h"

class InterpreteurFichierIni : public QObject
{
    Q_OBJECT

    public:
        explicit InterpreteurFichierIni(QString Chemin, Arduino* pArduino, QObject *parent = 0);//Constructeur
        ~InterpreteurFichierIni();                                                              //Destructeur

        void    Interpreter();                                                          //Interprete le fichier ini et configure la maquette
        inline QString getCheminFichierIni() const { return FichierIni.fileName(); }    //REtourne le chemin vers le fichier ini interpété

    private:
        Arduino*        pArduino;               //Communication arduino
        QSettings       FichierIni;             //Parseur ini
        QTimer          Timer;                  //Timer d'exec
        bool            bAttente;               //Attente
        bool            bErreur;                //Erreur
        int             Timeout;                //compteur timeout
        QList<QString>  ListeCommandeConfig;    //Liste des commande à exec

    private slots:
        void slot_timeout();                    //Slot à chaque déclenchement du timer
        void RetourCommande(QByteArray Retour); //Retour de commande arduino

    signals:
        void FinConfigMaquette(InterpreteurFichierIni*, bool);  //Signal de fin de config
};

#endif // INTERPRETEURFICHIERINI_H
