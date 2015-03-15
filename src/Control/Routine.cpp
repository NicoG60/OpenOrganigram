//-------------------------------------------------------------------------------
/**
 * @file        Routine.cpp
 * @brief       Représente une routine
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        21/02/14
 *
 * Représentation d'une routine en mémoire, contient la liste des instructions à exécuter.
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
#include "Routine.h"
#include "Inst/Inst_DebFin.h"
#include "Inst/Inst_Ope.h"
#include "Inst/Inst_Cond.h"
#include "Inst/Inst_ES.h"
#include "Inst/Inst_Rout.h"
#include "Inst/Inst_Tempo.h"
#include "Inst/Inst_Com.h"
#include "Inst/Inst_Boucle.h"

/**
 * Constructeur par défaut d'une routine
 *
 * @brief   Routine::Routine(QObject *  pParent = 0)
 * @param   pParent     Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     Routine::Constructeur()
 */
Routine::Routine(unsigned int nId, QObject *  pParent) :
    QObject     (pParent),
    nId         (nId),
    sNom        ("Routine sans nom")
{
    this->Constructeur() ;
}

/**
 * Constructeur avec nom
 *
 * @brief   Routine::Routine(QString Nom, QObject *  pParent = 0)
 * @param   nId             Identifiant de la routine
 * @param   sNom            Nom de la routine
 * @param   pManagerParent  Pointeur vers le manager contrenant cette routine
 * @param   bSansDepart     Option pour ne pas créer les 2 éléments de départ de base d'un organigramme (utilisé pour le rappel à partir d'un fichier)
 * @param   pParent         Pointeur vers l'objet QObject (ou dérivé) parent.
 * @see     Routine::Constructeur()
 */
Routine::Routine(unsigned int nId, QString sNom, ModuleManager * pManagerParent, bool bSansDepart, QObject *pParent) :
    QObject         (pParent),
    nId             (nId),
    sNom            (sNom),
    pManagerParent  (pManagerParent)
{
    this->Constructeur(bSansDepart) ;
}

/**
 * Destructeur, détruit au passage toutes les instructions
 *
 * @brief   Routine::~Routine()
 */
Routine::~Routine()
{
    for(register int i = 0; i < this->ListeInst.length(); i++)
    {
        delete this->ListeInst[i] ;
    }
}

/**
 * Ajoute une instruction en fonction des parametre donnés
 * @brief Routine::AjouterInstruction
 * @param Type              Type d'instruction à ajouter
 * @param nIdPrecedent      Identifiant de sa précédente
 * @param Ecartement        S'il s'agit d'une sortie écartée d'une condition
 * @param RetourCondition   S'il s'agit d'une instruction juste après une condition
 * @param Deplacement       S'il s'agit d'un déplacement d'instruction
 */
