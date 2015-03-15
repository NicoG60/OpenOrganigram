/**
 * @file        Instruction.cpp
 * @brief       Représente une instruction d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Représente une instruction de l'organigramme, elle peut s'exécuter et renvois l'id de sa suivante.
 * Une instruction peut avoir plusieurs "sortie". Par exemple une condition à une sortie VRAI et une sortie
 * FAUX. Les identifiants suivant sont donc classé dans une liste ou chaque entrée représente une sortie
 * potentielle. C'est l'exécution de l'instruction qui déterminera la sortie à empreinter.
 * L'attribut parent est là pour savoir si une instruction est enfant d'une condition ou d'une boucle.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tout est à faire, la classe est codée au minimum syndicale pour tester d'autres fonctionnalités
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====

//=====   Headers Peros   =====
#include "Instruction.h"
#include "Inst/Inst_DebFin.h"
#include "Inst/Inst_Ope.h"
#include "Inst/Inst_Cond.h"
#include "Inst/Inst_ES.h"
#include "Inst/Inst_Rout.h"
#include "Inst/Inst_Tempo.h"
#include "Inst/Inst_Com.h"
#include "Inst/Inst_Boucle.h"

/**
 * Constructeur
 *
 * @brief   Instruction::Instruction(int nId, int nParent = 0, int nNiveau = 0)
 * @param   nId     Identifiant de l'instruction
 * @param   nParent Parent de l'instruction, 0 si elle n'est pas contenue dans une boucle ou dans un condition
 */
Instruction::Instruction(unsigned int nId, Routine *pRoutine) :
    QObject (0),
    nIdPrec (0),
    nId     (nId),
    pRoutine(pRoutine)
{
    this->nIdSuivant.append(0);
}

/**
 * Destructeur virtuel, rien à faire ici, à réimplémenter dans les classe fille au besoin...
 *
 * @brief   Instruction::~Instruction()
 */
Instruction::~Instruction()
{
}

/**
 * CRée un nouvelle instruction en fonction des parametre
 * @brief Instruction::NouvelleInst
 * @param Type      Type d'instruction à créer
 * @param nId       Identifiant qu'elle aura
 * @param pRoutine  La routine à laquelle l'affectée
 * @return  Un pointeur vers cette instruction
 */
Instruction * Instruction::NouvelleInst(TypeCommande Type, unsigned int nId, Routine * pRoutine)
{
    Instruction *  pNouvelleInst    (0) ;
    switch(Type)
    {
    case OPE :
        pNouvelleInst = new Inst_Ope(nId, pRoutine);
        break ;

    case COND :
        pNouvelleInst = new Inst_Cond(nId, pRoutine);
        break ;

    case ES :
        pNouvelleInst = new Inst_ES(nId, pRoutine);
        break ;

    case ROUT :
        pNouvelleInst = new Inst_Rout(nId, pRoutine);
        break ;

    case TEMPO :
        pNouvelleInst = new Inst_Tempo(nId, pRoutine);
        break ;

    case COM :
        pNouvelleInst = new Inst_Com(nId, pRoutine);
        break ;

    case DEBFIN :
        pNouvelleInst = new Inst_DebFin(nId, pRoutine);
        break ;

    case BOUCLE:
        pNouvelleInst = new Inst_Boucle(nId, pRoutine);
        break ;
    }

    return pNouvelleInst ;
}

