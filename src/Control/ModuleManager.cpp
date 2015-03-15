//-------------------------------------------------------------------------------
/**
 * @file        ModuleManager.cpp
 * @brief       Gestionnaire de module
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        25/02/14
 *
 * Gestionnaire d'un organigramme d'un module en particulier. Il gèrera l'exécution de cet
 * organigramme ainsi que lordonnancement dexs différente instructions à travers les routines.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====
#include <QMessageBox>
#include <QDebug>

//=====   Headers Peros   =====
#include "ModuleManager.h"

/**
 * Constructeur de l'objet manager de programme.
 *
 * @brief   ModuleManager::ModuleManager(QString sNomModule = "Module sans nom", QObject *  pParent = 0)
 * @param   pParent     Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     Routine
 */
ModuleManager::ModuleManager(QString sNomModule, Arduino *pArduino, QObject *pParent) :
    QObject                 (pParent) ,
    sNomModule              (sNomModule) ,
    pTabW_AfficheurRoutine  (new QTabWidget),
    Temporisation           (INTERVAL_DEFAULT),
    CompteurTimeout         (0),
    CompteurARU             (0),
    ModeDebug               (false),
    AttenteReponse          (false),
    pArduino                (pArduino)
{    
    //Initialisation de l'afficheur de routine
    this->pTabW_AfficheurRoutine->setDocumentMode(true) ;
    this->pTabW_AfficheurRoutine->setMovable(true) ;
    this->pTabW_AfficheurRoutine->setTabPosition(QTabWidget::South) ;
    this->pTabW_AfficheurRoutine->setTabShape(QTabWidget::Triangular) ;

    //Initialisation des variables
    this->InitVariable();

    //Connexion signaux/slots
    connect(&(this->TimerExec), SIGNAL(timeout()), this, SLOT(ExecuterCommandeSuivante()));
    connect(this->pArduino, SIGNAL(RetourCommandeOrganigram(QByteArray)), this, SLOT(RetourCommande(QByteArray)));
    connect(&(this->TimerARU), SIGNAL(timeout()), this, SLOT(ExecuterARUSuivant()));


}

/**
 * Destructeur de l'objet
 *
 * @brief   ModuleManager::~ModuleManager()
 * @see     Routine
 */
ModuleManager::~ModuleManager()
{
    for(register int i = 0 ; i < ListeRoutines.length() ; i++)
    {
        if(ListeRoutines[i] != NULL)
        {
            delete ListeRoutines[i];
            ListeRoutines[i] = 0;
        }
    }

    if(pTabW_AfficheurRoutine != NULL)
    {
        delete pTabW_AfficheurRoutine ;
        pTabW_AfficheurRoutine = 0 ;
    }

}

/**
 * Ajout d'une nouvelle routine dans le module
 *
 * @brief   ModuleManager::AjouterRoutine(QString sNomRoutine)
 * @param   sNomRoutine Nom de la routine, obligatoire et unique
 * @see     Routine
 */
void ModuleManager::AjouterRoutine(QString sNomRoutine)
{
    //Préparation du texte des mesages d'erreur...
    QString Titre   ("Impossible de créer la routine") ;
    QString Message ("La routine doit avoir un nom") ;

    //Si il y a un nom de routine
    if(!sNomRoutine.isEmpty())
    {
        //On ajoute la routine
        Routine *   pNouvelleRoutine (new Routine(this->ListeRoutines.length(), sNomRoutine, this)) ;
        this->AjouterRoutine(pNouvelleRoutine);
    }
    else
    {
        QMessageBox::warning(0, Titre, Message, QMessageBox::Ok);
    }
}

void ModuleManager::AjouterRoutine(Routine *    pRout)
{
    //Si le pointeur n'est pas nul
    if(pRout != 0)
    {
        //On vérifie que le nom n'existe pas déjà
        bool    bExiste (false) ;

        for(register int i = 0; i < this->ListeRoutines.length(); i++)
        {
            bExiste = (pRout->getNom() == ListeRoutines[i]->getNom()) ;
        }

        //S'il n'existe pas
        if(!bExiste)
        {
            //On ajoute la routine
            ListeRoutines.append(pRout) ;

            //Ajout de la vue
            Vue *       pNouvelleVue    (new Vue(pRout)) ;
            ListeVues.append(pNouvelleVue) ;
            pNouvelleVue->Rafraichir() ;

            //Ajout d'une tab au widget
            pTabW_AfficheurRoutine->addTab(pNouvelleVue, pRout->getNom()) ;
        }
        else
        {
            QMessageBox::critical(0, "Ajout de routine", "Une routine du même nom existe déjà", QMessageBox::Ok);
        }
    }

}

/**
 * Supprime la routine dont le nom est passé en paramètre
 *
 * @brief ModuleManager::SupprimerRoutine
 * @param sNomRoutine   Le nom de la routine
 */
