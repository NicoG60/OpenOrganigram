//-------------------------------------------------------------------------------
/**
 * @file        BtnItem.cpp
 * @brief       Bouton pour inserer des éléments graphiques d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Bouton pour inserer des éléments grpahiques d'organigramme. Ce bouton est hérité du QPushButton
 * classique pour une gestion plus poussée des événements et pour déclencher correctement le drag'n drop.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Définir les MIME Data pour le drag'n drop => Binaire
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QDebug>
#include <qmath.h>

//=====   Headers Peros   =====
#include "f_GestionMem.h"
#include "ui_f_GestionMem.h"

//========   Defines   ========
#define ARDUINO     (0)
#define BANK0       (1)
#define BANK1       (2)
#define BANK2       (3)
#define BANK3       (4)

/**
 * Constructeur
 * @brief f_GestionMem::f_GestionMem
 * @param pArduino  Connexion à l'arduino
 * @param parent    Widget parent
 */
f_GestionMem::f_GestionMem(Arduino* pArduino, QWidget *parent) :
    QDialog         (parent),
    ConfigMemoire   ("ConfigMemoire.ini", QSettings::IniFormat),
    AttenteReponse  (false),
    NumBankFormat   (ARDUINO),
    NbreSlotFormat  (0),
    CompteurFormat  (0),
    CompteaurSlot   (0),
    Timeout         (0),
    BoutonAAnimer   (0),
    GifChargement   (":/Images/App/load.gif"),
    pArduino        (pArduino),
    Model           (new QStandardItemModel),
    ui              (new Ui::f_GestionMem)
{
    ui->setupUi(this);
    this->ui->listView->setModel(this->Model);

    connect(this->Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_ItemChecked(QStandardItem*)));

    //Initialisation du gif animé
    if (GifChargement.loopCount() != -1)
    {
        connect(&GifChargement, SIGNAL(finished()), &GifChargement, SLOT(start()));
    }
    connect(&GifChargement, SIGNAL(frameChanged(int)), this, SLOT(NouvelleImageGif(int)));

    //Initialisation Timers
    connect(&TimerFormat, SIGNAL(timeout()), this, SLOT(slot_Formater()));
    connect(&TimerEtatSlot, SIGNAL(timeout()), this, SLOT(slot_EtatSlots()));

    //Initialisation Arduino
    connect(pArduino, SIGNAL(ArduinoConnect(int,QString)), this, SLOT(slot_EtatConnexion(int,QString)));
    connect(pArduino, SIGNAL(RetourCommandeGenerale(QByteArray)), this, SLOT(RetourCommande(QByteArray)));

    pArduino->DemanderEtat();

    //InitialisationConfig
    this->ui->CB_Bank0->setCurrentIndex(ConfigMemoire.value("Bank0", QVariant(0)).toInt());
    this->ui->CB_Bank1->setCurrentIndex(ConfigMemoire.value("Bank1", QVariant(0)).toInt());
    this->ui->CB_Bank2->setCurrentIndex(ConfigMemoire.value("Bank2", QVariant(0)).toInt());
    this->ui->CB_Bank3->setCurrentIndex(ConfigMemoire.value("Bank3", QVariant(0)).toInt());
}

/**
 * Destructeur
 * @brief f_GestionMem::~f_GestionMem
 */
f_GestionMem::~f_GestionMem()
{
    delete this->Model;
    delete ui;
}

/**
 * Calcul du numéro du slot en chaine de caractère
 * @brief f_GestionMem::CalculerNumeroSlot
 * @return La chaine numéro de slot
 */
QString f_GestionMem::CalculerNumeroSlot()
{
    return this->CalculerNumeroSlot(this->NumBankFormat, this->CompteaurSlot);
}

