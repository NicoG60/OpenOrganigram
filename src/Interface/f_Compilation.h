// f_Compilation.h  1.0 21/12/2014  N.Jarnoux
#ifndef F_COMPILATION_H
#define F_COMPILATION_H

class f_Compilation;

#include <QDialog>
#include <QMovie>

#include "../Control/ModuleManager.h"
#include "Arduino.h"

namespace Ui {
class f_Compilation;
}

class f_Compilation : public QDialog
{
    Q_OBJECT

public:
    explicit f_Compilation(Arduino* pArduino, QList<ModuleManager*> ListeManager, QWidget *parent = 0);
    ~f_Compilation();

private slots:
    void on_CB_Module_currentIndexChanged(int);

    void on_Btn_Compiler_clicked();
    void slot_EtatSlots();
    void slot_Transfert();
    void RetourCommande(QByteArray Retour);

    void on_Btn_Envoyer_clicked();
    void NouvelleImageGif(int);

private:
    void                    ActualiserListeSlots();

    Arduino*                pArduino;
    bool                    AttenteReponse;
    int                     Timeout;
    QByteArray              DerniereReponse;
    QString                 DernierNumeroSlotEnvoye;

    QList<QString>          ListeCommandeEtatSlot;
    QTimer                  TimerEtatSlot;

    QList<ModuleManager*>   ListeManager;
    QByteArray              TabOctet;
    int                     CompteurEnvois;
    QTimer                  TimerTransfert;

    QMovie*                 GifChargement;          //Gif animé de chargement
    Ui::f_Compilation *     ui;
};

#endif // F_COMPILATION_H