void ModuleManager::SupprimerRoutine(QString sNomRoutine)
{
    for(register int i = 0; i < this->ListeRoutines.length(); i++)
    {
        if(sNomRoutine == this->ListeRoutines[i]->getNom())
        {
            this->SupprimerRoutine(i);
        }
    }
}

/**
 * Supprime la routine dont l'id est passé en paramètre
 *
 * @brief ModuleManager::SupprimerRoutine
 * @param nId   Identifiant de la routine
 */
void ModuleManager::SupprimerRoutine(int nId)
{
    //Si l'id est bien dans une plage acceptable, on la supprime
    if(nId >= 0 && nId < this->ListeRoutines.length())
    {
        Routine * RoutineASuppr (this->ListeRoutines.takeAt(nId));
        delete RoutineASuppr;
        RoutineASuppr = 0;
    }
}

/**
 * Compte le nombre d'instruction
 * @brief ModuleManager::getNbreInst
 * @return Le nombre d'instruction totale
 */
int ModuleManager::getNbreInst()
{
    int Retour  (0);

    for(register int i = 0; i < ListeRoutines.length(); i++)
    {
        Retour += ListeRoutines[i]->getNbreInst();
    }

    return Retour;
}

/**
 * Retourne la valeur d'une variable, si la variable à laquelle on essais d'acceder n'éxiste pas, la fonction renvois 65535 comme
 * erreur (Les varialbes n'allant que jusqu'à 65025, cela permet de détecter les erreur)
 *
 * @brief   ModuleManager::getValVar(unsigned int nIndice)
 * @param   nIndice L'indice de la variable ciblée (entre 0 et 25)
 * @return  La valeur de la varialbe ciblée
 */
unsigned short  ModuleManager::getValVar(unsigned int nIndice)
{
    unsigned short  Retour  (65535) ;
    if(nIndice <= 25)
    {
        Retour = this->ListeVariables[nIndice] ;
    }

    return Retour;
}

/**
 * Affecte une valeur à une variable
 *
 * @brief   ModuleManager::setValVar(unsigned int nIndice, unsigned short nVal)
 * @param   nIndice L'indice de la variable ciblée (entre 0 et 25)
 * @param   nVal    La valeur à affecter
 */
void ModuleManager::setValVar(unsigned int nIndice, unsigned short nVal)
{
    if(nIndice <= 25)
    {
        if(nVal <= 65025)
        {
            this->ListeVariables[nIndice] = nVal ;
        }
        else
        {
            this->ListeVariables[nIndice] = 65025 ;
        }
    }
}

/**
 * Envoie une commande à l'arduino
 * @brief ModuleManager::EnvoyerCommande
 * @param sCommande la commande à envoyer
 */
void ModuleManager::EnvoyerCommande(QString sCommande)
{
    //Si la commande est envoyée sans soucis, on se met en attente de réponse
    if(this->pArduino->EnvoyerDonnees(sCommande, ORG))
        this->AttenteReponse = true ;
}

/**
 * Fonction appelée à chaque retour de commande destiné au module manager
 * @brief ModuleManager::RetourCommande
 * @param sRetour Le tableau d'octet retourné par la maquette
 */
void ModuleManager::RetourCommande(QByteArray sRetour)
{
    this->DerniereReponse   = sRetour ;
    this->AttenteReponse    = false ;
    qDebug() << "RetourCommande Org ==> " << sRetour;
}

/**
 * Initialise l'environnement d'exécution
 * @brief ModuleManager::InitExecution
 */
void ModuleManager::InitExecution()
{
    //Passe en mode "non attente"
    this->AttenteReponse = false ;

    //Place la première instruction de la routine principale dans la pile
    IdentifiantInst PointeurDebut;

    PointeurDebut.nIdRoutine = 0;
    PointeurDebut.nIdInstruction = 0;

    this->PileExec.push(PointeurDebut);

    //Déselectionne tous les items de toutes les vues
    for(register int i = 0; i < this->ListeVues.length(); i++)
    {
        ListeVues[i]->DeselectionnerItems();
    }
}

/**
 * Lance l'exécution normale (sans debug) de l'organigramme
 * @brief ModuleManager::LancerExecution
 */
void ModuleManager::LancerExecution()
{
    //Met le mode pas à pas à faux
    this->ModeDebug = false;

    //Emission du signal rendant compte de l'état d'exécution
    emit this->EtatExec(EN_EXEC);

    //Initialisation des variables
    this->InitVariable();

    //Désactivation du widget d'affichage : aucune interaction n'est possible lors de l'exec
    this->pTabW_AfficheurRoutine->setEnabled(false);

    //Démarrage du t'imer d'exécution
    this->TimerExec.setInterval(this->Temporisation);
    this->TimerExec.start();
}