QString f_GestionMem::CalculerNumeroSlot(unsigned int Bank, unsigned int Compteur)
{
    int     NumSlot(0);

    switch(Bank)
    {
    case ARDUINO:
        NumSlot = 0;
        break;

    case BANK0:
        NumSlot = 1;
        break;

    case BANK1:
        NumSlot = 9;
        break;

    case BANK2:
        NumSlot = 17;
        break;

    case BANK3:
        NumSlot = 25;
    }

    NumSlot += Compteur;

    return QVariant(NumSlot).toString().rightJustified(2, '0');
}

void f_GestionMem::SauvegarderConfig()
{
    ConfigMemoire.setValue("Bank0", QVariant(this->ui->CB_Bank0->currentIndex()));
    ConfigMemoire.setValue("Bank1", QVariant(this->ui->CB_Bank1->currentIndex()));
    ConfigMemoire.setValue("Bank2", QVariant(this->ui->CB_Bank2->currentIndex()));
    ConfigMemoire.setValue("Bank3", QVariant(this->ui->CB_Bank3->currentIndex()));
}

void f_GestionMem::ActualiserListeVisu()
{
    this->Model->clear();

    QList<int>  NbreSlotParBank;

    NbreSlotParBank.append(1);
    NbreSlotParBank.append(qPow(2, this->ui->CB_Bank0->currentIndex()-1));
    NbreSlotParBank.append(qPow(2, this->ui->CB_Bank1->currentIndex()-1));
    NbreSlotParBank.append(qPow(2, this->ui->CB_Bank2->currentIndex()-1));
    NbreSlotParBank.append(qPow(2, this->ui->CB_Bank3->currentIndex()-1));

    for(register int i = 0; i < NbreSlotParBank.length(); i++)
    {
        for(register int j = 0; j < NbreSlotParBank[i]; i++)
        {
            this->ListeCommandeEtatSlot.append("K" + this->CalculerNumeroSlot(i, j));
        }
    }

    qDebug() << "ListeCommande : " << this->ListeCommandeEtatSlot;
    qDebug() << "Actualisation";
    this->TimerEtatSlot.start(1);
}

/**
 * Reour de commande arduino
 * @brief f_GestionMem::RetourCommande
 * @param Retour
 */
void f_GestionMem::RetourCommande(QByteArray Retour)
{
    if(Retour.left(4) == "DONE")
        AttenteReponse = false;
    else
    {
        if(Retour.left(5) == "VALUE")
        {
            this->DerniereReponse = Retour;
            this->AttenteReponse = false;
        }
    }
}

void f_GestionMem::slot_ItemChecked(QStandardItem* item)
{
    QString Commande    ("G" + item->text().mid(7,2));

    if(item->checkState() == Qt::Checked)
    {
        Commande.append("1");
    }
    else
    {
        Commande.append("0");
    }

    this->pArduino->EnvoyerDonnees(Commande, GEN);
}

/**
 * Retour de l'état de connexion de l'arduino
 * @brief f_GestionMem::slot_EtatConnexion
 * @param Etat
 */
void f_GestionMem::slot_EtatConnexion(int Etat, QString)
{
    if(Etat != 0)
        ui->tabWidget->setEnabled(false);
    else
        ui->tabWidget->setEnabled(true);
}

/**
 * Formate les slots un à un...
 * @brief f_GestionMem::slot_Formater
 */