/**
  Converti une description de commentaire en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescCommentaire  Desc)
{
    QString Retour;

    Retour += Desc.sCommentaire;

    return Retour;
}

/**
  Converti une description de condition en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescCondition    Desc)
{
    QString Retour;

    Retour += QVariant(static_cast<int>(Desc.TCondition)).toString();
    Retour += "`";
    Retour += QVariant(Desc.nIndiceVar).toString();
    Retour += "`";
    Retour += QVariant(static_cast<int>(Desc.TypeComp)).toString();
    Retour += "`";
    Retour += QVariant(Desc.nValeur).toString();
    Retour += "`";
    Retour += Desc.TestMat.sNomTest;
    Retour += "`";
    Retour += Desc.TestMat.sCommande;
    Retour += "`";
    Retour += QVariant(Desc.TestMat.bActive).toString();

    return Retour;
}

/**
  Converti une description d'entrée sortie en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescES           Desc)
{
    QString Retour;

    Retour += Desc.CommandeDemandee.sNomAction;
    Retour += "`";
    Retour += Desc.CommandeDemandee.sCommande;
    Retour += "`";
    Retour += QVariant(Desc.CommandeDemandee.bActive).toString();

    return Retour;
}

/**
  Converti une description de boucle en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescBoucle         Desc)
{
    QString Retour;

    Retour += QVariant(static_cast<int>(Desc.TypeDeBoucle)).toString();
    Retour += "`";
    Retour += QVariant(static_cast<int>(Desc.DescriptionCondition.TCondition)).toString();
    Retour += "`";
    Retour += QVariant(Desc.DescriptionCondition.nIndiceVar).toString();
    Retour += "`";
    Retour += QVariant(static_cast<int>(Desc.DescriptionCondition.TypeComp)).toString();
    Retour += "`";
    Retour += QVariant(Desc.DescriptionCondition.nValeur).toString();
    Retour += "`";
    Retour += Desc.DescriptionCondition.TestMat.sNomTest;
    Retour += "`";
    Retour += Desc.DescriptionCondition.TestMat.sCommande;
    Retour += "`";
    Retour += QVariant(Desc.DescriptionCondition.TestMat.bActive).toString();

    return Retour;
}

/**
  Converti une description d'opération en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescOperation    Desc)
{
    QString Retour;

    Retour += QVariant(Desc.nIndiceVariable).toString();
    Retour += "`";
    Retour += QVariant(static_cast<int>(Desc.Operation)).toString();
    Retour += "`";
    Retour += QVariant(Desc.nValeur).toString();

    return Retour;
}

/**
  Converti une description de routine en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescRoutine      Desc)
{
    QString Retour;

    Retour += QVariant(Desc.nIdRoutine).toString();
    Retour += "`";
    Retour += Desc.sNom;

    return Retour;
}

/**
  Converti une description de tempo en chaine de caractère
 * @brief Instruction::ConvertirVersChaine
 * @param Desc  La description
 * @return La chaine de caractère
 */
