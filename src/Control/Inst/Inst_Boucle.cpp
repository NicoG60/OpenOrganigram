//-------------------------------------------------------------------------------
/**
 * @file        Inst_Loop.cpp
 * @brief       Instruction boucle
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Représente une boucle dans l'organigramme
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tester si la classe est fonctionnelle
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Compil_Inst.h"
#include "Inst_Boucle.h"
#include "../ModInst/f_ModInst_Boucle.h"

/**
 * Opérateur testant la différence entre 2 description de boucle
 *
 * @brief operator !=
 * @param a Premier éléments à comparer
 * @param b Second élément à comparer
 * @return Vrai s'il sont différents, faux sinon
 */
bool operator !=(const DescBoucle& a, const DescBoucle& b)
{
    //On teste toute les valeurs de la description
    bool Retour(a.DescriptionCondition.nIndiceVar           !=  b.DescriptionCondition.nIndiceVar &&
                a.DescriptionCondition.nValeur              !=  b.DescriptionCondition.nValeur &&
                a.DescriptionCondition.TCondition           !=  b.DescriptionCondition.TCondition &&
                a.DescriptionCondition.TestMat.bActive      !=  b.DescriptionCondition.TestMat.bActive &&
                a.DescriptionCondition.TestMat.sCommande    !=  b.DescriptionCondition.TestMat.sCommande &&
                a.DescriptionCondition.TestMat.sNomTest     !=  b.DescriptionCondition.TestMat.sNomTest &&
                a.DescriptionCondition.TypeComp             !=  b.DescriptionCondition.TypeComp &&
                a.TypeDeBoucle                              !=  b.TypeDeBoucle);

    return Retour;
}

/**
 * Constructeur
 *
 * @brief Inst_Boucle::Inst_Boucle
 * @param nId       Identifiant de l'instruction
 * @param pRoutine  Routine parente
 */
Inst_Boucle::Inst_Boucle(unsigned int nId, Routine *pRoutine) :
    Instruction (nId, pRoutine),
    pInstPaire  (0),
    TypeDeBoucle(TANT_QUE),
    EstActive   (true)
{
    //Initialisation de la condition à réaliser
    this->DescriptionCondition.nIndiceVar   = 0;
    this->DescriptionCondition.nValeur      = 0;
    this->DescriptionCondition.TCondition   = LOG;
    this->DescriptionCondition.TypeComp     = EGUAL;
}

Compil_Inst *Inst_Boucle::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    if(this->EstLePremier())
    {
        switch (this->TypeDeBoucle) {
        case TANT_QUE:
            Retour->AjouterUnOpCode(Inst_Cond::CompilationStatiqueInverseeSansLiens(this->DescriptionCondition));
            Retour->AjouterLienVers(this->getRoutine()->getInstruction(this->pInstPaire->getIdSuivantParSortie()[0])->getIdentifiantInst());
            break;
        case JUSQU_A:
            Retour->AjouterUnOpCode('\x0');
            break;
        }
    }
    else
    {
        switch (this->TypeDeBoucle) {
        case TANT_QUE:
            Retour->AjouterUnOpCode('\x12');
            Retour->AjouterLienVers(this->pInstPaire->getIdentifiantInst(), true);
            break;
        case JUSQU_A:
            Retour->AjouterUnOpCode(Inst_Cond::CompilationStatiqueSansLiens(this->DescriptionCondition));
            Retour->AjouterLienVers(this->getRoutine()->getInstruction(this->getIdSuivantParSortie()[0])->getIdentifiantInst());
            Retour->AjouterUnOpCode('\x12');
            Retour->AjouterLienVers(this->pInstPaire->getIdentifiantInst(), true);
            break;
        }
    }

    return Retour;
}

/**
 * Execute la boucle
 *
 * @brief   Inst_Boucle::Executer
 * @param   Manager Manager avec lequel éxecuter la boucle
 * @return  L'identifiant de l'instruction suivante
 */
