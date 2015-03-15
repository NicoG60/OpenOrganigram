
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

#include "f_AffichageFenetre.h"

//=====   En-Têtes standards    =====
#include <QDebug>


/** Description détaillée de la méthode
 * @pre Constructeur par défaut
 *
 **/
f_Supervision::f_Supervision(Arduino *pControleur,QWidget *parent) : // Constructeur
    QWidget(parent),
    ui(new Ui::f_Supervision),
    pArduino(pControleur),
    nIndiceFenetre(0),
    Compteur(0),
    bAttenteReponse(false)
{
     this->ui->setupUi(this) ;

    pArduino->setSimulation(this);

    // Timer de l'arduino
    this->CompteurSup.setInterval(200);                                                   //On définit un intervalle  pour le Timer
    connect(&(this->CompteurSup),SIGNAL(timeout()),this, SLOT(on_TempsFinit()));              //Lorsque le temps est atteint, on lance Detection

    this->connect(pArduino, SIGNAL(RetourCommandeSupervision(QByteArray)), this, SLOT(RetourCommande(QByteArray))); //On récupère le signal de retour de commande de supervsion au retour de commande de la classe


}


/**  Destructeur
 *
 **/
f_Supervision::~f_Supervision() // Destructeur
{
    this->PurgerListeFenetre();

    this->pArduino=0;
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
    f_AffichageFenetre dialog(ListeFenetre, this);

    dialog.exec();
}

/**         Permet d'envoyer les données à l'Arduino
 * @param   sCommande= correspond à la commande
 *
 **/
void f_Supervision::EnvoyerDonneesSup(QString sCommande)
{
    if(!sCommande.isEmpty())                                                        //Si la commande n'est pas vide
    {
        if(!this->bAttenteReponse)                              //Et si on n'est pas en attente de réponse
        {
            bool bReponse (false);                                                  //bRetour à false
            bReponse = this->pArduino->EnvoyerDonnees(sCommande, SUPV);    //On récupère dans un bool l'état de l'envoi de commande pour voir si elle a réussi ou non

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
                this->pArduino->AnnulerDerniereCommande(SUPV);             //On annule aussi le dernier émetteur
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
    if(this->CompteurSup.isActive())       //Si le compteur est lancé et actif
    {
        this->CompteurSup.stop();                                     //On stop le timer
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
            if(this->ListeFenetre[i]->getWidget()->getCommandeSupervision() == CommandeCourante)       //Et on vérifie quelle commande correspond à quelle fenêtre
            {
                FenetreRetour = this->ListeFenetre[i]->getWidget();                             //On récupère le type de fenêtre
            }
        }

        this->FileCommande.dequeue();                                                           //On supprime la dernière commande

        if(FenetreRetour != 0)                                                                  //Si il n'est pas vide
        {
            FenetreRetour->RetourCommande(ValeurRetour);
        } 
    }

    if(Redemarrer)                                                                              //Si on peut lancer la reconnexion
    {
        this->CompteurSup.start();                                                            //On relance le timer
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
        this->ListeFenetre[Indice]->getWidget()->Superviser() ;    //On met dans un QString les commandes de chaque fenêtre
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
    this->CompteurSup.start();    //On lance le timer
    QWidget::showEvent(e);
}


/**  Permet d'arrêter le timer lorsque la supervision est fermée
 *
 **/
void f_Supervision::hideEvent(QHideEvent * e)
{
    qDebug() << "Supervision Hide";
    this->CompteurSup.stop();     //On arrête le timer
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

    QStringList ListeGroupe(FichierIni.childGroups());

    for(register int i = 0; i < ListeGroupe.length(); i++)
    {
        if(ListeGroupe[i].contains("BROCHE"))
        {
            qDebug() << ListeGroupe[i];
            f_SubWindow*        NouvelleFenetre(new f_SubWindow);
            f_WidgetInterne*    NouveauWidget;
            QString             Broche;
            FichierIni.beginGroup(ListeGroupe[i]);

            QString Type(FichierIni.value("Type").toString());

            if(Type == "NI")
            {
                Broche = FichierIni.value("Broche_Numerique").toString();
                NouveauWidget = new WidgetNI(FichierIni.value("Iocard").toString(), Broche);
            }
            else if(Type == "NO")
            {
                Broche = FichierIni.value("Broche_Numerique").toString();
                NouveauWidget = new WidgetNO(FichierIni.value("Iocard").toString(), Broche);
            }
            else if(Type == "AI")
            {
                Broche = FichierIni.value("Broche_Analogique").toString();
                NouveauWidget = new WidgetAI(FichierIni.value("Iocard").toString(), Broche);
            }
            else if(Type == "SRV")
            {

            }
            else if(Type == "MOT1")
            {

            }
            else if(Type == "TC")
            {

            }
            else if(Type == "I2C")
            {

            }
            else if(Type == "RTC")
            {

            }

            NouvelleFenetre->setWidget(NouveauWidget);
            ui->ZoneMDI->addSubWindow(NouvelleFenetre);
            NouvelleFenetre->setWindowTitle(NouveauWidget->getTitre());
            ListeFenetre.append(NouvelleFenetre);

            FichierIni.endGroup();
        }
    }

    for(register int i = 0; i < ListeFenetre.length(); i++)
        ListeFenetre[i]->setVisible(true);
}

QByteArray  f_Supervision::SimulerEnvoiDonnees(QByteArray Commande)
{
    qDebug() << "SIMULATION : " << Commande;
    QByteArray Retour="";
/*
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
*/
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
