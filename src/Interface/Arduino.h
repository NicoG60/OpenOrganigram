#ifndef ARDUINO_H
#define ARDUINO_H

class Arduino ;

//===STANDARD===//
#include <QObject>
#include <QByteArray>
#include <QTime>
#include <QTimer>
#include <QQueue>

//===PERSONNEL===//
#include "./LibSerialPort/qextserialenumerator.h"
#include "./LibSerialPort/qextserialport.h"
#include"../TypeCommun.h"
#include "../Supervision/f_Supervision.h"




class Arduino : public QObject
{
    Q_OBJECT

    public :
        explicit Arduino    (QObject *parent = 0) ;
                 void       Connecter(QString sNomPort) ;                                   //Methode de connexion avec en paramètre le nom du port
                 void       Deconnecter() ;                                                 //Méthode de déconnexion
                 void       DemanderEtat() ;                                                //Demande d'état de la maquette
                 void       setSimulation(f_Supervision* pSup);
                 void       AnnulerDerniereCommande(EmetteurCommande Emetteur);
                            ~Arduino() ;
                 void       RemplirQueueSupervisionWeb(QByteArray QueueRetour) ;

    public slots :
                 bool       EnvoyerDonnees(QString sCommande, EmetteurCommande Emetteur) ;    //Slot public d'émission des données avec la commande et l'émetteur
                 bool       EnvoyerDonnees(QByteArray sCommande, EmetteurCommande Emetteur) ; //Slot public d'émission des données avec la commande et l'émetteur (Version Tableau d'octet, implémenté par N. Jarnoux)
                 void       RecevoirDonnees() ;                                               //Slot privé de reception des données
                 void       RemplirTampon(QByteArray Tableau) ;                               //Slot privé de stockage tamporaire du tampon

    private slots :
                void        Detecter() ;                                                   //Slot privé de detection de l'Arduino
                void        RetourCommandeSimulation();
                void        UtiliserArduino();                                             //Slot déclenché quand l'arduino sera initialisé

    signals :
                void        ArduinoConnect(int nEtat, QString sCOM) ;                      //Signal de connection de l'Arduino
                void        DonneesALire(QByteArray Donnees) ;                             //Signal de données
                void        RetourCommandeSupervision(QByteArray ValeurRetourSup) ;        //Signal de retour de commande pour la Supervision
                void        RetourCommandeOrganigram(QByteArray ValeurRetourOrg) ;         //Signal de retour de commande pour l'Organigram
                void        Signal_Temps(QTime Incrementation) ;                           //Signal du temps de connection de l'Arduino
                void        RetourCommandeSupervisionWEB(QQueue<QString> QueueRetourCommandeSupWeb) ;    //Signal de retour de commande pour la supervison WEB
                void        RetourCommandeGenerale(QByteArray ValeurRetourGen);

    private :
                QByteArray                  Tampon ;                                //Tampon qui va permettre le stockage de la reception de la lecture
                QextSerialPort *            PortSerie ;                             //L'objet représentant le port
                QTimer *                    pCompteur ;                             //Timer de detection
                QTime                       DureeDeConnexion ;                      //Incrémenter pour la durée de connexion
                QQueue<EmetteurCommande>    FileEmetteur ;                          //File d'attente des émetteurs
                f_Supervision*              pSimulation;
                QByteArray                  RetourSim;
                bool                        bArduinoUtilisable;                     //Définit si l'arduino est utilisable ou non
                QQueue<QString>             QueueValeurCommandeSupW ;

};

#endif // ARDUINO_H