void Inst_Boucle::Executer()
{
    //Déclaration de l'identifiant de l'instruction suivante
    IdentifiantInst Identifiant;

    //En fonction du type de boucle
    switch(this->TypeDeBoucle)
    {
    case TANT_QUE:  //Si c'est un TANT QUE
        if(this->EstLePremier())    //S'il s'agit du premier élément (du début de la boucle)
        {
            //On demande le resultat de la condition
            bool Resultat (Inst_Cond::ComparaisonStatique(this->pRoutine->getManagerParent(), this->DescriptionCondition));

            //Si on attend pas de réponse de la par de l'arduino
            if(!Inst_Cond::AttenteReponse)
            {
                //Si le résultat est vrai
                if(Resultat)
                {
                    //On passe à l'instruction suivante dans la boucle
                    Identifiant.nIdRoutine = this->pRoutine->getId() ;
                    Identifiant.nIdInstruction = this->nIdSuivant[0] ;
                }
                else
                {
                    //S'il est faux, on saute la boucle et on passe à la suite
                    Identifiant.nIdRoutine = this->pRoutine->getId() ;
                    Identifiant.nIdInstruction = this->pInstPaire->getIdSuivantParSortie().at(0) ;
                }
            }
            else
            {
                //Si par contre on attend un réponse, on remet cet élément en tête de pile pour repasser dessus en attendant la réponse
                Identifiant.nIdRoutine = this->pRoutine->getId() ;
                Identifiant.nIdInstruction = this->nId;
            }

        }
        else
        {
            //Si ce n'est pas le premier, qu'on est donc en fin de boucle, on remonte jusqu'au début de boucle
            Identifiant.nIdRoutine = this->pRoutine->getId() ;
            Identifiant.nIdInstruction = this->getIdPaire() ;
        }
        break;

    case JUSQU_A:   //Si c'est un JUSQU'A

        //Si c'est le début de la boucle, on passe à l'instruction suivante dans la boucle sans se poser de question
        if(this->EstLePremier())
        {
            Identifiant.nIdRoutine = this->pRoutine->getId() ;
            Identifiant.nIdInstruction = this->nIdSuivant[0] ;
        }
        else
        {
            //Sinon, on demande le resultat de la condition
            bool Resultat (Inst_Cond::ComparaisonStatique(this->pRoutine->getManagerParent(), this->DescriptionCondition));

            //Si on est pas en attente de réponse
            if(!Inst_Cond::AttenteReponse)
            {
                //Si le resultat est vrai
                if(Resultat)
                {
                    //On sort de la boucle
                    Identifiant.nIdRoutine = this->pRoutine->getId() ;
                    Identifiant.nIdInstruction = this->nIdSuivant[0] ;
                }
                else
                {
                    //S'il est faux, on reboucle
                    Identifiant.nIdRoutine = this->pRoutine->getId() ;
                    Identifiant.nIdInstruction = this->pInstPaire->getIdPaire();
                }
            }
            else
            {
                //Si on est en attente d'une réponse, on met cet élément en tête de pile pour repasser dessus
                Identifiant.nIdRoutine = this->pRoutine->getId() ;
                Identifiant.nIdInstruction = this->nId;
            }
        }
        break;
    }

    //On ajoute à la pile l'identifiant d'instruction qui a été déterminé dans les conditions plus haut
    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant) ;
}

/**
 * Retourne une chaine de caractère affichable
 *
 * @brief   Inst_Boucle::toString
 * @return  La chaine affichable
 */
QString Inst_Boucle::toString()
{
    //Déclaration des variable
    QString     Retour;
    QStringList ListeChainePossible;

    //En fonction du type de boucle, on rempli la liste des chaines possibles
    switch(this->TypeDeBoucle)
    {
    case TANT_QUE:
        ListeChainePossible.append("Tant que\n" + Inst_Cond::toStringStatique(this->DescriptionCondition));
        ListeChainePossible.append("Fin de boucle");
        break;

    case JUSQU_A:
        ListeChainePossible.append("Répéter");
        ListeChainePossible.append("Jusqu'à\n" + Inst_Cond::toStringStatique(this->DescriptionCondition));
        break;
    }

    //En fonction de la position de l'élément (début ou fin de boucle) on retourne l'une ou l'autre des chaines dans la liste
    if(this->EstLePremier())
    {
        Retour = ListeChainePossible[0] ;
    }
    else
    {
        Retour = ListeChainePossible[1] ;
    }

    return Retour ;
}

/**
 * Ouvre une fenetr de modification
 *
 * @brief Inst_Boucle::Modifier
 */
