//-------------------------------------------------------------------------------
/**
 * @file        Inst_ES.cpp
 * @brief       Instruction d'entrée sortie sur le matériel
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Représente une action d'entrée/sortie sur le matériel : envois d'énergie dans une sortie, lecture d'une valeur dans une entrée...
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
#include "Inst_ES.h"
#include "../Compil_Inst.h"

/**
 * Constructeur
 *
 * @brief Inst_ES::Inst_ES
 * @param nId       Identifiant de l'instruction
 * @param pRoutine  Routine parente
 */
Inst_ES::Inst_ES(unsigned int nId, Routine *pRoutine):
    Instruction         (nId, pRoutine),
    bAttenteReponse     (false)
{

}

Compil_Inst *Inst_ES::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    QString Commandes (this->CommandeDemandee.sCommande);
    QStringList Commande(Commandes.split(","));

    for(register int i = 0; i < Commande.length(); i++)
    {
        char        Broche  (0);
        char        Var     (0);
        char        Val     (0);
        char        Col     (0);
        char        Lig     (0);
        char        h1      (0);
        char        h2      (0);
        char        h3      (0);

        QByteArray  Chaine;

        QByteArray  Frq;
        int         nFrq    (0);
        QByteArray  Tmp;
        int         nTmp    (0);

        switch(Commande[i].at(0).toLatin1())
        {
        case 'A':
            Retour->AjouterUnOpCode('\x4');

            Broche  = Commande[i].mid(1, 2).toInt();
            Retour->AjouterUnOpCode(Broche);

            Var = Commande[i].right(1)[0].toLatin1() - 64;
            Retour->AjouterUnOpCode(Var);
            break;

        case 'M':
            if(Commande[i].right(2)[0] == '#')
            {
                Retour->AjouterUnOpCode('\x5');

                Broche = Commande[i].mid(1, 2).toInt();
                Retour->AjouterUnOpCode(Broche);

                Var = Commande[i].right(1)[0].toLatin1() - 64;
                Retour->AjouterUnOpCode(Var);
            }
            else
            {
                Retour->AjouterUnOpCode('\x6');

                Broche = Commande[i].mid(1, 2).toInt();
                Retour->AjouterUnOpCode(Broche);

                Val = Commande[i].right(3).toInt();
                Retour->AjouterUnOpCode(Val);
            }
            break;

        case 'W':
            Retour->AjouterUnOpCode('\x7');

            Broche = Commande[i].mid(1, 2).toInt();
            Retour->AjouterUnOpCode(Broche);

            Val = Commande[i].right(1).toInt();
            Retour->AjouterUnOpCode(Val);

            break;

        case 'S':
            if(Commande[i].right(2)[0] == '#')
            {
                Retour->AjouterUnOpCode('\x8');

                Broche = Commande[i].mid(1, 2).toInt();
                Retour->AjouterUnOpCode(Broche);

                Var = Commande[i].right(1)[0].toLatin1() - 64;
                Retour->AjouterUnOpCode(Var);
            }
            else
            {
                Retour->AjouterUnOpCode('\x9');

                Broche = Commande[i].mid(1, 2).toInt();
                Retour->AjouterUnOpCode(Broche);

                Val = Commande[i].right(3).toInt();
                Retour->AjouterUnOpCode(Val);
            }
            break;

        case 'J':
            Retour->AjouterUnOpCode('\x14');
            break;

        case 'L':
            switch(Commande[i].at(1).toLatin1())
            {
            case 'C':
                Retour->AjouterUnOpCode('\x15');
                break;

            case 'G':
                Retour->AjouterUnOpCode('\x16');

                Col = Commande[i].mid(2, 2).toInt();
                Retour->AjouterUnOpCode(Col);

                Lig = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Lig);
                break;

            case 'W':
                Retour->AjouterUnOpCode('\x17');

                Chaine += Commande[i].mid(2);
                Retour->AjouterUnOpCode(Chaine);
                break;

            case 'V':
                Retour->AjouterUnOpCode('\x19');

                Val = Commande[i].mid(2, 3).toInt();
                Retour->AjouterUnOpCode(Val);

                Col = Commande[i].mid(5, 2).toInt();
                Retour->AjouterUnOpCode(Col);

                Lig = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Lig);
                break;

            case 'D':
                Retour->AjouterUnOpCode('\x1A');

                Var = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Var);
                break;

            case 'H':
                Retour->AjouterUnOpCode('\x1B');

                Var = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Var);
                break;

            case 'T':
                Retour->AjouterUnOpCode('\x20');

                Val = Commande[i].mid(2, 3).toInt();
                Retour->AjouterUnOpCode(Val);

                Col = Commande[i].mid(5, 2).toInt();
                Retour->AjouterUnOpCode(Col);

                Lig = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Lig);
                break;

            case 'L':
                Retour->AjouterUnOpCode('\x22');

                Val = Commande[i].mid(2, 3).toInt();
                Retour->AjouterUnOpCode(Val);

                Col = Commande[i].mid(5, 2).toInt();
                Retour->AjouterUnOpCode(Col);

                Lig = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Lig);
                break;
            }

            break;

        case 'Y':
            switch(Commande[i].at(1).toLatin1())
            {
            case 'R':
                Retour->AjouterUnOpCode('\x1C');

                Var = Commande[i].right(1).toInt();
                Retour->AjouterUnOpCode(Var);
                break;

            case 'V':
                Retour->AjouterUnOpCode('\x1D');

                Val = Commande[i].at(2).toLatin1();
                Retour->AjouterUnOpCode(Val);

                Retour->AjouterUnOpCode(Commande[i].at(3).toLatin1());

                Var = Commande[i].right(1)[0].toLatin1() - 64;
                Retour->AjouterUnOpCode(Var);
                break;

            case 'C':
                Retour->AjouterUnOpCode('\x1E');

                h1 = Commande[i].at(2).toLatin1();
                Retour->AjouterUnOpCode(h1);

                h2 = Commande[i].at(3).toLatin1();
                Retour->AjouterUnOpCode(h2);

                h3 = Commande[i].at(4).toLatin1();
                Retour->AjouterUnOpCode(h3);
                break;
            }

            break;

        case 'O':
            Retour->AjouterUnOpCode('\x23');

            Broche = Commande[i].mid(1, 2).toInt();
            Retour->AjouterUnOpCode(Broche);

            nFrq = Commande[i].mid(3, 4).toInt();

            Frq.append(nFrq >> 8);
            Frq.append(nFrq & 0x00FF);

            Retour->AjouterUnOpCode(Frq);

            nTmp = Commande[i].right(4).toInt();

            Tmp.append(nTmp >> 8);
            Tmp.append(nTmp & 0x00FF);

            Retour->AjouterUnOpCode(Tmp);
            break;

        case 'Q':
            Retour->AjouterUnOpCode('\x24');

            Broche = Commande[i].mid(1, 2).toInt();
            Retour->AjouterUnOpCode(Broche);

            Broche = Commande[i].mid(3, 2).toInt();
            Retour->AjouterUnOpCode(Broche);

            Var = Commande[i].right(1)[0].toLatin1() - 64;
            Retour->AjouterUnOpCode(Var);
            break;
        }
    }

    return Retour;
}