/**
 * Arrête l'exécution de l'organigramme
 * @brief ModuleManager::StopperExecution
 */
void ModuleManager::StopperExecution()
{
    //Arrête du timer, emission du signal d'état d'exec et remise en service des interaction avec l'affichage
    this->TimerExec.stop();
    emit this->EtatExec(ARRETE);
    this->pTabW_AfficheurRoutine->setEnabled(true);

    //Remet l'attente de réponse à faux et le compteur de Timeout à 0
    this->AttenteReponse = false ;
    this->CompteurTimeout = 0;

    //Déselection de tout les items
    for(register int i = 0; i < this->ListeVues.length(); i++)
    {
        ListeVues[i]->DeselectionnerItems();
    }

    //Réiniçtialisation des variables
    this->InitVariable();

    //Déclenchement des commandes d'arrêt d'urgence
    this->TimerARU.start(1);
}

/**
 * Remet toutes les variables à 0
 * @brief ModuleManager::InitVariable
 */
void ModuleManager::InitVariable()
{
    for(register unsigned int i = 0; i < 26; i++)
    {
        this->ListeVariables[i] = 0;
    }
}

/**
 * Exécute la commande suivante dans l'organigramme
 * @brief ModuleManager::ExecuterCommandeSuivante
 */
void ModuleManager::ExecuterCommandeSuivante()
{
    //Si la pile n'est pas vide
    if(!this->PileExec.isEmpty())
    {
        //Si on est pas en attente d'une réponse de la maquette
        if(!this->AttenteReponse)
        {
            //On remet le compteur de timeout à 0 et on déclare les variables necessaire
            this->CompteurTimeout = 0;
            IdentifiantInst PointeurInst;
            Instruction *    InstExec    (0);

            //On récupère l'instruction du haut de la pile
            PointeurInst    = this->PileExec.pop() ;
            InstExec        = this->ListeRoutines[PointeurInst.nIdRoutine]->getInstruction(PointeurInst.nIdInstruction) ;

            //Récupération de la vue correspondante à la routine en cours d'exécution
            Vue *   VueExec (this->ListeVues[PointeurInst.nIdRoutine]);

            //Récupération de l'index de la vue
            int     IxdexTab(this->pTabW_AfficheurRoutine->indexOf(VueExec));

            //Bascule sur l'onglet correspondant à la routine en cours
            this->pTabW_AfficheurRoutine->setCurrentIndex(IxdexTab);

            //Récupération de l'item correspondant à l'instruction en cours d'exécution
            Item *  ItemExec(VueExec->getItem(PointeurInst.nIdInstruction));

            //On déselectionne tous les items de la vue
            VueExec->DeselectionnerItems();

            //On ne selectinone que celui en cours
            ItemExec->setSelected(true);

            //On exécute enfin l'instruction
            InstExec->Executer();

            //On définit l'interval avant la prochaine instruction (en cas de tempo, c'est ici que ça se joue)
            this->TimerExec.setInterval(this->Temporisation);
            this->Temporisation = INTERVAL_DEFAULT ; //RAZ de la tempo

            //Si on est en mode debug
            if(this->ModeDebug)
            {
                //On indique que le programme est en pause
                emit this->EtatExec(PAUSE);
            }
        }
        else //Si on est en attente d'une réponse
        {
            this->CompteurTimeout++; //On compte

            if(CompteurTimeout > 10) //Si on met plus de 10 essais (1 seconde) à réussir, on abandonne la commande
            {
                qDebug() << "Exec org : Timeout...";
                this->AttenteReponse = false;
                this->CompteurTimeout = 0;
                this->pArduino->AnnulerDerniereCommande(ORG);
            }
        }

    }
    else //Si la pile est vie, on arrête l'exécution
    {
        this->StopperExecution();
    }
}

/**
 * Exécution de l'arret d'urgence suivant
 * @brief ModuleManager::ExecuterARUSuivant
 */
void ModuleManager::ExecuterARUSuivant()
{
    if(!AttenteReponse) //Si on est pas en attente de réponse
    {
        //RAZ du compteur timeout
        this->CompteurTimeout = 0;

        //Si il reste des ARU à envoyer, on les envoie
        if(this->CompteurARU < this->ListeARU.length())
        {
            this->EnvoyerCommande(this->ListeARU[this->CompteurARU]);
            this->CompteurARU++;
        }
        else //Sinon on arrête le timer et on remet tout à zéro
        {
            this->CompteurARU = 0;
            this->TimerARU.stop();
        }
    }
    else //Si on attend une réponse
    {
        this->CompteurTimeout++; //On compte

        if(CompteurTimeout > 1000) //Si on met plus de 1000 essais (1 seconde) on abandonne la commande
        {
            qDebug() << "ARU : Timeout...";
            this->AttenteReponse = false;
            this->CompteurTimeout = 0;
        }
    }
}

