
//-----------------------------------------------------------------------------------
/** @file       f_Supervision.cpp
 *  @brief      IHM Principale
 *  @author     Kévin BATARD
 *  @author     STS IRIS, Lycée Nicolas Appert, ORVAULT (FRANCE)
 *  @since      4 Février 2014
 *  @version    1.0
 *  @date       21 Février 2014
 *
 *  Classe de gestion de l'IHM principale de supervision, qui définit la zone MDI
 *
 *  Fabrication
 *
 *  @todo       Il faut coder la classe
 *
 *  @bug
 *
 */
//-----------------------------------------------------------------------------------

//=====   En-Têtes Personnels   =====
#include "f_Supervision.h"
#include "ui_f_Supervision.h"
#include "f_CapteurN.h"
#include "f_CapteurA.h"
#include "f_ActionneurG.h"
#include "f_ActionneurB.h"
#include "f_FenetreDeBase.h"
#include "f_Buzzer.h"
#include "f_Telecommande.h"
#include "f_Moteur.h"
#include "f_lcd.h"

//=====   En-Têtes standards    =====
#include <QDebug>


/** Description détaillée de la méthode
 * @pre Constructeur par défaut
 *
 **/
f_Supervision::f_Supervision(Arduino *pControleur,QWidget *parent) : // Constructeur
    QWidget(parent),
    ui(new Ui::f_Supervision),
    pConnecterArduino(pControleur),
    pCompteurSup(0),
    nIndiceFenetre(0),
    Compteur(0),
    bAttenteReponse(false),
    bLCD(false)
{
     this->ui->setupUi(this) ;

    pConnecterArduino->setSimulation(this);

    // Timer de l'arduino
    this->pCompteurSup= new QTimer (this) ;                                                 //Création du Timer
    this->pCompteurSup->setInterval(200);                                                   //On définit un intervalle  pour le Timer
    connect(this->pCompteurSup,SIGNAL(timeout()),this, SLOT(on_TempsFinit()));              //Lorsque le temps est atteint, on lance Detection
    this->ui->ZoneMDI->setWindowTitle("Fenetre") ;                                          //Titre de la fenêtre

    //this->CreerLCD("LCD","LCD",15);

    this->connect(pConnecterArduino, SIGNAL(RetourCommandeSupervision(QByteArray)), this, SLOT(RetourCommande(QByteArray))); //On récupère le signal de retour de commande de supervsion au retour de commande de la classe


}


/**  Destructeur
 *
 **/
f_Supervision::~f_Supervision() // Destructeur
{
    for(register int i = 0; i < ListeFenetre.length(); i++)     //On parcourt l'intégralité des fenêtres
    {
        delete this->ListeFenetre[i] ;                          //On les supprime
    }

    this->pConnecterArduino=0;
    this->nIndiceFenetre=0 ;
    this->Compteur=0;
    this->bAttenteReponse=false;

    delete ui ;


}


/**
 * @test    Va afficher toute les fenêtres de la QList
 *
 **/
void f_Supervision::on_BtAfficher_clicked() // Permet de ré afficher toute les fenêtres
{
    for(register int i = 0; i < ListeFenetre.length(); i++)     //On parcourt l'intégralité des fenêtres
    {
        this->ListeFenetre[i]->setVisible(true) ;               //On rend toutes les fenêtres visibles
    }
}


/**
 * @test    Va rendre invisible toute les fenêtres de la QList
 *
 **/
void f_Supervision::on_BtMasquer_clicked() // Permet de masquer toute les fenêtres
{
    for(register int i = 0; i < ListeFenetre.length(); i++)     //On parcourt l'intégralité des fenêtres
    {
        this->ListeFenetre[i]->setVisible(false) ;              //On rend toutes les fenêtres invisibles
    }
}

/**         Création de fenêtre pour LED
 * @param   sNom=Nom de l'actionneur, nNumBroche=Numéro de la broche, sCommande=Une commande d'action, sCheminImage
 * @test    Création de d'une fenêtre ActionneurB
 *
 **/
void f_Supervision::CreerActionneurB(QString sNom, unsigned int nNumBroche, QString sCommande, QString sCheminImage)
{

    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_ActionneurB* f_Actionneur = new f_ActionneurB ;               //Création objet f_ActionneurB
    NouvelleSubWindow->setWidget(f_Actionneur);                     //On ajoute à la nouvelle subwindow l'actionneur
    NouvelleSubWindow->setWindowTitle("LED");                       //On précise un titre
    f_Actionneur->Commande(sCommande);                              //On renvoie la commande à f_ActionneurB
    f_Actionneur->ImageLabel(sCheminImage);                         //On envoie le chemin de fichier image à f_ActionneurB
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe

}