void Routine::AjouterInstruction(TypeCommande Type, unsigned int nIdPrecedent, bool Ecartement, bool RetourCondition, bool Deplacement)
{
    //Déclaration des variables
    Instruction *       pNouvelleInst   (0) ;
    Instruction *       pPrecedent      (this->ListeInst[nIdPrecedent]) ;
    Instruction *       pSuivant        (0) ;
    QList<unsigned int> nListeIdSuivants(pPrecedent->getIdSuivantParSortie()) ;
    unsigned int        nIdInst         (this->ListeInst.length());
    unsigned int        nSortie         (0);

    //si la liste des Id suivant de l'instruction précédente est supérieure à 1 et qu'un écartement est précisé, alors c'est qu'il est affecté à la sortie écartée
    if(nListeIdSuivants.length() > 1 && Ecartement)
    {
        nSortie = 1 ;
    }

    //Récupération de l'instruction suivantes
    pSuivant = this->ListeInst[nListeIdSuivants[nSortie]] ;

    //Récupération de la liste des instructions précédant de la suivante
    QList<Instruction*> pListePrec      (this->getInstructionPrecedentDe(pSuivant->getId()));

    //En fonction du type, on créer l'instruction
    switch(Type)
    {
    case OPE :
        pNouvelleInst = new Inst_Ope(nIdInst, this);
        break ;

    case COND :
        pNouvelleInst = new Inst_Cond(nIdInst, this);
        break ;

    case ES :
        pNouvelleInst = new Inst_ES(nIdInst, this);
        break ;

    case ROUT :
        pNouvelleInst = new Inst_Rout(nIdInst, this);
        break ;

    case TEMPO :
        pNouvelleInst = new Inst_Tempo(nIdInst, this);
        break ;

    case COM :
        pNouvelleInst = new Inst_Com(nIdInst, this);
        break ;

    case BOUCLE:
        pNouvelleInst = new Inst_Boucle(nIdInst, this);
        break ;
    default:
        break;
    }

    bool Continuer  (false);

    //S'il s'agit d'un déplacement, on continue
    if(Deplacement)
    {
        Continuer = true;
    }
    else //Sinon c'est que c'est une nouvelle instruction, on lance l'interface de modification, si elle est acceptée, on continue
    {
        if(pNouvelleInst->Modifier())
        {
            Continuer = true;
        }
    }

    //Si on peut continuer
    if(Continuer)
    {
        //On ajoute notre instruction à la liste de la routine
        this->ListeInst.append(pNouvelleInst);

        //S'il s'agit d'une boucle, il faut créer sa paire
        if(Type == BOUCLE)
        {
            Inst_Boucle * pNouvelleBoucle   (static_cast<Inst_Boucle *>(pNouvelleInst));
            Inst_Boucle * pNouvellePaire  (new Inst_Boucle(nIdInst + 1, this)) ;

            pNouvellePaire->setDesc(pNouvelleBoucle->getDesc());

            this->ListeInst.append(pNouvellePaire);

            pNouvelleBoucle->setInstPaire(pNouvellePaire);
            pNouvellePaire->setInstPaire(pNouvelleBoucle);

            pNouvelleBoucle->setSuivant(nIdInst + 1);
            pNouvellePaire->setPrec(nIdInst);

            //Si c'est un retour de condition, on modifie convenablement les deux instrction
            if(RetourCondition)
            {
                for(register int i = 0; i < pListePrec.length(); i++)
                {
                    QList<unsigned int> nSuivant    (pListePrec[i]->getIdSuivantParSortie()) ;
                    for(register int i = 0; i < nSuivant.length(); i++)
                    {
                        if(nSuivant[i] == pSuivant->getId())
                        {
                            pListePrec[i]->setSuivant(nIdInst, i) ;
                        }
                    }
                }
            }
            else
            {
                pPrecedent->setSuivant(nIdInst, nSortie);
            }

            //On assigne les Ids suivants et précédents
            pNouvelleBoucle->setPrec(nIdPrecedent);
            pNouvellePaire->setSuivant(pSuivant->getId());

            if(pNouvelleInst->getIdPrec() == pSuivant->getIdPrec())
            {
                pSuivant->setPrec(nIdInst + 1);
            }
        }
        else //Si c'est pas une boucle
        {
            //On récupère la liste des Id suivant de l'instruction précédente
            QList<unsigned int> nSuivantPrec(pPrecedent->getIdSuivantParSortie()) ;

            //Si c'est un retour de condition, on assigne toutes les instruction de fin de condition à pointer sur la nouvelle
            if(RetourCondition)
            {
                for(register int i = 0; i < pListePrec.length(); i++)
                {
                    QList<unsigned int> nSuivant    (pListePrec[i]->getIdSuivantParSortie()) ;
                    for(register int j = 0; j < nSuivant.length(); j++)
                    {
                        if(nSuivant[j] == pSuivant->getId())
                        {
                            pListePrec[i]->setSuivant(nIdInst, j) ;
                        }
                    }
                }
            }
            else //Sinon c'est un assignation bête
            {
                pPrecedent->setSuivant(nIdInst, nSortie);
            }

            //On ajoute l'instruction précédente
            pNouvelleInst->setPrec(nIdPrecedent);

            //On récupere la liste des suivant de notre nouvelle instruction
            QList<unsigned int> nSuivant    (pNouvelleInst->getIdSuivantParSortie()) ;

            //Pour chaque sortie, on l'assigne au suivant
            for(register int i = 0; i < nSuivant.length(); i++)
            {
                pNouvelleInst->setSuivant(pSuivant->getId(), i);
            }

            //Si la nouvelle et la suivante ont la meêm précédente
            if(pNouvelleInst->getIdPrec() == pSuivant->getIdPrec())
            {
                //S'il y a un ecartement
                if(Ecartement)
                {
                    //si le précédent est une conition
                    if(pPrecedent->getType() == COND)
                    {
                        //Si leus deux sortie de la condition pointe sur 2 instructions différentes
                        if(nSuivantPrec[1] == pSuivant->getId() && nSuivantPrec[0] != pSuivant->getId())
                        {
                            //On assigne la précédente de la suivante sur la nouvelle instruction
                            pSuivant->setPrec(nIdInst);
                        }
                    }
                }
                else
                {
                    //sinon On assigne la précédente de la suivante sur la nouvelle instruction
                    pSuivant->setPrec(nIdInst);
                }
            }
        }
    }
    else
    {
        //Si on peut pas continuer, on supprimer la nouvelle isntruction créée
        delete pNouvelleInst;
    }
}

/**
 * Ajoute une nouvelle instruction betement et mechemment
 * @brief Routine::AjouterInstruction
 * @param Inst
 */
void Routine::AjouterInstruction(Instruction * Inst)
{
    //si le pointeur n'est pas nul, on l'ajoute en considérant que ces propriété sont cohérente avec le reste des instructions
    if(Inst != 0)
    {
        this->ListeInst.append(Inst);
    }
}

