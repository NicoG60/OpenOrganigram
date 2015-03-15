// Serveur.h     1.0     25.02   Jonathan Aubry

#ifndef __SERVEUR_H
#define __SERVEUR_H


/*
* Déclaration anticipée de la classe
*/
class Serveur ;


/*
* Inclusion des fichiers EN-TÊTE
*/
#include <QTcpServer>
#include <QDebug>
#include <QVector>

#include "Thread.h"
#include "Interface/Arduino.h"


/*
* Déclaration de la classe
*/
class Serveur : public QTcpServer
{
    Q_OBJECT


    public :
        //Constructeur
        explicit Serveur(Arduino *pArduino, QObject *parent = 0) ;

        //Démarrage du serveur
        void StartServer() ;

        //Destructeur
        ~Serveur() ;

        //Fonction d'arrêt du serveur
        void StopServeur() ;

    protected :


    private :
        /*-- Attributs privées --*/
        //Fonction d'attente de connexion
        void incomingConnection(int nHandleSocket) ;

        //Tailles des fichiers html de la page de supervision
        int nTaille_Composant ;
        int nTaille_Valeur ;

        //Contient l'adresse de l'arduino
        Arduino * pAdresseArduino ;

        //Vecteur d'adresse des threads
        QVector <Thread *> Vecteur_Thread ;

        //Thread
        Thread * pThread ;

        //Numéro de port du serveur
        int nNumeroPort ;

        /*-- Méthodes privées --*/
        //Fonctions d'initalisation de la page de supervision web grâce au fichier de configuration
        void InitialiserComposants() ;
        void InitialiserValeur() ;

    signals :


    public slots :
};

#endif // __SERVEUR_H