/**         Création de fenêtre pour Servo Moteur
 * @param   sNom=Nom de l'actionneur, nNumBroche=Numéro de la broche, sCommande=Une commande d'action, Chemin image
 * @test    Création de d'une fenêtre ActionneurG
 *
 **/
void f_Supervision::CreerActionneurG(QString sNom, unsigned int nNumBroche, QString sCommande)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_ActionneurG * f_Actionneur = new f_ActionneurG ;              //Création objet f_ActionneurG
    f_Actionneur->Commande(sCommande);                              //On renvoie la commande à f_ActionneurB
    NouvelleSubWindow->setWidget(f_Actionneur);                     //On ajoute à la nouvelle subwindow l'actionneur
    NouvelleSubWindow->setWindowTitle("Servo Moteur") ;             //On précise un titre
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe

}

/**         Création de fenêtre pour les moteurs
 * @param   sNom=Nom du moteur, sNumBroche=Numéros de la broches, sTypeMoteur=le type, sCheminImage
 * @test    Création de d'une fenêtre ActionneurB
 *
 **/
void f_Supervision::CreerMoteur(QString sNom, QString sNumBroche, QString sTypeMoteur, QString sCheminImage)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    F_Moteur * F_Mot = new F_Moteur ;                               //Création objet f_Moteur
    F_Mot->ImageLabel(sCheminImage);                                //On envoie le chemin de fichier image à f_Moteur
    F_Mot->DefinirBroche(sNumBroche);                               //On renvoie les numéros de brochse du moteur
    F_Mot->DefinirTypeMoteur(sTypeMoteur);                          //On renvoie le type de moteur
    NouvelleSubWindow->setWidget(F_Mot);                            //On ajoute à la nouvelle subwindow le moteur
    NouvelleSubWindow->setWindowTitle("Moteur") ;                   //On précise un titre
    QVariant vNumBroche(sNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe

}



/**         Création de fenêtre pour capteur analogique
 * @param   sNom=Nom de l'actionneur, sType=IN/OUT, nNumBroche=Numéro de la broche, sCommande=Une commande d'action
 * @test    Création de d'une fenêtre CapteurA
 *
 **/
void f_Supervision::CreerCapteurA(QString sNom, unsigned int nNumBroche, QString sCommande, QString sCheminImage, int nConversion)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_CapteurA * f_Capteur = new f_CapteurA ;                       //Création objet f_CapteurA
    NouvelleSubWindow->setWidget(f_Capteur);                        //On ajoute à la nouvelle subwindow le capteur analogique
    NouvelleSubWindow->setWindowTitle(sNom) ;                       //On précise un titre
    f_Capteur->Commande(sCommande);                                 //On renvoie la commande à f_CapteurA
    f_Capteur->Convertir(nConversion);                              //On renvoie le type de capteur pour savoir si il faut une conversion de la valeur
    f_Capteur->ImageLabel(sCheminImage);                            //On envoie le chemin de fichier image à f_CapteurA
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe

}


/**         Création d'un bouton poussoir
 * @param   sNom=Nom de l'actionneur, nNumBroche=Numéro des broche, sCommande=Une commande d'action,chemin image
 * @test    Création de d'une fenêtre CapteurN
 *
 **/
void f_Supervision::CreerCapteurN(QString sNom, unsigned int nNumBroche, QString sCommande, QString sCheminImage)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_CapteurN *f_Capteur = new f_CapteurN;                         //Création objet f_CapteurN
    f_Capteur->Commande(sCommande);                                 //On renvoie la commande à f_CapteurN
    NouvelleSubWindow->setWidget(f_Capteur);                        //On ajoute à la nouvelle subwindow le capteur numérique
    NouvelleSubWindow->setWindowTitle("Bouton Poussoir") ;          //On précise un titre
    f_Capteur->ImageLabel(sCheminImage);                            //On envoie le chemin de fichier image à f_CapteurN
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe

}

/**         Création de fenêtre pour tous les capteurs non définis
 * @param   sNom=Nom du moteur,sType=IN/OUT, nNumBroche=Numéro de la broche sCheminImage= chemin de l'image, sCommande1 et sCommande2 = 2 commandes, sTexte1 et sTexte2= 2 textes correspondant
 *
 **/
