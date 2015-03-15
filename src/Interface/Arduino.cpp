//-------------------------------------------------------------------------------
/**
 * @file        Arduino.cpp
 * @brief       Connexion, détection, lecture et écriture de l'Arduino
 *
 * @author      K.BATARD
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       18/03/14
 * @version     1.0
 * @date        15/04/14
 *
 * Cette classe va permettre que lorsque la maquette est déconnectée il y a possibilité
 * de la reconnecter automatiquement. Elle va permettre également de communiquer avec l'Arduino.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------
//=====   En-Têtes Personnels   =====
#include "Arduino.h"

//=====   En-Têtes standards    =====
#include <QDebug>
#include <iostream>
using namespace std ;



/**     Constructeur de la classe Arduino
 *      La création du timer avec un temps d'intervalle définit à 1 seconde
 *      permettant le lancement de la méthode Detection().
 *      Tous les attributs de la classe sont initialisés.
 *
 **/
Arduino::Arduino(QObject *parent) :
    QObject(parent),
    Tampon(0),
    PortSerie(0),
    pCompteur(0)
{
    // Timer de l'arduino
    this->pCompteur= new QTimer (this) ;                                    //Création du Timer
    this->pCompteur->setInterval(10) ;                                      //On définit un intervalle de 10ms pour le Timer
    connect(this->pCompteur,SIGNAL(timeout()),this, SLOT(Detecter())) ;     //Lorsque le temps est atteint, on lance Detecter()
    this->pCompteur->start() ;                                              //On lance le Timer
    this->DureeDeConnexion.setHMS(0, 0, 0, 0) ;                             //On initialise le compteur de temps de connexion à 0
}

/**   Destructeur de la classe Arduino, le port est fermé, détruit,
 *    et initialisé, ainsi que tous les autres attributs.
 *
 **/
Arduino::~Arduino()
{
    if(this->PortSerie != 0)                                                               //Si le port existe
    {
        this->PortSerie->close() ;                                                         //On le ferme
        disconnect(this->PortSerie, SIGNAL(readyRead()), this, SLOT(RecevoirDonnees())) ;  //On deconnecte le signal de lecture
        delete this->PortSerie ;                                                           //On supprime le port
    }
    if (this->pSimulation !=0)
    {
        delete this->pSimulation ;
        this->pSimulation=0 ;
    }
    this->PortSerie      = 0 ;                                                             //On l'initialise à 0
    delete this->pCompteur ;                                                               //On supprime le compteur
    this->pCompteur      = 0 ;                                                             //On l'initialise à 0
    this->RetourSim= "" ;
}

void Arduino::setSimulation(f_Supervision* pSup)
{
    pSimulation = pSup;
}


/**         Elle va permettre d'annuler la dernière commande selon l'émetteur pour ne pas bloquer la file
 *          en cas de mode simulation
 *  @param  EmetteurCommande Emetteur, contient le dernier émetteur
**/
void Arduino::AnnulerDerniereCommande(EmetteurCommande Emetteur)
{
    int Index(this->FileEmetteur.indexOf(Emetteur));                //On définit dans un entier le premier émetteur dans la liste
    this->FileEmetteur.removeAt(Index);                             //On supprime cet émetteur
}

/**    On stocke dans le QByteArray "Donnees" ce que nous envoie l'Arduino
 *     et on le transmet à une mémoire tampon.
 *
 **/
void Arduino::RecevoirDonnees()
{

    QByteArray Donnees = PortSerie->readAll() ;              //Lecture de l'Arduino et stockage dans un QByteArray

    this->RemplirTampon(Donnees) ;                           //On envoie le retour de la lecture dans la mémoire tampon

}

/**     Elle permet de stocker la totalité du message de retour
 *      en attendant un retour chariot et de fin de ligne.
 * @param   QByteArray Tableau, reçoit le retour de commande de RecevoirDonnees.
 **/
