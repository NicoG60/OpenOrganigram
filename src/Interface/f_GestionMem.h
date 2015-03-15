//  f_GestionMem.h       1.0     03/06/14     N.Jarnoux
#ifndef F_GESTIONMEM_H
#define F_GESTIONMEM_H

class f_GestionMem;

//===== Headers standards =====
#include <QDialog>
#include <QMovie>
#include <QSettings>
#include <QStandardItemModel>

//=====   Headers persos  =====
#include "Arduino.h"

namespace Ui {
class f_GestionMem;
}

class f_GestionMem : public QDialog
{
    Q_OBJECT

    public:
        explicit f_GestionMem(Arduino* pArduino, QWidget *parent = 0);  //constructeur
        ~f_GestionMem();                                                //Destructeur

        static QString  CalculerNumeroSlot(unsigned int Bank, unsigned int Compteur);   //Calcul le numéro du slot à effacer en chaine de caractère

    private:
        QString             CalculerNumeroSlot();   //Calcul le numéro du slot à effacer en chaine de caractère

        void                SauvegarderConfig();
        void                ActualiserListeVisu();

        QSettings           ConfigMemoire;

        bool                AttenteReponse;         //Attente d'une réponse de l'arduino

        int                 NumBankFormat;          //Numero de la bank à formater
        int                 NbreSlotFormat;         //Nombre de slot à formater
        int                 CompteurFormat;         //Compteur de formatage d'un slot
        int                 CompteaurSlot;          //Compteur de slot à formateur
        int                 Timeout;                //Compteur de timeout

        QPushButton*        BoutonAAnimer;          //Pointeur vers le bouton qui viens d'etre cliqué pour afficher une animation
        QMovie              GifChargement;          //Gif animé de chargement
        Arduino*            pArduino;               //Pointeur vers la communication avec arduino
        QTimer              TimerFormat;            //Timer de formatage

        QStandardItemModel* Model;
        QList<QString>      ListeCommandeEtatSlot;
        QString             DernierNumeroSlotEnvoye;
        QByteArray          DerniereReponse;
        QTimer              TimerEtatSlot;          //Timer d'etat des slots

        Ui::f_GestionMem *  ui;                     //Interface

    private slots:
        void RetourCommande(QByteArray Retour);     //Retour de commande Arduino
        void slot_ItemChecked(QStandardItem* item);
        void slot_EtatConnexion(int Etat, QString); //Slot d'état de connexion de la maquette
        void slot_Formater();                       //Slot de formatage
        void slot_EtatSlots();                      //Slot d'etat des slots memoires
        void on_ActiverAutorun_clicked();           //Slot de click sur l'autorun
        void on_DesactiverAutorun_clicked();
        void on_Btn_Format_Arduino_clicked();       //Slot d'appuie sur le formatage dela memoire interne
        void NouvelleImageGif(int);                 //Slot de nouvelle image du gif
        void on_CB_Bank0_currentIndexChanged(int index);
        void on_CB_Bank1_currentIndexChanged(int index);
        void on_CB_Bank2_currentIndexChanged(int index);
        void on_CB_Bank3_currentIndexChanged(int index);
        void on_Btn_Format_Bank0_clicked();
        void on_Btn_Format_Bank1_clicked();
        void on_Btn_Format_Bank2_clicked();
        void on_Btn_Format_Bank3_clicked();
        void on_tabWidget_currentChanged(int index);
        void on_Btn_Actualiser_clicked();
        void on_LancerProg_clicked();
};

#endif // F_GESTIONMEM_H