void f_Supervision::CreerFenetreDeBase (QString sNom, QString sType,unsigned int nNumBroche, QString sCommande1, QString sCheminImage,QString sTexte1, QString sCode1, QString sTexte2, QString sCode2)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_FenetreDeBase* f_FenetreBase = new f_FenetreDeBase ;          //Création objet f_FenetreDeBase
    f_FenetreBase->Commande(sCommande1);                            //On renvoie la commande à f_FenetreDeBase
    f_FenetreBase->Affichage(sTexte1,sCode1,sTexte2,sCode2);        //On envoie les deux commandes possibles avec les actions correspondantes
    NouvelleSubWindow->setWidget(f_FenetreBase);                    //On ajoute à la nouvelle subwindow la fenêtre
    f_FenetreBase->ImageLabel(sCheminImage);                        //On envoie le chemin de fichier image à f_FenetreDeBase
    NouvelleSubWindow->setWindowTitle("Fenêtre"+sNom) ;             //On précise un titre
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte="" ;
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    if(sType=="AI")                                                 //Si c'est un capteur analogique
    {
       Texte += " \nCONNECTÉ À LA BROCHE N° A-" ;                   //On ajoute un texte de liaison précisant "A"
       Texte += vNumBroche.toString() ;                             //On ajoute ensuite la broche
       NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;       //On envoie le titre complet à la classe
    }
    else if(sType=="SONAR")                                         //Si c'est un capteur ultrason de distance
    {
        f_FenetreBase->TypeSONAR(sType);                            //On renvoie le type SONAR
        Texte += " \nCONNECTÉ À LA BROCHE N° " ;                    //On ajoute un texte de liaison
        QString sCom1 = sCommande1.mid(1) ;                         //On commence après le premier caractère
        QString sCom2 = sCom1.left(2) + " ET " ;                    //On récupère les deux premier caractères et on ajoute un texte de liaison
        QString sCom3 = sCom1.right(2) ;                            //On récupère les deux dernier caractères
        Texte += sCom2+sCom3;                                       //On les assemble
        NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;      //On envoie le titre complet à la classe
    }
    else
    {
         Texte += " \nCONNECTÉ À LA BROCHE N° " ;                   //On ajoute un texte de liaison
         Texte += vNumBroche.toString() ;                           //On ajoute ensuite la broche
         NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;     //On envoie le titre complet à la classe
    }



}

/**         Création d'un Buzzer
 * @param   sNom=Nom, nNumBroche=Numéro des broche, sCommande=Une commande d'action,
 *
 **/
void f_Supervision::CreerBuzzer(QString sNom,unsigned int nNumBroche, QString sCommande)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_Buzzer* f_Buzz = new f_Buzzer ;                               //Création objet f_Buzzer
    f_Buzz->Commande(sCommande);                                    //On renvoie la commande à f_Buzzer
    NouvelleSubWindow->setWidget(f_Buzz);                           //On ajoute à la nouvelle subwindow la fenêtre buzzer
    NouvelleSubWindow->setWindowTitle("Fenêtre Buzzer") ;           //On précise un titre
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe
}


/**         Création d'une télécommande
 * @param   sNom=Nom, nNumBroche=Numéro des broche, sCommande=Une commande d'action
 *
 **/
void f_Supervision::CreerTelecommande(QString sNom, unsigned int nNumBroche, QString sCommande)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_Telecommande * f_Tele = new f_Telecommande ;                  //Création objet f_Telecommande
    NouvelleSubWindow->setWidget(f_Tele);                           //On ajoute à la nouvelle subwindow la fenêtre télécommande
    f_Tele->Commande(sCommande);                                    //On renvoie la commande à f_Telecommande
    qDebug () << "Commane tele" << sCommande ;
    NouvelleSubWindow->setWindowTitle("Fenêtre Télécommande");      //On précise un titre
    QVariant vNumBroche(nNumBroche) ;                               //Variation d'un int en QString
    QString Texte ="";
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ À LA BROCHE N° " ;                        //On ajoute un texte de liaison
    Texte += vNumBroche.toString() ;                                //On ajoute ensuite la broche
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe
}

/**         Création du LCD
 * @param   sNom=Nom de l'actionneur, nNumBroche=Numéro des broche, sCommande=Une commande d'action
 *
 **/
void f_Supervision::CreerLCD(QString sNom, QString /*sType*/,unsigned int /*nNumBroche*/)
{
    f_SubWindow *   NouvelleSubWindow   (new f_SubWindow(this)) ;   //On déclare un nouveau f_Subwindow
    this->ui->ZoneMDI->addSubWindow(NouvelleSubWindow) ;            //On l'ajoute à la zone MDI
    this->ListeFenetre.append(NouvelleSubWindow);                   //On ajoute cette fenêtre à la liste de fenêtre

    f_LCD* pLCD = new f_LCD ;                                       //Création objet f_LCD
    NouvelleSubWindow->setWidget(pLCD);                             //On ajoute à la nouvelle subwindow la fenêtre LCD
    NouvelleSubWindow->setWindowTitle("Fenêtre LCD") ;              //On précise un titre
    //QVariant vNumBroche(nNumBroche) ;
    QString Texte="" ;
    Texte += sNom ;                                                 //Texte se remplit du nom passé en paramètre
    Texte += " \nCONNECTÉ AU BUS I2C " ;                            //On ajoute un texte de liaison
   // Texte += vNumBroche.toString() ;
    NouvelleSubWindow->getWidget()->setTexteTitre(Texte) ;          //On envoie le titre complet à la classe
}