bool Inst_Boucle::Modifier()
{
    //Déclaraion des variables
    bool Retour (true);
    bool Condition(false);

    //En fonction du type de boucle, on va vérifier la position de l'élément
    switch(this->TypeDeBoucle)
    {
    case TANT_QUE:
        Condition = this->EstLePremier() ;
        break;

    case JUSQU_A:
        Condition = !(this->EstLePremier()) ;
        break;
    }

    //Et en fonction de cette position, on va lancer la modification soit sur nous-même, soit sur l'instruction paire (ça dépend de qui contient et exécute la condition...)
    if(Condition)
    {
        Retour = this->LancerModification();
    }
    else
    {
        Retour = this->pInstPaire->Modifier();
    }

    return Retour ;
}

/**
 * Mutateur du pointeur vers l'instruction paire
 *
 * @brief Inst_Boucle::setInstPaire
 * @param pInstPaire    Pointeur vers l'instruction paire
 */
void Inst_Boucle::setInstPaire(Inst_Boucle * pInstPaire)
{
    if(pInstPaire != NULL)
    {
        this->pInstPaire = pInstPaire ;
    }
}

/**
 * Mutateur de la description de la boucle
 *
 * @brief Inst_Boucle::setDesc
 * @param Description   La description de la boucle
 */
void Inst_Boucle::setDesc(DescBoucle Description)
{
    //Affectation à nos propre attributs
    this->DescriptionCondition  = Description.DescriptionCondition ;
    this->TypeDeBoucle          = Description.TypeDeBoucle ;

    //S'il y a une instruction paire et que sa description est différente : on la modifie aussi. elle doivent toujours être identique
    if(this->pInstPaire != 0)
    {
        if(this->pInstPaire->getDesc() != Description)
        {
            this->pInstPaire->setDesc(Description);
        }
    }

}

/**
 * Ascesseur de la desription
 * @brief Inst_Boucle::getDesc
 * @return retourne la description de la boucle
 */
DescBoucle Inst_Boucle::getDesc()
{
    f_ModInst_Boucle f_Desc(this);
    return f_Desc.getDescription();
}

/**
 * Vérifie entre cette instruction et l'instruction paire, laquelle est la premiere dans l'organigramme
 *
 * @brief   Inst_Boucle::EstLePremier
 * @return  Vrai si elle est en premier, faux sinon
 */
bool Inst_Boucle::EstLePremier()
{
    bool Retour = true;

    //S'il y a une instruction paire
    if(this->pInstPaire != 0)
    {
        //On regarde laquelle à l'Id le plus petit. Le plus petit est le premier.
        Retour = (this->pInstPaire->getId() > this->nId);
    }

    return  Retour;
}

/**
 * Méthode servant au parseur XML pour recharger les configuration des éléments
 * @brief Inst_Boucle::setAttr
 * @param NomAttr   Le nom de l'attribut visé
 * @param Valeur    Sa valeur
 */
void Inst_Boucle::setAttr(QString NomAttr, QStringRef Valeur)
{
    //On affecte des valeurs au attributs en fonction du Nom et de la valeur donnée

    //Modifie le type de boucle
    if(NomAttr == "TypeDeBoucle")
    {
        int         nValeur (Valeur.toInt()) ;
        TypeBoucle    tValeur (static_cast<TypeBoucle>(nValeur));

        this->TypeDeBoucle = tValeur ;
    }

    //Déclare la condition avec des valeurs par défaut
    if(NomAttr == "DescriptionCondition")
    {
        DescCondition tValeur;

        tValeur.TCondition          = LOG;
        tValeur.nIndiceVar          = 0;
        tValeur.TypeComp            = EGUAL;
        tValeur.nValeur             = 0;
        tValeur.TestMat.sNomTest    = "";
        tValeur.TestMat.sCommande   = "";
        tValeur.TestMat.bActive     = false;

        this->DescriptionCondition = tValeur;
    }

    // === REMPLISSAGE DE LA CONDITION ===

    if(NomAttr == "TCondition")
    {
        int             nValeur     (Valeur.toInt()) ;
        TypeCondition   tValeur     (static_cast<TypeCondition>(nValeur));

        this->DescriptionCondition.TCondition = tValeur;
    }

    if(NomAttr == "nIndiceVar")
    {
        unsigned int nValeur    (Valeur.toInt()) ;

        this->DescriptionCondition.nIndiceVar = nValeur ;
    }

    if(NomAttr == "TypeComp")
    {
        int             nValeur (Valeur.toInt()) ;
        TypeComparaison tValeur (static_cast<TypeComparaison>(nValeur));

        this->DescriptionCondition.TypeComp = tValeur ;
    }

    if(NomAttr == "nValeur")
    {
        unsigned short nValeur  (Valeur.toUShort()) ;

        this->DescriptionCondition.nValeur = nValeur ;
    }

    if(NomAttr == "TestMat")
    {
        TestArduino NouveauTest;

        NouveauTest.sNomTest    = "";
        NouveauTest.sCommande   = "";
        NouveauTest.bActive     = false ;

        this->DescriptionCondition.TestMat = NouveauTest;
    }

    if(NomAttr == "sNomTest")
    {
        QString sValeur (Valeur.toString());

        this->DescriptionCondition.TestMat.sNomTest = sValeur ;
    }

    if(NomAttr == "sCommande")
    {
        QString sValeur (Valeur.toString());

        this->DescriptionCondition.TestMat.sCommande = sValeur ;
    }

    if(NomAttr == "bActive")
    {
        QString sValeur (Valeur.toString());
        bool    bValeur (false);

        if(sValeur == "true")
        {
            bValeur = true;
        }

        this->DescriptionCondition.TestMat.bActive = bValeur ;
    }

    //Si le nom ne correspond pas à ceux présent ici, on exécute la méthode parente
    Instruction::setAttr(NomAttr, Valeur);
}
/**
 * Mutateur de l'activation de la boucle
 * @brief Inst_Boucle::setActivation
 * @param Activ la valeur de l'activation
 */
