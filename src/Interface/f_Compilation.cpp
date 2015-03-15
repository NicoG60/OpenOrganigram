#include <qmath.h>
#include <QDebug>

#include "f_GestionMem.h"
#include "f_Compilation.h"
#include "ui_f_Compilation.h"

f_Compilation::f_Compilation(Arduino *pArduino, QList<ModuleManager*> ListeManager, QWidget *parent) :
    QDialog         (parent),
    pArduino        (pArduino),
    AttenteReponse  (false),
    Timeout         (0),
    ListeManager    (ListeManager),
    CompteurEnvois  (0),
    GifChargement   (new QMovie(":/Images/App/load.gif")),
    ui              (new Ui::f_Compilation)
{
    connect(pArduino, SIGNAL(RetourCommandeGenerale(QByteArray)), this, SLOT(RetourCommande(QByteArray)));

    ui->setupUi(this);

    for(register int i = 0; i < this->ListeManager.length(); i++)
    {
        this->ui->CB_Module->addItem(this->ListeManager[i]->getNomModule());
    }

    //Initialisation du gif animé
    if (GifChargement->loopCount() != -1)
    {
        connect(GifChargement, SIGNAL(finished()), GifChargement, SLOT(start()));
    }
    connect(GifChargement, SIGNAL(frameChanged(int)), this, SLOT(NouvelleImageGif(int)));

    connect(&TimerEtatSlot, SIGNAL(timeout()), this, SLOT(slot_EtatSlots()));
    connect(&TimerTransfert, SIGNAL(timeout()), this, SLOT(slot_Transfert()));
}

f_Compilation::~f_Compilation()
{
    //Initialisation du gif animé
    if (GifChargement->loopCount() != -1)
    {
        disconnect(GifChargement, SIGNAL(finished()), GifChargement, SLOT(start()));
    }
    disconnect(GifChargement, SIGNAL(frameChanged(int)), this, SLOT(NouvelleImageGif(int)));

    delete GifChargement;
    delete ui;
}

void f_Compilation::on_CB_Module_currentIndexChanged(int)
{
    this->TabOctet.clear();
    this->ui->LB_Taille->clear();
    this->ui->Grp_Slot->setEnabled(false);
}

void f_Compilation::on_Btn_Compiler_clicked()
{
    this->TabOctet = ListeManager[this->ui->CB_Module->currentIndex()]->Compiler();



    int     NbreOctets = this->TabOctet.size();
    QString Texte = QVariant(NbreOctets).toString() + " octets";
    this->ui->LB_Taille->setText(Texte);



    this->ActualiserListeSlots();
    this->ui->Grp_Slot->setEnabled(true);
}

void f_Compilation::slot_EtatSlots()
{
    if(!this->AttenteReponse)
    {
        Timeout     = 0;
        if(this->DerniereReponse.isEmpty())
        {
            if(!this->ListeCommandeEtatSlot.isEmpty())
            {
                qDebug() << "EtatSlot : Envois Commande";

                QString CommandeAEnvoyer(this->ListeCommandeEtatSlot.takeFirst());

                this->AttenteReponse = this->pArduino->EnvoyerDonnees(CommandeAEnvoyer, GEN);

                this->DernierNumeroSlotEnvoye = CommandeAEnvoyer.right(2);
            }
            else
            {
                qDebug() << "EtatSlot : Stop";
                this->TimerEtatSlot.stop();
            }
        }
        else
        {
            qDebug() << "EtatSlot : Gestion Réponse";

            QList<QByteArray> Reponse (this->DerniereReponse.mid(6).split(':'));

            this->ui->CB_Slot->addItem("Slot n°" + this->DernierNumeroSlotEnvoye + " : " + Reponse[0].data());

            this->DerniereReponse.clear();
        }
    }
    else
    {
        Timeout++;

        if(Timeout > 1000)
        {
            Timeout     = 0;
            AttenteReponse    = false;
            qDebug() << "EtatSlot : Timeout...";
            pArduino->AnnulerDerniereCommande(GEN);
        }
    }
}

void f_Compilation::RetourCommande(QByteArray Retour)
{
    qDebug() << "RetourCommande" << Retour;
    if(Retour.left(4) == "DONE")
    {
        this->AttenteReponse = false;
    }
    else
    {
        if(Retour.left(5) == "VALUE")
        {
            this->DerniereReponse = Retour;
            this->AttenteReponse = false;
        }
    }
}

void f_Compilation::ActualiserListeSlots()
{
    this->ui->CB_Slot->clear();

    QSettings   Config("ConfigMemoire.ini", QSettings::IniFormat);
    QList<int>  NbreSlotParBank;

    NbreSlotParBank.append(1);
    NbreSlotParBank.append(qPow(2, Config.value("Bank0", QVariant(0)).toInt()-1));
    NbreSlotParBank.append(qPow(2, Config.value("Bank1", QVariant(0)).toInt()-1));
    NbreSlotParBank.append(qPow(2, Config.value("Bank2", QVariant(0)).toInt()-1));
    NbreSlotParBank.append(qPow(2, Config.value("Bank3", QVariant(0)).toInt()-1));

    for(register int i = 0; i < NbreSlotParBank.length(); i++)
    {
        for(register int j = 0; j < NbreSlotParBank[i]; i++)
        {
            this->ListeCommandeEtatSlot.append("K" + f_GestionMem::CalculerNumeroSlot(i, j));
        }
    }

    this->TimerEtatSlot.start(1);
}

void f_Compilation::on_Btn_Envoyer_clicked()
{
    QString CommandeChoixSlot("F" + this->ui->CB_Slot->currentText().mid(7,2));
    this->GifChargement->start();
    this->CompteurEnvois = 0;
    this->setEnabled(false);
    this->AttenteReponse = this->pArduino->EnvoyerDonnees(CommandeChoixSlot, GEN);
    this->TimerTransfert.start(1);
}

void f_Compilation::slot_Transfert()
{
    if(!this->AttenteReponse)
    {
        Timeout     = 0;
        if(this->CompteurEnvois < this->TabOctet.size())
        {
            QByteArray CommandeAEnvoyer("B");
            CommandeAEnvoyer += this->TabOctet[this->CompteurEnvois];

            this->AttenteReponse = this->pArduino->EnvoyerDonnees(CommandeAEnvoyer, GEN);

            if(this->AttenteReponse)
                this->CompteurEnvois++;
        }
        else
        {
            this->GifChargement->stop();
            this->ui->Btn_Envoyer->setIcon(QIcon(":/Images/App/play.png"));
            this->TimerTransfert.stop();
            this->setEnabled(true);
            QString RenommerSlot    ("H" + this->ui->CB_Slot->currentText().mid(7,2) + this->ui->CB_Module->currentText().leftJustified(8, ' ').toUpper());
            this->AttenteReponse = this->pArduino->EnvoyerDonnees(RenommerSlot, GEN);
            this->ActualiserListeSlots();
        }
    }
    else
    {
        Timeout++;

        if(Timeout > 1000)
        {
            Timeout     = 0;
            this->CompteurEnvois--;
            AttenteReponse    = false;
            qDebug() << "Transfert : Timeout...";
            pArduino->AnnulerDerniereCommande(GEN);
        }
    }
}

void f_Compilation::NouvelleImageGif(int)
{
    this->ui->Btn_Envoyer->setIcon(QIcon(GifChargement->currentPixmap()));
}