/**         Permet d'envoyer les données à l'Arduino
 * @param   sCommande= correspond à la commande
 *
 **/
void f_Supervision::EnvoyerDonneesSup(QString sCommande)
{
    if(!sCommande.isEmpty())                                                        //Si la commande n'est pas vide
    {
        if(!this->bAttenteReponse || this->bLCD==true)                              //Et si on n'est pas en attente de réponse
        {
            bool bReponse (false);                                                  //bRetour à false
            bReponse = this->pConnecterArduino->EnvoyerDonnees(sCommande, SUPV);    //On récupère dans un bool l'état de l'envoi de commande pour voir si elle a réussi ou non

            if(bReponse)                                                            //Si elle a réussi
            {
                this->bAttenteReponse = true ;                                      //On met à true l'attente
                this->FileCommande.enqueue(sCommande) ;                            //On ajoute la commande à la file de commande
            }
        }
        else
        {
            this->Compteur ++;                                                      //On incrémente un compteur

            if(this->Compteur > 5)                                                  //Si on dépasse 5 tours de boucle
            {
                this->Compteur = 0;
                qDebug() << "f_Supervision::EnvoyerDonneesSup => Timeout...";       //TIMEOUT
                this->bAttenteReponse = false ;                                     //Attente repasse à false
                this->FileCommande.dequeue() ;                                      //On enlève la dernière commande dans la file
                this->pConnecterArduino->AnnulerDerniereCommande(SUPV);             //On annule aussi le dernier émetteur
            }
         }
    }

}

/**         Slot public : Permet de gérer le retour de commande de l'Arduino
 * @param   ValeurRetour= retour de commande renvoyé par l'Arduino
 *
 **/
void f_Supervision::RetourCommande(QByteArray ValeurRetour)
{
    bool Redemarrer(false);                                             //bool permettant la reconnexion de l'Arduino
    if(this->pCompteurSup != 0 && this->pCompteurSup->isActive())       //Si le compteur est lancé et actif
    {
        this->pCompteurSup->stop();                                     //On stop le timer
        Redemarrer = true;                                              //On le passe à true
    }

    this->bAttenteReponse = false ;

    if(ValeurRetour.left(4) == "DONE")                                  //Si le retour de commande commence par "DONE" on ne le gère pas
    {
        //qDebug() << "Done";
    }

    //qDebug() << "f_Supervision::RetourCommande - ValeurRetour : " << ValeurRetour;
    QString CommandeCourante ="" ;                                                              //On définit un string de commande actuelle
    if(!this->FileCommande.isEmpty())                                                           //Si la file de commande n'est pas vide
    {
        CommandeCourante = this->FileCommande.head();                                           //On prend la première commande de la file
    }

    if(!CommandeCourante.isEmpty())                                                             //Si la commande n'est pas vide
    {
        f_WidgetInterne *   FenetreRetour(0) ;                                                  //On déclare un pointeur nul
        for(register int i = 0; i < ListeFenetre.length(); i++)                                 //On parcourt toutes les fenêtres
        {
            if(this->ListeFenetre[i]->getWidget()->getCommandeEtat() == CommandeCourante)       //Et on vérifie quelle commande correspond à quelle fenêtre
            {
                FenetreRetour = this->ListeFenetre[i]->getWidget();                             //On récupère le type de fenêtre
            }
        }

        this->FileCommande.dequeue();                                                           //On supprime la dernière commande

        if(FenetreRetour != 0)                                                                  //Si il n'est pas vide
        {

            switch (FenetreRetour->getType()) {                                                 //En fonction du type de fenêtre
            case CAP_N :
                qobject_cast<f_CapteurN *>(FenetreRetour)->ChangementEtat(ValeurRetour);        //On retourne la valeur à la classe f_CapteurN
                break;

            case ACT_B :
                qobject_cast<f_ActionneurB *>(FenetreRetour)->ChangementImage(ValeurRetour);    //On retourne la valeur à la classe f_ActionneurB
                break;

            case CAP_A :
                qobject_cast<f_CapteurA *>(FenetreRetour)->RetourValeurPotan(ValeurRetour);     //On retourne la valeur à la classe f_CapteurA
                break;

            case TELECOMMANDE :
                qobject_cast<f_Telecommande*>(FenetreRetour)->ChangementBouton(ValeurRetour);   //On retourne la valeur à la classe f_Telecommande
                break;

            case AUTRE :
                qobject_cast<f_FenetreDeBase*>(FenetreRetour)->RetourValeurFDB(ValeurRetour);   //On retourne la valeur à la classe f_FenetreDeBase
            default:
                break;
            }
        } 
    }

    if(Redemarrer)                                                                              //Si on peut lancer la reconnexion
    {
        this->pCompteurSup->start();                                                            //On relance le timer
    }
}

