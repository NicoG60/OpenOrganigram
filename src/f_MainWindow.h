//  f_MainWindow.h      1.0     21/02/14     N.Jarnoux
#ifndef F_MAINWINDOW_H
#define F_MAINWINDOW_H

class f_MainWindow;

//===== Headers standards =====
#include <QMainWindow>
#include <QList>
#include <QStandardItem>
#include <QLabel>
#include <QXmlStreamReader>
#include <QProgressBar>

//=====   Headers persos  =====
#include "Control/ModuleManager.h"
#include "Interface/BtnItem.h"
#include "Interface/Arduino.h"
#include "Interface/InterpreteurFichierIni.h"
#include "SupervisionWeb/ServeurHTTPThread.h"

typedef struct {
    unsigned char ProjetOuvert : 1 ;
    unsigned char ProjetEnregistre : 1 ;
    unsigned char ModuleCharge : 1 ;
} EtatProjet ;

typedef struct
{
    EtatProjet  Etat;
    QString     sCheminVersProjet ;
} DescProjet ;

namespace Ui {
class f_MainWindow ;
}

class f_MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit        f_MainWindow(QWidget *  pParent = 0) ;  //Constructeur de la fenetre
                        ~f_MainWindow() ;                       //Destructeur de la fenetre

        void            Click_BtnItem(TypeCommande Type);       //Gestion d'un clic simple sur le bouton à gauche
        inline Arduino* getArduino()    { return pArduino ; }   //Accesseur de la connesion arduino

    protected:
        void        closeEvent(QCloseEvent *    pEvent) ;   //Gestion de l'évenement de fermeture

    private:
        //===== ATTRIBUTS  PRIVES =====
        QString                 MotDePass ;                 //Mot de passe du compte admin (je suppose...)
        QString                 ProfilparDefaut ;           //Identifie le profil par défaut de l'appli
        QList<ModuleManager *>  Lst_pManagers ;             //Liste des managers de modules
        int                     IndiceModuleCourant ;       //Indice dans la liste du module actuellement chargé
        DescProjet              EtatProjetenCours ;         //Représente l'état du projet (ouvert, enregistré...)
        QStandardItem *         pStIt_Projet ;              //Element de la liste déroulante de gestion de projet
        QStandardItem *         pStIt_ListeModules ;        //Idem
        BtnItem *               pBt_ItemDock[7] ;           //7 boutons dockés sur le côté pour ajouter les items organigramme1
        Arduino *               pArduino ;                  //Connexion à l'arduino
        QLabel *                pEtatConnexion ;            //Label rendant compte de l'état de la connexion
		ServeurHTTPThread       ServeurWeb;                 //Serveur web pour supervision
        QByteArray              ConfigurationArduinoCourante;//Configuration acutellement chargée dans le logiciel
        Ui::f_MainWindow *      ui ;                        //Objet d'interface graphique

        //===== METHODES PRIVEES =====
        void InitialiserGestionProjet() ;
        void InitialiserBt_ItemDock() ;                     //Initialise les 6 Boutons dockés
        void ActiverBt_ItemDock(bool en) ;                  //Active ou desactive les Boutons Dockés
        void ActiverActions(bool bActive) ;                 //Active ou desactive les actions possible (barre d'outils/menus...)
        bool Sauver() ;                                     //Sauvegarde un projet
        void Fermer() ;                                     //Ferme un projet
        void Ouvrir() ;                                     //Ouvre un projet
        void ParserLeFichier(QFile& FichierOrganigramme, QProgressBar *pBarreChargement) ;  //Parcour le fichier XML et l'interprete
        void CreerNouveauProjet(QString CheminIni);         //Créer un nouveau projet
        bool ProjetSauve() ;                                //Teste si le projet est sauvé ou non
        void ObtenirEmplacementProjet(bool bOuvrir = false) ;//Ouvre le dialog pour choisir l'emplacement et le nom de sauvegarde
        void OuvrirFenetreConfig() ;                        //Ouvre une fenêtre de config
        void OuvrirFenetreNouveauProfil() ;                 //Ouvre une fenetre de profil
        void OuvrirFenetreChoisirProfil() ;                 //Ouvre la fenêtre de choix de profil
        void OuvrirFenetreSupprimerProfil() ;               //Ouvre la fenêtre de suppression de profil
        void OuvrirFenetreModifierProfil() ;                //Ouvre la fenêtre de modification de profil
        void ChargerModule(QString sNomModule) ;            //Selectionne le bon module, met son indice dans IndiceModuleCourant et l'affiche.
        void RechargerConfig();

    private slots:
        void on_actionNouveau_triggered() ;                         //Demande de nouveau projet
        void on_actionEnregistrer_triggered() ;                     //Demande d'enregistrement simple
        void on_actionEnregistrer_sous_triggered() ;                //Demande d'enregistrer sous
        void on_actionFermer_triggered() ;                          //Demande de fermeture du projet
        void on_actionQuitter_triggered() ;                         //Demande de fermeture de l'app
        void on_actionConfiguration_triggered() ;                   //Demande d'affichage de la configuration
        void on_actionNouveauProfil_triggered() ;                   //Demande de création de profil
        void on_actionChoisirProfil_triggered();                    //Demande d'affichage de choix du profil actif
        void on_actionSupprimerProfil_triggered();                  //Demande d'affichage de suppression de profil
        void on_actionModifierProfil_triggered();                   //Demande d'affichage de modification de profil
        void on_treeView_doubleClicked(const QModelIndex &index);   //Demande d'affichage d'une routine par double clic sur la liste
        void on_actionAgrandir_triggered();                         //Demande de zoom du dessin
        void on_actionRetrecir_triggered();                         //Demande de dézoom du dessin
        void on_actionInterpreteur_triggered();                     //Demande d'ouverture de l'interpreteur Arduino
        void slot_EtatConnexion(int Etat, QString COM) ;            //Demande de mise à jour de l'état de la conexion avec Arduino
        void on_actionExecuter_triggered();                         //Demande d'exécution d'un organigramme
        void on_actionArreter_triggered();                          //DEmande l'arrete d'un organigramme
        void slot_EtatExecution(int nEtat);                         //Retour de l'état de connexion de arduino
        void on_actionJump_triggered();                             //Demande de saut à l'instruction suivante
        void on_actionOuvrir_triggered();                           //Demande d'ouverture de projet
        void on_actionConvert_triggered();                          //Demande de compilation
        void on_actionGestionMem_triggered();                       //Ouvre la fneetre de gestion mémoire
        void on_actionImporter_triggered();                         //DEmande de nouveau projet à partri d'un plan de câblage existant
        void slot_FinInterpretationIniImportConfig(InterpreteurFichierIni *Interpreteur, bool Reussi);  //Fin d'interpretation du fichier ini pour l'importation
        void slot_FinInterpretationIniOuvrirProjet(InterpreteurFichierIni *Interpreteur, bool Reussi);  //Fin d'interpretation du fichier ini pour l'ouverture
        void on_actionServeur_Web_toggled(bool arg1);               //DEmande de démarrage du seveur web
        void on_actionRedemarrer_la_maquette_triggered();
        void on_actionGestion_des_composants_I2C_triggered();
};

#endif // F_MAINWINDOW_H