void Arduino::RemplirTampon(QByteArray Tableau)
{

    this->Tampon +=  Tableau ;                                      //Le QByteArray Tampon reçoit les caractères de Tableau

    QString     DetecterFinTransmission("\r\n") ;                   //Retour chariot et fin de ligne
    QByteArray  FinDeTampon ("" );
    QByteArray  DebutTampon ("") ;

    for(register int i = 0; i < this->Tampon.length(); i++)         //On parcourt la totalité du Tampon
    {
        if(this->Tampon.mid(i, 2) == DetecterFinTransmission)       //Si le Tampon détecte le retour chariot et le retour à la ligne
        {
            DebutTampon = this->Tampon.left(i) ;                    //DebutTampon prend la valeur des premiers caractères de gauche du tampon
            FinDeTampon = this->Tampon.mid(i+2) ;                   //FinDeTampon prend la valeur 2 caractères après la DebutTampon

            if(DebutTampon.left(5)=="MODEL" || DebutTampon.left(5)=="VERSI")  //Si le Tampon contient le nom ou la version de l'Arduino on ne fais rien
            {
                //qDebug () << "DebutTampon" << DebutTampon;

                //qDebug () << "FinTampon" << FinDeTampon;
            }
            else
            {
                emit DonneesALire(DebutTampon) ;                    //On émet le signal de données à lire avec sa valeur.

                if(!this->FileEmetteur.isEmpty())
                {
                    switch(this->FileEmetteur.dequeue())                //Permet de différencier le type de retour
                    {
                    case SUPW :
                        //emit RetourCommandeSupervisionWEB(DebutTampon); //Retour de commande pour la supervision Web
                        RemplirQueueSupervisionWeb(DebutTampon) ;
                        break;

                    case ORG :
                        emit RetourCommandeOrganigram(DebutTampon) ;    //Retour de commande pour l'Organigram
                        break ;

                    case SUPV :
                        emit RetourCommandeSupervision(DebutTampon) ;   //Retour de commande pour la Supervision
                        break;
                    case GEN:
                        emit RetourCommandeGenerale(DebutTampon);       //Retour de commande pour l'interpréteur
                    default :
                        break ;
                    }
                }
            }

            this->Tampon.clear() ;                                  //On vide la mémoire Tampon.

            if(!FinDeTampon.isEmpty())                              //Si le Tampon n'est pas vide
            {
                this->RemplirTampon(FinDeTampon) ;                  //On relance la fonction avec FinDeTampon
            }
            else
            {
                this->Tampon.clear() ;                              //Sinon on vide le Tampon
                emit RetourCommandeSupervisionWEB(QueueValeurCommandeSupW) ;
            }
        }
    }
}

/**         Elle permet d'envoyer des instructions à l'Arduino en mentionnant la commande et l'émetteur
 * @param   QString sCommande, correspond à la commande (Exemple: W131, et EmetteurCommande Emetteur, spécifie le type de retour (ORG/SUPV/..).
 **/
bool Arduino::EnvoyerDonnees(QString sCommande, EmetteurCommande Emetteur)
{
    QByteArray  TabCommande = "";
    TabCommande.append(sCommande);
    return this->EnvoyerDonnees(TabCommande, Emetteur);
}

/**         Surcharge de la méthode précédente mais en reçevant des QByteArray au lieu de QString, nécessaire pour M. JARNOUX
 * @param   QByteArray sCommande, correspond à la commande (Exemple: W131, et EmetteurCommande Emetteur, spécifie le type de retour (ORG/SUPV/..).
 **/
bool Arduino::EnvoyerDonnees(QByteArray sCommande, EmetteurCommande Emetteur)
{
    bool bRetour (false) ;                                                          //Booléen de Retour

    if (this->bArduinoUtilisable)                                                   // Si le port de l'Arduino existe
    {
        qint64 nReponse (0) ;                                                       //Déclaration de la réponse de l'écriture
        nReponse = this->PortSerie->write(sCommande) ;                              //On envoie la commande et on range le retour dans nReponse

        if(nReponse != -1)                                                         //Si il n'y a pas d'erreur
        {
            qDebug() << "Donnees envoyée! " << sCommande;
            this->FileEmetteur.enqueue(Emetteur) ;                                  //On ajoute l'émmeteur à la file d'émetteur
            bRetour = true ;                                                        //On met bRetour à TRUE
        }
    }
    else
    {
        this->FileEmetteur.enqueue(Emetteur) ;                                  //On ajoute à la file le dernier émetteur

        this->RetourSim = this->pSimulation->SimulerEnvoiDonnees(sCommande);    //Retour de commande pour la simulation
        QTimer::singleShot(1, this, SLOT(RetourCommandeSimulation()));          //On appelle une seule fois au bout de 1s RetourCommandeSimulation()

        bRetour = true ;                                                        //On met bRetour à TRUE

    }

    return bRetour ;                                                            //On retourne bRetour
}