QString Instruction::ConvertirVersChaine(DescTempo        Desc)
{
    QString Retour;

    Retour += QVariant(Desc.nMilliseconde).toString();

    return Retour;
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescCommentaire*  Desc)
{
    if(Desc != 0)
    {
        Desc->sCommentaire = Chaine;
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescCondition*    Desc)
{
    if(Desc != 0)
    {
        QStringList Liste   (Chaine.split("`"));

        if(Liste.length() == 7)
        {
            TypeCondition   TCondition      (static_cast<TypeCondition>(Liste[0].toInt()));
            unsigned int    nIndiceVar      (Liste[1].toUInt());
            TypeComparaison TypeComp        (static_cast<TypeComparaison>(Liste[2].toInt()));
            unsigned short  nValeur         (Liste[3].toUShort());
            QString         sNomTest        (Liste[4]);
            QString         sCommande       (Liste[5]);
            bool            bActive         (QVariant(Liste[6]).toBool());

            Desc->TCondition        = TCondition;
            Desc->nIndiceVar        = nIndiceVar;
            Desc->TypeComp          = TypeComp;
            Desc->nValeur           = nValeur;
            Desc->TestMat.sNomTest  = sNomTest;
            Desc->TestMat.sCommande = sCommande;
            Desc->TestMat.bActive   = bActive;
        }
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescES*           Desc)
{
    if(Desc != 0)
    {
        QStringList Liste   (Chaine.split("`"));

        if(Liste.length() == 3)
        {
            QString         sNomAction        (Liste[0]);
            QString         sCommande       (Liste[1]);
            bool            bActive         (QVariant(Liste[2]).toBool());

            Desc->CommandeDemandee.sNomAction   = sNomAction;
            Desc->CommandeDemandee.sCommande    = sCommande;
            Desc->CommandeDemandee.bActive      = bActive;
        }
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescBoucle*         Desc)
{
    if(Desc != 0)
    {
        QStringList Liste   (Chaine.split("`"));

        if(Liste.length() == 8)
        {
            TypeBoucle        TypeDeBoucle    (static_cast<TypeBoucle>(Liste[0].toInt()));
            TypeCondition   TCondition      (static_cast<TypeCondition>(Liste[1].toInt()));
            unsigned int    nIndiceVar      (Liste[2].toUInt());
            TypeComparaison TypeComp        (static_cast<TypeComparaison>(Liste[3].toInt()));
            unsigned short  nValeur         (Liste[4].toUShort());
            QString         sNomTest        (Liste[5]);
            QString         sCommande       (Liste[6]);
            bool            bActive         (QVariant(Liste[7]).toBool());

            Desc->TypeDeBoucle                              = TypeDeBoucle;
            Desc->DescriptionCondition.TCondition           = TCondition;
            Desc->DescriptionCondition.nIndiceVar           = nIndiceVar;
            Desc->DescriptionCondition.TypeComp             = TypeComp;
            Desc->DescriptionCondition.nValeur              = nValeur;
            Desc->DescriptionCondition.TestMat.sNomTest     = sNomTest;
            Desc->DescriptionCondition.TestMat.sCommande    = sCommande;
            Desc->DescriptionCondition.TestMat.bActive      = bActive;
        }
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescOperation*    Desc)
{
    if(Desc != 0)
    {
        QStringList Liste   (Chaine.split("`"));

        if(Liste.length() == 3)
        {
            unsigned int    nIndiceVariable (Liste[0].toUInt());
            TypeOperation   Operation       (static_cast<TypeOperation>(Liste[1].toInt()));
            unsigned short  nValeur         (Liste[2].toUShort());

            Desc->nIndiceVariable   = nIndiceVariable;
            Desc->Operation         = Operation;
            Desc->nValeur           = nValeur;
        }
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescRoutine*      Desc)
{
    if(Desc != 0)
    {
        QStringList Liste   (Chaine.split("`"));

        if(Liste.length() == 2)
        {
            int     nIdRoutine  (Liste[0].toInt());
            QString sNom        (Liste[1]);

            Desc->nIdRoutine    = nIdRoutine;
            Desc->sNom          = sNom;
        }
    }
}

/**
 * Converti une chaine de caractère formatée vers une description
 * @brief Instruction::ConvertirVersDesc
 * @param Chaine    La chaine à traiter
 * @param Desc      La description à fournir
 */
void Instruction::ConvertirVersDesc(QString Chaine, DescTempo*        Desc)
{
    if(Desc != 0)
    {
        unsigned int    nMillisecondes  (Chaine.toUInt());

        Desc->nMilliseconde = nMillisecondes;
    }
}

/**
 * Mutateur de l'identifiant de suivant
 *
 * @brief   Instruction::setSuivant(int nIdSuivant, int nIdSortie = 0)
 * @param   nIdSuivant  Identifiant de l'instruction suivante
 * @param   nIdSortie   Identifiant de la sortie
 */
void Instruction::setSuivant(unsigned int nIdSuivant, unsigned int nIdSortie)
{
    unsigned int nNbreSortie (this->nIdSuivant.length()) ;

    if(nIdSortie < nNbreSortie)
    {
        this->nIdSuivant[nIdSortie] = nIdSuivant ;
    }
}

/**
 * Remplace l'id suivant Ancien par l'id suivant Nouveau
 * @brief Instruction::RemplacerIdSuivant
 * @param Ancien    Ancien Id
 * @param Nouveau   Nouvel Id
 */
void Instruction::RemplacerIdSuivant(unsigned int Ancien, unsigned int Nouveau)
{
    for(register int i = 0; i < this->nIdSuivant.length(); i++)
    {
        if(this->nIdSuivant[i] == Ancien)
        {
            this->nIdSuivant[i] = Nouveau;
        }
    }
}

/**
 * Mutateur de l'identifiant précédent
 *
 * @brief   Instruction::setPrec(int nIdPred)
 * @param   nIdPrec  Identifiant de l'instruction précédente
 */
void Instruction::setPrec(int nIdPred)
{
    this->nIdPrec = nIdPred ;
}

/**
 * Mutateur général pour ouverture de fichier
 * @brief Instruction::setAttr
 * @param NomAttr
 * @param Valeur
 */
void Instruction::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "nIdSuivant")
    {
        unsigned int    nValeur     (Valeur.toUInt()) ;
        int             nIndice     (this->nIdSuivant.length() - 1) ;

        this->nIdSuivant[nIndice] = nValeur ;
    }

    if(NomAttr == "nIdPrec")
    {
        unsigned int    nValeur     (Valeur.toUInt()) ;

        this->nIdPrec = nValeur ;
    }
}

/**
 * Fonction d'exécution primaire, elle est surchargée dans les classe fille de celle ci.
 *
 * @brief   Instruction::Executer(ModuleManager *  Manager)
 * @return  Identifiant de l'instrufction suivante à exécuter.
 */
void Instruction::Executer()
{
    IdentifiantInst Identifiant;

    Identifiant.nIdRoutine      = this->pRoutine->getId() ;
    Identifiant.nIdInstruction  = this->nIdSuivant[0] ;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Se supprime de la routine auquel elle est affectée
 * @brief Instruction::Supprimer
 */
void Instruction::Supprimer()
{
    this->pRoutine->Supprimer(this);
    this->pRoutine->getManagerParent()->RafraichirVues();
}

/**
 * Recalcule les identifiant de l'instruction en fonction du seuil (si au dessus, on le décrémente)
 * @brief Instruction::RecalculerId
 * @param Seuil
 */
void Instruction::RecalculerId(unsigned int Seuil)
{
    if(this->nId > Seuil)
    {
        this->nId--;
    }

    for(register int i = 0; i < nIdSuivant.length(); i++)
    {
        if(nIdSuivant[i] > Seuil)
        {
            nIdSuivant[i]--;
        }
    }

    if(this->nIdPrec > Seuil)
    {
        this->nIdPrec--;
    }
}

/**
 * Ecrit le début de la balise d'enregistrement XML avec quelques propriétés communes
 * @brief Instruction::DebutBalise
 * @param Parser    Le parseur à utiliser
 */
void Instruction::DebutBalise(QXmlStreamWriter* Parser)
{
    Parser->writeStartElement("Instruction");
    Parser->writeAttribute("Type", QVariant((int)this->getType()).toString());
    Parser->writeAttribute("nId", QVariant(this->nId).toString());

    for(register int i = 0; i < this->nIdSuivant.length(); i++)
    {
        Parser->writeEmptyElement("nIdSuivant");
        Parser->writeAttribute("valeur", QVariant(this->nIdSuivant[i]).toString());
    }

    Parser->writeEmptyElement("nIdPrec");
    Parser->writeAttribute("valeur", QVariant(this->nIdPrec).toString());
}

/**
 * Ferme la balise de sauvegarde XML
 * @brief Instruction::FinBalise
 * @param Parser
 */
void Instruction::FinBalise(QXmlStreamWriter* Parser)
{
    Parser->writeEndElement();
}

IdentifiantInst Instruction::getIdentifiantInst()
{
    IdentifiantInst Id;
    Id.nIdInstruction   = this->nId;
    Id.nIdRoutine       = this->pRoutine->getId();

    return Id;
}