/**         Se lance toute les 200ms pour envoyer les commanes
 *
 **/
void f_Supervision::on_TempsFinit()
{
    if(!this->ListeFenetre.isEmpty())                                                       //Si il existe au moins une fenêtre
    {
        int Indice   (this->nIndiceFenetre);                                                //On indique dans un int le numéro de fenêtre
        QString sCommande (this->ListeFenetre[Indice]->getWidget()->getCommandeEtat()) ;    //On met dans un QString les commandes de chaque fenêtre
        if(!sCommande.isEmpty())                                                            //Si la commande n'est pas vide
        {
            this->EnvoyerDonneesSup(sCommande);                                             //On envoie la commande
        }
        this->nIndiceFenetre++;                                                             //On incrémente l'indice de fenêtre
        if(this->nIndiceFenetre == this->ListeFenetre.length())                             //Si l'indice de fenêtre à fait le tour de toutes les fenêtres
        {
            this->nIndiceFenetre = 0;                                                       //On initialise l'indice de fenêtre
        }
    }
}


/**  Permet de lancer le timer lorsque la supervision est ouverte
 *
 **/
void f_Supervision::showEvent(QShowEvent * e)
{
    qDebug() << "Supervision Show";
    if(this->pCompteurSup != 0)         //Si le compteur est créé
    {
        this->pCompteurSup->start();    //On lance le timer
    }
    QWidget::showEvent(e);
}


/**  Permet d'arrêter le timer lorsque la supervision est fermée
 *
 **/
void f_Supervision::hideEvent(QHideEvent * e)
{
    qDebug() << "Supervision Hide";
    if(this->pCompteurSup != 0)         //Si le compteur est créé
    {
        this->pCompteurSup->stop();     //On arrête le timer
    }
    QWidget::hideEvent(e);
}


/**         Slot Privé : Permet de réorgniser les fenêtres de façon horizontales lorsque l'on clique sur le bouton
 **/
void f_Supervision::on_BtHorizontal_clicked()
{

    if (this->ui->ZoneMDI->subWindowList().isEmpty())
    {
        return;
    }
    QPoint position(0, 0);
    foreach (QMdiSubWindow *window, this->ui->ZoneMDI->subWindowList())
    {
        QRect rect(0, 0, this->ui->ZoneMDI->width() / this->ui->ZoneMDI->subWindowList().count(),
                         this->ui->ZoneMDI->height());
        window->setGeometry(rect);
        window->move(position);
        position.setX(position.x() + window->width());

    }
}


/**         Slot Privé : Permet de réorgniser les fenêtres de façon verticales lorsque l'on clique sur le bouton
 **/
void f_Supervision::on_BtVertical_clicked()
{
    if (this->ui->ZoneMDI->subWindowList().isEmpty())
    {
        return;

    }
    QPoint position(0, 0);
    foreach (QMdiSubWindow *window, this->ui->ZoneMDI->subWindowList())
    {
        QRect rect(0, 0, this->ui->ZoneMDI->width() / this->ui->ZoneMDI->subWindowList().count(),
                         this->ui->ZoneMDI->height());
        window->setGeometry(rect);
        window->move(position);
        position.setY(position.y()+ window->width());
    }
}


/**         Slot Public : Permet de gérer l'affichage des fenêtres selon la position du dock de supervision
 * @param   Loc= position de la supervision
 *
 **/
void f_Supervision::EmplacementSupervision(Qt::DockWidgetArea Loc)
{   
    switch (Loc) {
    case 0x1:
        this->on_BtVertical_clicked();
        break;
    case 0x2:
        this->on_BtVertical_clicked();
        break;
    case 0x4:
        this->on_BtHorizontal_clicked();
        break;
    case 0x8:
        this->on_BtHorizontal_clicked();
        break;
    default:
        break;
    }

}


/**         Elle permet de lire les fichiers INI d'un plan de cablâge et de récupérer les informations dedanns pour pouvoir par la suite détecter les actionneurs ou
 *          capteurs présents. Elle transmet les broches numériques, analogiques,le chemin d'image, la commande etc.. à chaque carte I/O
 * @param   sChemin=le chemin du fichier INI
 *
 **/
