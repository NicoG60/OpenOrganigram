#ifndef f_INTERPRETEURCOMMANDES_H
#define f_INTERPRETEURCOMMANDES_H

class f_InterpreteurCommandes ;

//===STANDARD===//
#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QList>
#include <QStack>

//===PERSONNEL===//
#include"../TypeCommun.h"

namespace Ui {
class f_InterpreteurCommandes;
}

class f_InterpreteurCommandes : public QDialog
{
    Q_OBJECT

    public :
        explicit f_InterpreteurCommandes  (QWidget *parent = 0) ;                             //Constructeur classe Lecture
                                          ~f_InterpreteurCommandes() ;                                               //Destructeur classe Lecture

    protected :
        virtual void            keyPressEvent ( QKeyEvent * event ) ;                  //Event permettant de choisir l'action qui suit les touches du clavier

    public slots :
                void            RecevoirDonneesL(QByteArray DonneesRecues) ;                   //Affiche les données qui sont reçues en paramètre
                void            ArduinoConnect(int nEtat, QString sCOM) ;                      //Affiche l'état et le port de connection de l'Arduino

    private slots :
                void            on_BoutonValider_clicked() ;                                   //Se lance lorsque le bouton "Valider" est préssé
                void            Signal_TempsL(QTime Incrementation) ;                          //Affiche le temps de connection de l'Arduino

    signals :
                void            EnvoyerDonneesL(QString Commande, EmetteurCommande Emetteur) ;  //Signal d'émission de données avec en paramètre la commande et l'émetteur

    private :
                Ui::f_InterpreteurCommandes *   ui;
                QList<QString>                  sSauvegardeCommande ;                               //String qui enregistre la dernière commande envoyée
                int                             indexRetour;

};

#endif // f_INTERPRETEURCOMMANDES_H