void f_GestionMem::slot_Formater()
{
    QString NumeroSlot(this->CalculerNumeroSlot());
    QByteArray CommandeFormat;

    CommandeFormat.append('B');
    CommandeFormat.append('\255');

    //Si on est pas en attente de réponse
    if(!this->AttenteReponse)
    {

        Timeout     = 0;
        //si le compteur de formatage est de dessous de la taille d'un slot
        if(this->CompteurFormat < 4)
        {
            QByteArray CommandeAEnvoyer;
            switch(this->CompteurFormat)
            {
            case 0:
                CommandeAEnvoyer += QString("F" + NumeroSlot);
                break;

            case 1:
                CommandeAEnvoyer += CommandeFormat;
                break;

            case 2:
                CommandeAEnvoyer += QString("H" + NumeroSlot + "__VIDE__");
                break;

            case 3:
                CommandeAEnvoyer += QString("G" + NumeroSlot + "0");
                break;
            }

            this->AttenteReponse = this->pArduino->EnvoyerDonnees(CommandeAEnvoyer, GEN);
            this->CompteurFormat++;
        }
        else //Si on est au dessusde la taille d'un slot c'est que c'est la fin
        {
            this->CompteurFormat = 0;
            this->CompteaurSlot++;
            qDebug() << "Compteur Slot : " << this->CompteaurSlot;
            if(this->CompteaurSlot >= this->NbreSlotFormat)
            {
                this->CompteaurSlot = 0;
                this->TimerFormat.stop();
                this->GifChargement.stop();
                this->BoutonAAnimer->setIcon(QIcon());
            }

        }
    }
    else    //Si on est en attente, on compte... au bout d'une seconde on abandonne
    {
        Timeout++;

        if(Timeout > 1000)
        {
            Timeout     = 0;
            AttenteReponse    = false;
            qDebug() << "Timeout...";
            this->CompteurFormat--;
            pArduino->AnnulerDerniereCommande(GEN);
        }
    }
}

void f_GestionMem::slot_EtatSlots()
{
    if(!this->AttenteReponse)
    {
        Timeout     = 0;
        if(this->DerniereReponse.isEmpty())
        {
            if(!this->ListeCommandeEtatSlot.isEmpty())
            {
                qDebug() << "Envois Commande";

                QString CommandeAEnvoyer(this->ListeCommandeEtatSlot.takeFirst());

                this->AttenteReponse = this->pArduino->EnvoyerDonnees(CommandeAEnvoyer, GEN);

                this->DernierNumeroSlotEnvoye = CommandeAEnvoyer.right(2);
            }
            else
            {
                qDebug() << "Stop";
                this->TimerEtatSlot.stop();
            }
        }
        else
        {
            qDebug() << "Gestion Réponse";

            QList<QByteArray> Reponse (this->DerniereReponse.mid(6).split(':'));

            QStandardItem* NouvelItem   (new QStandardItem);

            NouvelItem->setCheckable(true);
            NouvelItem->setText("Slot n°" + this->DernierNumeroSlotEnvoye + " : " + Reponse[0].data());

            if(Reponse[1] == "1")
            {
                NouvelItem->setCheckState(Qt::Checked);
            }
            else
            {
                NouvelItem->setCheckState(Qt::Unchecked);
            }

            this->Model->appendRow(NouvelItem);
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
            qDebug() << "Timeout...";
            this->CompteurFormat--;
            pArduino->AnnulerDerniereCommande(GEN);
        }
    }
}

/**
 * Click sur le bouton d'activation de l'autorun
 * @brief f_GestionMem::on_ActiverAutorun_clicked
 */
void f_GestionMem::on_ActiverAutorun_clicked()
{
    pArduino->EnvoyerDonnees(QString("T1"), GEN);
}

void f_GestionMem::on_DesactiverAutorun_clicked()
{
    pArduino->EnvoyerDonnees(QString("T0"), GEN);
}

/**
 * Click qur le bouton de formatage de la mémoire interne
 * @brief f_GestionMem::on_Btn_Format_Arduino_clicked
 */
void f_GestionMem::on_Btn_Format_Arduino_clicked()
{
    this->NumBankFormat     = ARDUINO;
    this->NbreSlotFormat    = 1;
    this->BoutonAAnimer     = this->ui->Btn_Format_Arduino;

    GifChargement.start();
    TimerFormat.start(1);
}

/**
 * A chaque nouvelle image du gif, on met à jour le bouton
 * @brief f_GestionMem::NouvelleImageGif
 */