void Inst_Boucle::setActivation(bool Activ)
{
    this->EstActive = Activ;
}

/**
 * Ascesseur de l'activation de la maquette
 * @brief Inst_Boucle::getEstActive
 * @return la valeur de l'activation
 */
bool Inst_Boucle::getEstActive()
{
    return this->EstActive;
}

/**
 * Enregistre son contenu au format XML grâce au parseur fourni
 * @brief Inst_Boucle::Enregistrer
 * @param Parser Le parseur à utiliser
 */
void Inst_Boucle::Enregistrer(QXmlStreamWriter* Parser)
{
    //Ecriture des balises communes à toutes les instructions
    this->DebutBalise(Parser);

        //Ecriture du type de boucle
        Parser->writeEmptyElement("TypeDeBoucle");
        Parser->writeAttribute("valeur", QVariant((int)this->TypeDeBoucle).toString());

        //Ecriture de la condition
        Parser->writeStartElement("DescriptionCondition");

            Parser->writeEmptyElement("TCondition");
            Parser->writeAttribute("valeur", QVariant((int)this->DescriptionCondition.TCondition).toString());

            Parser->writeEmptyElement("nIndiceVar");
            Parser->writeAttribute("valeur", QVariant(this->DescriptionCondition.nIndiceVar).toString());

            Parser->writeEmptyElement("nValeur");
            Parser->writeAttribute("valeur", QVariant(this->DescriptionCondition.nValeur).toString());

            Parser->writeStartElement("TestMat");

                Parser->writeEmptyElement("bActive");

                if(this->DescriptionCondition.TestMat.bActive)
                {
                    Parser->writeAttribute("valeur", "true");
                }
                else
                {
                    Parser->writeAttribute("valeur", "false");
                }


                Parser->writeEmptyElement("sCommande");
                Parser->writeAttribute("valeur", this->DescriptionCondition.TestMat.sCommande);

                Parser->writeEmptyElement("sNomTest");
                Parser->writeAttribute("valeur", this->DescriptionCondition.TestMat.sNomTest);

            Parser->writeEndElement();

        Parser->writeEndElement();

        //Ecriture de la fin de balise commune
    this->FinBalise(Parser);
}

/**
 * Ouvre une fenetre de modification
 *
 * @brief Inst_Boucle::LancerModification
 */
bool Inst_Boucle::LancerModification()
{
    //Déclaration des variables
    bool                Retour      (true);
    f_ModInst_Boucle    FenetreModif(this) ;
    int                 reponse     (0) ;

    //EExécution de la fenetre de modification
    reponse = FenetreModif.exec();

    //Si l'utilisateur a cliqué sur Ok
    if(reponse == QDialog::Accepted)
    {
        //On récupère la nouvelle description
        DescBoucle Description  (FenetreModif.getDescription()) ;

        //On la met en place dans l'élément
        this->setDesc(Description) ;
        //this->pInstPaire->setDesc(Description);
    }
    else
    {
        //Sinon on retourne faux
        Retour = false;
    }

    return Retour;
}