void Arduino::RetourCommandeSimulation()
{
    this->RemplirTampon(this->RetourSim);
}

/**
 *          Elle permet d'énumérer la totalité des ports séries connectés à l'ordinateur, de n'en garder que le port
 *          où l'Arduino est connecté. De lancer la méthode de connexion et de l'ouvrir.
 *          Elle permet aussi de lancer le compteur qui définit le nombre de temps que l'Arduino est connecté
 *
 **/
void Arduino::Detecter()
{
    QextSerialEnumerator    Enumerateur ;                                   //L'objet Enumerateur mentionnant tous les ports séries
    QList<QextPortInfo>     ListePortsCOM = Enumerateur.getPorts() ;        //on met ces informations dans une liste

    //on parcourt la liste des ports

    bool bArduinoPresent(false) ;                                           //Booléen à true si l'Arduino est présent
    QString     sNomPortCOM ("") ;                                          //sNom va permettre de récuperer le nom du port (EX:COM7)
    for(signed register int i=0; i < ListePortsCOM.length(); i++)           //On parcours la totalité des ports séries détectés
    {
        //FONCTIONNEL WINDOWS ET LINUX !!!!
        if(ListePortsCOM[i].vendorID == 0x2341)                     //Si parmi tous les ports, un port avec le VID à 2341 alors l'Arduino est détecté
        {
            bArduinoPresent = true ;                                //On met le booléen à true pour indiquer que l'Arduino est là
            sNomPortCOM = ListePortsCOM[i].portName ;               //sNom est complété par le numéro de port de l'Arduino
        }
    }

    if(bArduinoPresent)                                              //Si l'Arduino est présent
    {
        //qDebug() << "Arduino Détecté" ;                            //On affiche un message indiquant sa détection
        if(this->PortSerie == 0)                                     //Si le port est nul
        {
            this->Connecter(sNomPortCOM) ;                           //On lance la connexion avec le numéro de port
        }
        else
        {
            this->DureeDeConnexion = this->DureeDeConnexion.addMSecs(10) ; //On incrémente le compteur de 10 ms

            if(!this->PortSerie->isOpen())                           //Si le port n'est pas ouvert
            {
                this->PortSerie->open(QextSerialPort::ReadWrite) ;   //On l'ouvre en lecture et écriture
            }
        }
    }
    else
    {

        //qDebug() << "Arduino Non Détecté" ;                        //Message indiquant que l'Arduino est non détecté
        this->Deconnecter() ;                                        //On lance la déconnexion
    }

    emit Signal_Temps(this->DureeDeConnexion) ;                      //On émet le signal qui va permettre d'incrémenter le compteur de temps de connexion de l'Arduino
}

/**         Elle permet de connecter l'Arduino en l'initialisant selon les règles du port série
 *          Elle émet un signal pour indiquer l'état de connectivité de l'Arduino.
 *
 * @param   QString sNomPort, correspont au nom du port émis lors de la détection
 **/