void f_GestionMem::NouvelleImageGif(int)
{
    BoutonAAnimer->setIcon(QIcon(GifChargement.currentPixmap()));
}

void f_GestionMem::on_CB_Bank0_currentIndexChanged(int index)
{
    if(index > 0)
    {
        this->ui->Btn_Format_Bank0->setEnabled(true);
    }
    else
    {
        this->ui->Btn_Format_Bank0->setEnabled(false);
    }

    this->SauvegarderConfig();
}

void f_GestionMem::on_CB_Bank1_currentIndexChanged(int index)
{
    if(index > 0)
    {
        this->ui->Btn_Format_Bank1->setEnabled(true);
    }
    else
    {
        this->ui->Btn_Format_Bank1->setEnabled(false);
    }

    this->SauvegarderConfig();
}

void f_GestionMem::on_CB_Bank2_currentIndexChanged(int index)
{
    if(index > 0)
    {
        this->ui->Btn_Format_Bank2->setEnabled(true);
    }
    else
    {
        this->ui->Btn_Format_Bank2->setEnabled(false);
    }

    this->SauvegarderConfig();
}

void f_GestionMem::on_CB_Bank3_currentIndexChanged(int index)
{
    if(index > 0)
    {
        this->ui->Btn_Format_Bank3->setEnabled(true);
    }
    else
    {
        this->ui->Btn_Format_Bank3->setEnabled(false);
    }

    this->SauvegarderConfig();
}

/**
 * Click sur le bouton de formatage de la banque 0
 * @brief f_GestionMem::on_Btn_Format_Bank0_clicked
 */
void f_GestionMem::on_Btn_Format_Bank0_clicked()
{
    this->NumBankFormat     = BANK0;
    this->NbreSlotFormat    = qPow(2, this->ui->CB_Bank0->currentIndex()-1);
    this->BoutonAAnimer     = this->ui->Btn_Format_Bank0;

    qDebug() << "BANK 0 : " << this->NbreSlotFormat << " slot(s)";

    GifChargement.start();
    TimerFormat.start(1);
}

void f_GestionMem::on_Btn_Format_Bank1_clicked()
{
    this->NumBankFormat     = BANK1;
    this->NbreSlotFormat    = qPow(2, this->ui->CB_Bank1->currentIndex()-1);
    this->BoutonAAnimer     = this->ui->Btn_Format_Bank1;

    qDebug() << "BANK 1 : " << this->NbreSlotFormat << " slot(s)";

    GifChargement.start();
    TimerFormat.start(1);
}

void f_GestionMem::on_Btn_Format_Bank2_clicked()
{
    this->NumBankFormat     = BANK2;
    this->NbreSlotFormat    = qPow(2, this->ui->CB_Bank2->currentIndex()-1);
    this->BoutonAAnimer     = this->ui->Btn_Format_Bank2;

    qDebug() << "BANK 2 : " << this->NbreSlotFormat << " slot(s)";

    GifChargement.start();
    TimerFormat.start(1);
}

void f_GestionMem::on_Btn_Format_Bank3_clicked()
{
    this->NumBankFormat     = BANK3;
    this->NbreSlotFormat    = qPow(2, this->ui->CB_Bank3->currentIndex()-1);
    this->BoutonAAnimer     = this->ui->Btn_Format_Bank3;

    qDebug() << "BANK 3 : " << this->NbreSlotFormat << " slot(s)";

    GifChargement.start();
    TimerFormat.start(1);
}

void f_GestionMem::on_tabWidget_currentChanged(int index)
{
    if(this->ui->tabWidget->tabText(index) == "Visualisation")
    {
        this->ActualiserListeVisu();
    }
}

void f_GestionMem::on_Btn_Actualiser_clicked()
{
    this->ActualiserListeVisu();
}

void f_GestionMem::on_LancerProg_clicked()
{
    this->pArduino->EnvoyerDonnees(QString("X"), GEN);
}