void f_Supervision::OuvrirFichierINI(QString sChemin)
{

    this->PurgerListeFenetre();

    QSettings FichierIni(sChemin,QSettings::IniFormat) ;                    //On ouvre le fichier INI en fonction du chemin passé en paramètre
    //======================= FICHIERS INI ==========================//
//== On parcourt l'intégralité du fichier INI, et si on détecte un "Groupe" commencant par "EASYCON" ==/
//== On récupère la broche numérique, analogique, le type, et le chemin image ==/
//== Chaque groupe est identifié à une carte I/O, et on transmet les informations nécessaires ==/
//== Je vais seulement détailler le premier cas, car c'est à chaque fois le même système ==/

        QStringList ListeGroupe (FichierIni.childGroups());
        bool bSonarPulse (false);
        bool bSonarEcho (false);
        QString                 sSonarPulse ="" ;
        QString                 sSonarEcho ="" ;
        QString                 sCommandeSONAR = "" ;
        QString                 sBrocheMoteur = "";
        int                     nSonarPulse =0 ;
        int                     nSonarEcho =0 ;
        for(register int i = 0; i < ListeGroupe.length(); i++)
        {
            if(ListeGroupe[i].left(4) == "EASY" || ListeGroupe[i].left(7) == "ARDUINO")
            {
                QString Type="";
                QString Image="";
                int BrocheNum=0;
                int BrocheAna=0;

                QStringList debug (FichierIni.value(ListeGroupe[i] + "/Broche_Numerique").toString());
                qDebug() << debug;

                BrocheNum           =  FichierIni.value(ListeGroupe[i] + "/Broche_Numerique").toString().split(',').at(0).toUInt();
                BrocheAna           =  FichierIni.value(ListeGroupe[i] + "/Broche_Analogique").toUInt();
                sBrocheMoteur       =  FichierIni.value(ListeGroupe[i] + "/Broche_Numerique").toString().split(',').join(',');
                Type                =  FichierIni.value(ListeGroupe[i] + "/Type").toString();
                Image               =  FichierIni.value(ListeGroupe[i] + "/Iocard").toString();
                QString  GestionImage = Image.left(4);
                qDebug() << Type;
                if(Image=="IRF1")                                                                       //Si c'est une télécommande
                {
                    QSettings settings ("./IOCard/IRF1/config.ini", QSettings::IniFormat );             //On ouvre le fichier INI de la télécommande
                    QString sNom = settings.value("TITRE/NOM").toString();                              //Dedans on récupère le NOM
                    //QString   sCheminImage = "./IOCard/"+Image ;                                      //On récupère le chemin de l'image
                    for (register int i =0; i<sNom.length();i++)                                        //On parcourt tous les caractères
                    {
                        sNom[i]=sNom[i].toUpper();                                                      //On met le nom en majuscule
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheAna) ;                                                     //La broche analogique va être mise dans un QString
                    QString Texte ="I";                                                                 //On met le caractère correspondant à la carte I/O (Ici : I pour la télécommande)
                    sCommande += Texte ;                                                                //On met ce caractère dans un nouveau QString
                    if(BrocheAna<10)                                                                    //Si la broche est composé d'un seul caractère
                    {
                        sCommande += "0";                                                               //On rajoute un "0" pour faire deux caractères
                    }
                    sCommande += vCommande.toString() ;                                                 //On forme la commande avec le "I" et la broche
                    this->CreerTelecommande(sNom,BrocheAna,sCommande);                                  //On appelle ensuite la méthode créant la carte en question (Ici : La télécommande)
                }
                else if (Image=="LCD1")
                {
                    QSettings settings ("./IOCard/LCD1/config.ini", QSettings::IniFormat );
                    QString sNom = settings.value("TITRE/NOM").toString();
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    this->bLCD=true ;
                    this->CreerLCD(sNom,Type,0);

                }
                else if (Image=="POTAR1" || Image=="CTN1" || Image=="LDR1")
                {
                    QSettings settings ("./IOCard/"+Image+"/config.ini", QSettings::IniFormat );
                    QString sNom = settings.value("TITRE/NOM").toString();
                    QString   sCheminImage = "./IOCard/"+Image ;
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString  sCommande = "" ;
                    QVariant vCommande(BrocheAna) ;
                    QString Texte ="A";
                    sCommande += Texte ;
                    if(BrocheAna<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    int nConversion =0 ;
                    if(Image=="CTN1")
                    {
                        nConversion=1;
                    }
                    else if (Image=="LDR1")
                    {
                        nConversion=2;
                    }
                    else
                    {
                        nConversion=0;
                    }
                    this->CreerCapteurA(sNom,BrocheAna,sCommande,sCheminImage,nConversion);
                }
                else if (Image=="SERVO1")
                {
                    QSettings settings ("./IOCard/SERVO1/config.ini", QSettings::IniFormat );
                    QString sNom = settings.value("TITRE/NOM").toString();
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString Texte ="S";
                    sCommande += Texte ;
                    if(BrocheNum<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    this->CreerActionneurG(sNom,BrocheNum,sCommande);
                }
                else if (Image=="MOT1_DIR" || Image=="MOT2_DIR" || Image=="MOT1_PWM" || Image=="MOT2_PWM" || Image=="MOTEUR")
                {
                    QSettings   settings ("./IOCard/"+Image+"/config.ini", QSettings::IniFormat );
                    QString     sNom = settings.value("TITRE/NOM").toString();
                    QString     sCheminImage = "./IOCard/"+Image ;
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(sBrocheMoteur) ;
                    sCommande += vCommande.toString() ;
                    this->CreerMoteur(sNom,sCommande,Image,sCheminImage);
                }
                else if (GestionImage=="BTN1")
                {
                    QString sImage ="";
                    if (Image=="BTN1_BLACK")
                    {
                        sImage=Image ;
                    }
                    else if (Image=="BTN1_GREEN")
                    {
                        sImage=Image ;
                    }
                    else if (Image=="BTN1_WHITE")
                    {
                        sImage=Image ;
                    }
                    else if (Image=="BTN1_RED")
                    {
                        sImage=Image ;
                    }
                    else
                    {

                    }
                    QSettings settings ("./IOCard/"+sImage+"/config.ini", QSettings::IniFormat );
                    QString   sCheminImage = "./IOCard/"+sImage ;
                    QString sNom = settings.value("TITRE/NOM").toString();
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString Texte ="R";
                    sCommande += Texte ;
                    if(BrocheNum<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    this->CreerCapteurN(sNom,BrocheNum,sCommande,sCheminImage);
                }
                else if (Image=="BUZZER1")
                {
                    QSettings settings ("./IOCard/BUZZER1/config.ini", QSettings::IniFormat );
                    QString sNom = settings.value("TITRE/NOM").toString();
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString Texte ="O";
                    sCommande += Texte ;
                    if(BrocheNum<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    this->CreerBuzzer(sNom,BrocheNum,sCommande);
                }
                else if (GestionImage=="LED1")
                {
                    QString sImage ="";
                    if (Image=="LED1_RED")
                    {
                        sImage=Image ;
                    }
                    else if (Image=="LED1_GREEN")
                    {
                        sImage=Image ;
                    }
                    else if (Image=="LED1_YELLOW")
                    {
                        sImage=Image ;
                    }
                    else
                    {

                    }
                    QSettings settings ("./IOCard/"+sImage+"/config.ini", QSettings::IniFormat );
                    QString   sCheminImage = "./IOCard/"+sImage ;
                    QString sNom = settings.value("TITRE/NOM").toString();
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString Texte ="R";
                    sCommande += Texte ;
                    if(BrocheNum<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    this->CreerActionneurB(sNom,BrocheNum,sCommande,sCheminImage);
                }
                else if (Image=="SONAR1_PULSE")
                {
                    QString sCommande = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString Texte ="Q";
                    sCommande += Texte ;
                    if(BrocheNum<10)
                    {
                        sCommande += "0";
                    }
                    sCommande += vCommande.toString() ;
                    nSonarPulse=BrocheNum ;
                    sSonarPulse=sCommande ;
                    bSonarPulse=true ;
                }
                else if (Image=="SONAR1_ECHO")
                {
                   QVariant vCommande(BrocheNum) ;
                   QString sCommande ="";
                   if(BrocheNum<10)
                   {
                       sCommande += "0";
                   }
                   sCommande=vCommande.toString() ;
                   nSonarEcho=BrocheNum;
                   sSonarEcho=sCommande;
                   bSonarEcho=true ;
                }
                else //if (Image=="SWITCH1" || Image=="ILS1" || Image=="BARI1")
                {
                    QSettings settings ("./IOCard/"+Image+"/config.ini", QSettings::IniFormat );
                    QString   sNom         = settings.value("TITRE/NOM").toString();
                    QString   sTexte1      = settings.value("TEST1/Nom").toString();
                    QString   sCode1       = settings.value("TEST1/Code").toString();
                    QString   sTexte2      = settings.value("TEST2/Nom").toString();
                    QString   sCode2       = settings.value("TEST2/Code").toString();
                    sTexte1.chop(1);
                    sTexte2.chop(1);
                    sCode1=sCode1.mid(3);
                    sCode2=sCode2.mid(3);
                    QString   sCheminImage = "./IOCard/"+Image ;
                    for (register int i =0; i<sNom.length();i++)
                    {
                        sNom[i]=sNom[i].toUpper();
                    }

                    QString sTexte = "" ;
                    QVariant vCommande(BrocheNum) ;
                    QString sCommande ="";
                    if((Type=="NI" && GestionImage!="SONA") || (Type == "NO" && GestionImage!="SONA"))
                    {
                        sCommande="R";
                        if(BrocheNum<10)
                        {
                            sTexte += "0";
                        }
                        sCommande += sTexte ;
                        sCommande += vCommande.toString() ;
                        this->CreerFenetreDeBase(sNom,Type,BrocheNum,sCommande,sCheminImage,sTexte1,sCode1,sTexte2,sCode2);
                    }
                    else if (Type=="AI")
                    {
                        sCommande="A" ;
                        if(BrocheAna<10)
                        {
                            sTexte += "0";
                        }
                        sCommande += sTexte ;
                        sCommande += vCommande.toString() ;
                        this->CreerFenetreDeBase(sNom,Type,BrocheAna,sCommande,sCheminImage,sTexte1,sCode1,sTexte2,sCode2);
                    }
                }
            }
        }
        if(bSonarEcho==true && bSonarPulse==true)
        {
            sCommandeSONAR=sSonarPulse+QString::number(nSonarEcho) ;
            QString sType = "SONAR" ;
            this->CreerFenetreDeBase("SONAR ULTRASON",sType,nSonarPulse,sCommandeSONAR,"","","","","");
        }

        this->on_BtAfficher_clicked();
        this->on_BtHorizontal_clicked();
}

QByteArray  f_Supervision::SimulerEnvoiDonnees(QByteArray Commande)
{
    qDebug() << "SIMULATION : " << Commande;
    QByteArray Retour="";

    QChar CharCommande  (Commande[0]);


    switch (CharCommande.toLatin1()) {
    case 'V':
        Retour = "SIMULATION";
        break;

    case 'N':
        Retour = "MODEL NAME : SIMULATION\r\nVERSION : 1.0";
        break;

    case 'M':
        for(register int i = 0; i < this->ListeFenetre.length(); i++)
        {
            if(this->ListeFenetre[i]->getType() == MOTEUR)
            {
                F_Moteur* pFenetreMoteur    (qobject_cast<F_Moteur*>(this->ListeFenetre[i]->getWidget()));

                if(pFenetreMoteur->getsBrocheMoteur() == Commande.mid(1, 2))
                {
                    Retour = pFenetreMoteur->SimulerCommande(Commande);
                }
            }
        }
        break;

    case 'I':
        Retour = "VALUE=-1";
        break;

    case 'W':
        for(register int i = 0; i < this->ListeFenetre.length(); i++)
        {
            if(this->ListeFenetre[i]->getType() == ACT_B)
            {
                f_ActionneurB* pFenetreAct    (qobject_cast<f_ActionneurB*>(this->ListeFenetre[i]->getWidget()));

                if(pFenetreAct->getCommandeEtat().right(2) == Commande.mid(1, 2))
                {
                    Retour = pFenetreAct->SimulerCommande(Commande);
                }
            }
        }
        break;

    case 'R':
            for(register int i = 0; i < this->ListeFenetre.length(); i++)
            {
                if(this->ListeFenetre[i]->getType() == CAP_N)
                {
                    f_CapteurN* pFenetreCap    (qobject_cast<f_CapteurN*>(this->ListeFenetre[i]->getWidget()));

                    if(pFenetreCap->getCommandeEtat().right(2) == Commande.mid(1, 2))
                    {
                        Retour = pFenetreCap->SimulerCommande(Commande);
                    }
                }
            }
            break;

    case 'A':
    case 'a':
        for(register int i = 0; i < this->ListeFenetre.length(); i++)
        {
            if(this->ListeFenetre[i]->getType() == CAP_A)
            {
                f_CapteurA* pFenetreCap    (qobject_cast<f_CapteurA*>(this->ListeFenetre[i]->getWidget()));

                if(pFenetreCap->getCommandeEtat().right(2) == Commande.mid(1, 2))
                {
                    Retour = pFenetreCap->SimulerCommande(Commande);
                }
            }
        }
        break;

    case 'S':
        for(register int i = 0; i < this->ListeFenetre.length(); i++)
        {
            if(this->ListeFenetre[i]->getType() == ACT_G)
            {
                f_ActionneurG* pFenetreAct    (qobject_cast<f_ActionneurG*>(this->ListeFenetre[i]->getWidget()));

                if(pFenetreAct->getCommandeEtat().right(2) == Commande.mid(1, 2))
                {
                    Retour = pFenetreAct->SimulerCommande(Commande);
                }
            }
        }
        break;

    case 'Q':
        Retour = "VALUE=0";
        break;

    case 'E':
    case 'J':
    case 'P':
    case 'O':
        Retour = QString("DONE " + Commande).toStdString().c_str();
        break;

    case 'L':

        break;

    default:
        break;
    }

    Retour += "\r\n";

    return Retour;
}

void f_Supervision::PurgerListeFenetre()
{
    for(register int i = 0; i < this->ListeFenetre.length(); i++)
    {
        delete this->ListeFenetre[i];
    }

    this->ListeFenetre.clear();
}
