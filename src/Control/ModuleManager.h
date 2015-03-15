//  ModuleManager.h      1.0     25/02/14     N.Jarnoux
#ifndef MODULEMANAGER_H
#define MODULEMANAGER_H

class ModuleManager ;

#define INTERVAL_DEFAULT    (50)

#define EN_EXEC             (0)
#define ARRETE              (1)
#define PAUSE               (2)

//===== Headers standards =====
#include <QObject>
#include <QList>
#include <QTabWidget>
#include <QStack>
#include <QTimer>
#include <QXmlStreamWriter>

//=====   Headers persos  =====
#include "Routine.h"
#include "../Vue/Vue.h"
#include "../Interface/Arduino.h"

class ModuleManager : public QObject
{
    Q_OBJECT
    public:
        explicit                ModuleManager(QString sNomModule, Arduino * pArduino, QObject *   pParent = 0); //Constructeur
                                ~ModuleManager() ;                                                              //Destructeur

                void            AjouterRoutine(QString sNomRoutine) ;       //CRéer une nouvelle routine en donnant son nom
                void            AjouterRoutine(Routine *    pRout) ;        //Ajoute une routine à la liste
                void            SupprimerRoutine(QString sNomRoutine) ;     //Supprime la routine portant le nom
                void            SupprimerRoutine(int nId) ;                 //Supprime la routine à cet id
                void            InitExecution() ;       //Initialise l'environnement d'exécution d'un organigramme
                void            LancerExecution() ;     //Lance l'exécution de l'organigramme
                void            StopperExecution() ;    //Stoppe l'exécution de l'organigramme
                void            PasserInstSuiv() ;      //Passe à l'instruction suivant (en cas de mode debug)
                void            setTemporisation(unsigned int ms) ; //Modifie la temporisation entre 2 instruction
                void            AjouterALaPile(IdentifiantInst Identifiant) ;   //Ajoute une instruction dans la pile
        inline  QString         getNomModule()                      { return sNomModule ; }             //Accesseur du nom du module
        inline  QTabWidget *    getTabWidget()                      { return pTabW_AfficheurRoutine ; } //Accesseur du tab widget affichant les routines
        inline  QList<Routine*> getListeRoutines()                  { return ListeRoutines ; }           //Accesseur de la liste des routines
        inline  QList<ActionArduino>    getListeActionPossible()    { return ListeActionPossible ; }    //Mutateur d'action possible
        inline  QList<TestArduino>      getListeTestPossible()      { return ListeTestPossible ; }      //Mutateur de test possible
        inline  QByteArray      getDerniereReponse()                { return DerniereReponse ; }        //Ascesseur de la derniere réponse obtenue de l'arduino
        inline  void            setDerniereReponse(QByteArray Rep)  { DerniereReponse = Rep ;}          //Mutateur de la derniere réponse obtenue
        inline  int             getNbreRoutine()                    { return ListeRoutines.length() ; } //Ascesseur du nombre de routine
        inline  void            AjouterARU(QString ARU)             { if(!ARU.isEmpty()) ListeARU.append(ARU); }    //Ajoute une commande d'arrêt d'urgence
                int             getNbreInst();                                          //Ascesseur du nombre d'instruction
                unsigned short  getValVar(unsigned int nIndice) ;                       //Accesseur d'une valeur d'une variable par son indice
                void            setValVar(unsigned int nIndice, unsigned short nVal) ;  //Mutateur d'une valeur de variable
                void            EnvoyerCommande(QString sCommande) ;    //Envoie une commande à l'arduino
                void            RafraichirVues();       //Rafraichi les vues
                void            Enregistrer(QXmlStreamWriter* Parser);  //Enregistre son contenu
                void            AjouterAction(ActionArduino Action);        //Ajoute une action possible
                void            AjouterAction(QList<ActionArduino> Action); //Ajoute une liste d'action possible
                void            AjouterTest(TestArduino Test);              //Ajoute un test possible
                void            AjouterTest(QList<TestArduino> Test);       //Ajoute une liste de test possible
                QByteArray      Compiler();

    private:
        QList<Routine *>        ListeRoutines ;             //Liste des routines
        QString                 sNomModule ;                //Nom du module représenté
        QTabWidget *            pTabW_AfficheurRoutine ;    //Widget d'affichage en onglets
        QList<Vue *>            ListeVues ;                 //Liste des vues d'organigrammes
        unsigned short          ListeVariables[26] ;        //Liste des variables utilisables
        QList<ActionArduino>    ListeActionPossible ;       //Liste des action possibles
        QList<TestArduino>      ListeTestPossible ;         //Liste des tests possibles

        //Attributy pour l'execution
        QTimer                  TimerExec ;                 //Timer d'exécution
        QTimer                  TimerARU ;                  //Timer d'arrêt d'urgence
        QStack<IdentifiantInst> PileExec;                   //Pile d'exécution
        unsigned int            Temporisation ;             //Temporisation entre instruction
        int                     CompteurTimeout;            //Compteur de timeout
        int                     CompteurARU ;               //Compteur d'arrêt d'urgence
        bool                    ModeDebug ;                 //Mode pas à pas
        bool                    AttenteReponse ;            //Attente de réponse de l'arduino
        Arduino *               pArduino ;                  //Pointeur vers la communication avec arduino
        QByteArray              DerniereReponse ;           //Derniere réponse obtenue de arduino
        QList<QString>          ListeARU;                   //Liste des arrêts d'urgence

        void                    InitVariable();             //Initialisation des variables

    private slots:
        void                    ExecuterCommandeSuivante() ;    //Exécuter l'instruction suivante
        void                    ExecuterARUSuivant();           //exécuter l'arrêt d'urgence suivant
        void                    RetourCommande(QByteArray sRetour) ;//Retour de commande de l'arduino

    signals:
        void                    EtatExec(int); //Signal d'état d'exécution
};

#endif // MODULEMANAGER_H