/**
 * Ajoute une instruction dans la pile
 * @brief ModuleManager::AjouterALaPile
 * @param Identifiant Le couple Id d'instruction et Id de routine pour retrouver facilement une instruction
 */
void ModuleManager::AjouterALaPile(IdentifiantInst Identifiant)
{
    //Si la routine existe
    if(static_cast<int>(Identifiant.nIdRoutine) < ListeRoutines.length())
    {
        //si l'instruction existe
        if(static_cast<int>(Identifiant.nIdInstruction) < ListeRoutines[Identifiant.nIdRoutine]->getListe().length())
        {
            //On ajoute l'instruction
            this->PileExec.push(Identifiant);
        }
    }
}

/**
 * Mutateur de temporisation (temps entre 2 instruction, réinitialisé à une valeur par défaut à chaque tour)
 * @brief ModuleManager::setTemporisation
 * @param ms le nombre de milliseconde
 */
void ModuleManager::setTemporisation(unsigned int ms)
{
    this->Temporisation = ms ;
}

/**
 * Mode pas à pas : saute à l'instruction suivante et se met en pause
 * @brief ModuleManager::PasserInstSuiv
 */
void ModuleManager::PasserInstSuiv()
{
    this->ModeDebug = true;
    QTimer::singleShot(this->Temporisation, Qt::CoarseTimer, this, SLOT(ExecuterCommandeSuivante()));
    emit this->EtatExec(EN_EXEC);
}

/**
 * Demande à toutes les vues de se rafraichir
 * @brief ModuleManager::RafraichirVues
 */
void ModuleManager::RafraichirVues()
{
    for(int i = 0; i < this->ListeVues.length(); i++)
    {
        this->ListeVues[i]->Rafraichir();
    }
}

/**
 * Enregistre son contenu au format XML
 * @brief ModuleManager::Enregistrer
 * @param Parser    Le parseur à utiliser
 */
void ModuleManager::Enregistrer(QXmlStreamWriter* Parser)
{
    Parser->writeStartElement("Module");
    Parser->writeAttribute("sNomModule", this->sNomModule);

    for(register int i = 0; i < this->ListeActionPossible.length(); i++)
    {
        Parser->writeStartElement("ActionArduino");

        Parser->writeAttribute("sNomAction", ListeActionPossible[i].sNomAction);

        Parser->writeAttribute("sCommande", ListeActionPossible[i].sCommande);

        if(ListeActionPossible[i].bActive)
        {
            Parser->writeAttribute("bActive", "true");
        }
        else
        {
            Parser->writeAttribute("bActive", "false");
        }

        Parser->writeEndElement();
    }
    for(register int i = 0; i < this->ListeTestPossible.length(); i++)
    {
        Parser->writeStartElement("TestArduino");

        Parser->writeAttribute("sNomTest", ListeTestPossible[i].sNomTest);

        Parser->writeAttribute("sCommande", ListeTestPossible[i].sCommande);

        if(ListeTestPossible[i].bActive)
        {
            Parser->writeAttribute("bActive", "true");
        }
        else
        {
            Parser->writeAttribute("bActive", "false");
        }

        Parser->writeEndElement();
    }
    for(register int i = 0; i < this->ListeARU.length(); i++)
    {
        Parser->writeEmptyElement("ARU");
        Parser->writeAttribute("valeur", this->ListeARU[i]);
    }

    for(register int i = 0; i < ListeRoutines.length(); i++)
    {
        ListeRoutines[i]->Enregistrer(Parser);
    }

    Parser->writeEndElement();
}

/**
 * Ajoute un action à la liste d'action possible
 * @brief ModuleManager::AjouterAction
 * @param Action    Action à ajouter
 */
void ModuleManager::AjouterAction(ActionArduino Action)
{
    ListeActionPossible.append(Action);
}

/**
 * Ajoute une liste d'action à la liste d'action possible
 * @brief ModuleManager::AjouterAction
 * @param Action    Liste à ajouter
 */
void ModuleManager::AjouterAction(QList<ActionArduino> Action)
{
    ListeActionPossible.append(Action);
}

/**
 * Ajoute un test à la liste de tests possibles
 * @brief ModuleManager::AjouterTest
 * @param Test  Le test à ajouter
 */
void ModuleManager::AjouterTest(TestArduino Test)
{
    ListeTestPossible.append(Test);
}

/**
 * Ajoute une liste de tests à la liste de tests possibles
 * @brief ModuleManager::AjouterTest
 * @param Test  La list de test à ajouter
 */
void ModuleManager::AjouterTest(QList<TestArduino> Test)
{
    ListeTestPossible.append(Test);
}

QByteArray ModuleManager::Compiler()
{
    QByteArray Retour;


    return Retour;
}