/**
 * Execute l'instruction
 *
 * @brief   Inst_ES::Executer
 * @return  L'identifiant suivant
 */
void Inst_ES::Executer()
{
    //On récupère la ou les commandes séparées par des virgules
    QString Commandes (this->CommandeDemandee.sCommande);

    //Si on est pas en attente de réponse de la maquette
    if(!this->bAttenteReponse)
    {
        //On split pour récuperer chaque commande indépendamment
        QStringList Commande(Commandes.split(","));

        //Pour chaque commande
        for(register int i = 0; i < Commande.length(); i++)
        {
            //Si l'avant dernier caractère est un #
            if(Commande[i].right(2).at(0) == '#')
            {
                //On remplace le #* par la variable correspondante si c'est une commande M ou S
                int nIndiceVariable (Commande[i][Commande[i].length() - 1].toLatin1() - 65);
                Commande[i] = Commande[i].left(Commande[i].length() - 2);

                if(this->CommandeDemandee.sCommande[0] == 'M' || this->CommandeDemandee.sCommande[0] == 'S')
                {
                    QVariant    VariantVar(this->pRoutine->getManagerParent()->getValVar(nIndiceVariable));
                    QString     ValeurVar(VariantVar.toString().rightJustified(3, '0', true));
                    Commande[i] += ValeurVar;
                }
                else //Si ce nest pas une de ces deux commandes, c'est que le # signifie une récupération de valeur donc on se met en attente
                {
                    this->bAttenteReponse = true;
                }
            }


            this->pRoutine->getManagerParent()->EnvoyerCommande(Commande[i]);
        }

    }
    else //Si on est en attente
    {
        //On split les commandes
        QStringList Commande(Commandes.split(","));

        //Pour chaque commande on assigne la valeur dans la variable correspondante
        for(register int i = 0; i < Commande.length(); i++)
        {
            QByteArray  Reponse         (this->pRoutine->getManagerParent()->getDerniereReponse());
            int         nIndiceVariable (Commande[i][Commande[i].length() -1].toLatin1() - 65);

            QVariant    ValeurRetour;

            switch(Commande[i][0].toLatin1())
            {
            case 'a':
            case 'A':
            case 'Q':
                ValeurRetour = Reponse.mid(6);
                this->pRoutine->getManagerParent()->setValVar(nIndiceVariable, ValeurRetour.toString().toUShort());
                break;
            }

            this->bAttenteReponse = false;
        }

    }

    //Définition de l'instruction à exécuter ensuite
    IdentifiantInst Identifiant;

    //Si on attend un réponse, on se remet dans la pile pour la récupérée
    if(this->bAttenteReponse)
    {
        Identifiant.nIdRoutine = this->pRoutine->getId() ;
        Identifiant.nIdInstruction = this->nId ;
    }
    else //Sinon on passe à notre suivante
    {
        Identifiant.nIdRoutine = this->pRoutine->getId() ;
        Identifiant.nIdInstruction = this->nIdSuivant[0] ;
    }

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Renvois une chaine affichable
 *
 * @brief   Inst_ES::toString
 * @return  La chaine affichable
 */
QString Inst_ES::toString()
{
    return this->CommandeDemandee.sNomAction ;
}


/**
 * Ouvre une fenetre de modification de cette instruction
 *
 * @brief Inst_ES::Modifier
 */
bool Inst_ES::Modifier()
{
    bool            Retour      (true);
    f_ModInst_ES    FenetreModif(this) ;
    int             Reponse     (0) ;

    //Exécution de la fenetre
    Reponse = FenetreModif.exec() ;

    //Si l'utilisateur à cliqué sur Ok
    if(Reponse == QDialog::Accepted)
    {
        this->setDesc(FenetreModif.getDescription()) ;
    }
    else
    {
        Retour = false;
    }

    return Retour ;
}

/**
 * Mutateur général pour ouverture de fichier
 * @brief Inst_ES::setAttr
 * @param NomAttr   Nom de l'attribut
 * @param Valeur    Valeur de l'attribut
 */
void Inst_ES::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "CommandeDemandee")
    {
        ActionArduino tValeur;

        tValeur.sNomAction  = "";
        tValeur.sCommande   = "";
        tValeur.bActive     = false;
    }

    if(NomAttr == "sNomAction")
    {
        QString sValeur (Valeur.toString());

        this->CommandeDemandee.sNomAction = sValeur ;
    }

    if(NomAttr == "sCommande")
    {
        QString sValeur (Valeur.toString());

        this->CommandeDemandee.sCommande = sValeur ;
    }

    if(NomAttr == "bActive")
    {
        QString sValeur (Valeur.toString());
        bool    bValeur (false);

        if(sValeur == "true")
        {
            bValeur = true;
        }

        this->CommandeDemandee.bActive = bValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur);
}