/**
 * Met un instruction dans la corbeille en faisant tout les échange d'lid necessaire
 * @brief Routine::MettreDansLaCorbeille
 * @param Inst
 */
void Routine::Supprimer(Instruction * Inst)
{
    Instruction*        InstSuivante;
    QList<Instruction*> ListePrecedent  (this->getInstructionPrecedentDe(Inst->getId()));
    QList<unsigned int> ListeSuivante   (Inst->getIdSuivantParSortie());

    for(register int i = 0; i < ListePrecedent.length(); i++)
    {
        ListePrecedent[i]->RemplacerIdSuivant(Inst->getId(), ListeSuivante[0]);
    }

    InstSuivante = this->getInstruction(ListeSuivante[0]);
    if(InstSuivante->getIdPrec() == Inst->getId())
    {
        InstSuivante->setPrec(Inst->getIdPrec());
    }


    if(Inst->getType() == BOUCLE)
    {
        Inst_Boucle * Boucle  (static_cast<Inst_Boucle*>(Inst));
        Boucle->setActivation(false);

        if(Boucle->getInstPaire()->getEstActive())
        {
            this->Supprimer(Boucle->getInstPaire());
        }
    }

    if(Inst->getType() == COND)
    {
        Inst_Cond *     Condition   (static_cast<Inst_Cond*>(Inst));
        Instruction*    Premiere    (this->getInstruction(Condition->getIdSuivantParSortie().at(1)));
        Instruction*    Derniere    (Condition->getDernierInstBrancheEcartee());

        if(Derniere->getId() != 0)
        {
            for(register int i = 0; i < ListePrecedent.length(); i++)
            {
                ListePrecedent[i]->RemplacerIdSuivant(ListeSuivante[0], Premiere->getId());
            }

            Premiere->setPrec(Inst->getIdPrec());

            this->getInstruction(ListeSuivante[0])->setPrec(Derniere->getId());

            Derniere->RemplacerIdSuivant(Derniere->getIdSuivantParSortie().at(0), ListeSuivante[0]);
        }
    }

    this->ListeInst.removeOne(Inst);
    this->RecalculerId(Inst->getId());
    Inst->deleteLater();
}

/**
 * Retourne une liste des instruction pointant vers l'instruction dont l'id est passé en parametre
 * @brief Routine::getInstructionPrecedentDe
 * @param nId   L'id à scanner
 * @return une liste des instruction pointant vers l'instruction dont l'id est passé en parametre
 */
QList<Instruction*> Routine::getInstructionPrecedentDe(unsigned int nId)
{
    QList<Instruction*> Retour;

    for(register int i = 0; i < this->ListeInst.length(); i++)
    {
        QList<unsigned int> nIdSuivant  (this->ListeInst[i]->getIdSuivantParSortie()) ;

        for(register int j = 0; j < nIdSuivant.length(); j++)
        {
            if(nIdSuivant[j] ==  nId)
            {
                Retour << this->ListeInst[i];
            }
        }
    }

    return Retour ;
}

/**
 * Instruction commune à tout les constructeurs
 *
 * @brief   Routine::Constructor()
 * @see     Routine::Routine(QObject *  pParent = 0), Routine::Routine(QString Nom, QObject *  pParent = 0), Instruction, DebFinInst
 */
void Routine::Constructeur(bool bSansDepart)
{
    this->ListeInst.clear() ;

    //s'il on demande une base d'organogramme
    if(!bSansDepart)
    {
        Inst_DebFin *   Debut   (new Inst_DebFin(0, this, true)) ;
        Inst_DebFin *   Fin     (new Inst_DebFin(1, this, false)) ;

        Debut->setSuivant(1) ;
        Fin->setPrec(0) ;

        this->ListeInst << Debut;

        this->ListeInst << Fin;
    }
}

/**
 * Recalcule tous les id des instructions en ca de suppression
 * @brief Routine::RecalculerId
 * @param Seuil     L'id supprimé
 */
void Routine::RecalculerId(unsigned int Seuil)
{
    for(register int i = 0; i < ListeInst.length(); i++)
    {
        ListeInst[i]->RecalculerId(Seuil);
    }
}

/**
 * Enregistre au format XML
 * @brief Routine::Enregistrer
 * @param Parser    Le parseur à utiliser
 */
void Routine::Enregistrer(QXmlStreamWriter* Parser)
{
    Parser->writeStartElement("Routine");
    Parser->writeAttribute("nId", QVariant(this->nId).toString());
    Parser->writeAttribute("sNom", this->sNom);

    for(register int i = 0; i < ListeInst.length(); i++)
    {
        ListeInst[i]->Enregistrer(Parser);
    }

    Parser->writeEndElement();
}