void Arduino::Connecter(QString sNomPort)
{
    //qDebug() << "Connexion :" ;                                                //On essaie de faire la connexion avec la carte Arduino
    bool bOuverture (false) ;                                                    //Booléen qui va détecter l'ouverture


    this->PortSerie = new QextSerialPort() ;                                     //On créé le port

    this->PortSerie->setPortName(sNomPort) ;                                     //On lui applique le port COM passé en paramètre
    this->PortSerie->setBaudRate(BAUD9600) ;                                     // On règle la vitesse du port
    this->PortSerie->setParity(PAR_NONE) ;                                       // On définit le bit de parité
    this->PortSerie->setStopBits(STOP_1) ;                                       // On définit le nombre de bit de stop
    this->PortSerie->setDataBits(DATA_8) ;                                       // On définit le nombre de bits de données
    this->PortSerie->setFlowControl(FLOW_OFF) ;                                  // Pas de contrôle de flux


    bOuverture = this->PortSerie->open(QextSerialPort::ReadWrite) ;              // Ouverture du port en écriture et lecture
    if(!bOuverture)                                                              // Test d'ouverture du port
    {
        emit ArduinoConnect(PORT_FERME, sNomPort) ;                              //On émet que le port est fermé avec son nom
        //qDebug () << " Port Fermé " ;                                          //Affichage port fermé
        //qDebug() << this->PortSerie->errorString();                            //On affiche l'erreur d'ouverture si possible
    }
    else
    {
        emit ArduinoConnect(CONNECTE, sNomPort) ;                                //On émet que l'Arduino est connecté et à quel port
        //qDebug () << "Port ouvert " ;                                          //Affichage port ouvert
    }
    connect(this->PortSerie,SIGNAL(readyRead()),this,SLOT(RecevoirDonnees())) ;  //Lorsque le port série est prêt à lire, on exécute RecevoirDonnees

    QTimer::singleShot(2000, Qt::CoarseTimer, this, SLOT(UtiliserArduino()));    //On appel une seule fois UtiliserArduino() au bout de 2 s
}

void Arduino::UtiliserArduino()
{
    this->bArduinoUtilisable = true;
}

/**      Elle permet de Deconnecter l'Arduino et de remettre à zéro le compteur
 *
 **/
void Arduino::Deconnecter()
{
    emit ArduinoConnect(DECONNECTE, "") ;                                                   //On émet que l'Arduino est déconnecté

    this->DureeDeConnexion.setHMS(0, 0, 0, 0) ;                                             //On met le compteur à zéro

    if(this->PortSerie != 0)                                                                //Si le port série n'est pas nul(Fermé, utilisé dans une autre application par exemple)
    {
        //qDebug() << "Deconnexion" ;                                                       //On affiche que l'on va le déconnecter
        this->PortSerie->close() ;                                                          //On ferme le port
        disconnect(this->PortSerie, SIGNAL(readyRead()), this, SLOT(RecevoirDonnees())) ;   //On déconnecte les signaux précédents
        delete this->PortSerie ;                                                            //On supprime le port
    }

    this->bArduinoUtilisable = false;                                                       //On dit que l'Arduino n'est pas utilisable

    this->PortSerie = 0 ;                                                                   //On initialise le port à 0
}

/**     Elle permet d'émettre l'état de l'Arduino
 *
 **/
void Arduino::DemanderEtat()
{
    if(this->PortSerie == 0)                                                  //Si le port est nul
    {
        emit ArduinoConnect(DECONNECTE, "") ;                                 //On envoie le signal indiquant qu'il est déconnecté
    }
    else
    {
        if(this->PortSerie->isOpen() && this->bArduinoUtilisable)             //Si le port est ouvert et utilisable
        {
            emit ArduinoConnect(CONNECTE, this->PortSerie->portName()) ;      //On envoie le signal indiquant qu'il est connecté ainsi que son numéro de port
        }
        else
        {
            emit ArduinoConnect(PORT_FERME, this->PortSerie->portName()) ;    //On envoie le signal indiquant que le port est fermé ainsi que son numéro de port
        }
    }
}


void Arduino::RemplirQueueSupervisionWeb(QByteArray QueueRetour)
{
    /*if (!Tampon.isEmpty())
    {
        this->QueueValeurCommandeSupW.enqueue(QueueRetour) ;
    }
    else
    {
        emit RetourCommandeSupervisionWEB(QueueValeurCommandeSupW) ;
    }*/

    QueueValeurCommandeSupW.enqueue(QueueRetour);
}