/**
 * Mutatuer de description
 * @brief Inst_ES::setDesc
 * @param Desc  Nouvelle description à mettre en place
 */
void Inst_ES::setDesc(DescES Desc)
{
    this->CommandeDemandee = Desc.CommandeDemandee ;
}

/**
 * Ascesseur de description
 * @brief Inst_ES::getDesc
 * @return  La description courante
 */
DescES Inst_ES::getDesc()
{
    f_ModInst_ES f_Desc(this);
    return f_Desc.getDescription();
}

/**
 * Enregistre au format XML
 * @brief Inst_ES::Enregistrer
 * @param Parser    Le parseur à utiliser
 */
void Inst_ES::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeStartElement("CommandeDemandee");

        Parser->writeEmptyElement("bActive");

        if(this->CommandeDemandee.bActive)
        {
            Parser->writeAttribute("valeur", "true");
        }
        else
        {
            Parser->writeAttribute("valeur", "false");
        }

        Parser->writeEmptyElement("sCommande");
        Parser->writeAttribute("valeur", this->CommandeDemandee.sCommande);

        Parser->writeEmptyElement("sNomAction");
        Parser->writeAttribute("valeur", this->CommandeDemandee.sNomAction);

    Parser->writeEndElement();

    this->FinBalise(Parser);
}
