//-------------------------------------------------------------------------------
/**
 * @file        Inst_DebFin.cpp
 * @brief       Item de début ou de fin d'organigramme
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       21/02/14
 * @version     1.0
 * @date        03/03/14
 *
 * REprésente le début ou la fin d'un organigramme. cette instruction ne fait pas d'actions en particulier,
 * elle délimite simplement les bornes du programmes.
 *
 * Fabrication  OpenOrganigram.pro
 *
 * @todo        Tester si la classe fonctionne
 *
 * @bug         /
 */
//-------------------------------------------------------------------------------

//===== Headers standards =====

//=====   Headers Peros   =====
#include "Inst_DebFin.h"
#include "../Compil_Inst.h"

/**
 * Constructeur.
 *
 * @brief   Inst_DebFin::Inst_DebFin(int nId)
 * @param   nId     Id de l'instruction, normalement obligatoirement 0 ou 1 puisque ce sont les deux premieres créées
 * @see     Instruction
 */
Inst_DebFin::Inst_DebFin(unsigned int nId, Routine *pRoutine) :
    Instruction(nId, pRoutine)
{
}

/**
 * Constructeur.
 *
 * @brief   Inst_DebFin::Inst_DebFin(int nId, bool bDebut)
 * @param   nId     Id de l'instruction, normalement obligatoirement 0 ou 1 puisque ce sont les deux premieres créées
 * @param   bDebut  indique si c'est le début ou la fin qu'il doit représenter
 * @see     Instruction
 */
Inst_DebFin::Inst_DebFin(unsigned int nId, Routine *pRoutine, bool bDebut) :
    Instruction (nId, pRoutine),
    bDebut      (bDebut)
{
}

Compil_Inst *Inst_DebFin::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    if(this->bDebut)
        Retour->AjouterUnOpCode('\x0');
    else
    {
        if(this->pRoutine->getId() == 0)
            Retour->AjouterUnOpCode('\xFF');
        else
        {
            Retour->AjouterUnOpCode('\x12');
            Retour->AjouterUnOpCode('\x00');
            Retour->AjouterUnOpCode('\x00');
        }
    }

    return Retour;
}

/**
 * Génération du texte à afficher dans l'organigramme.
 *
 * @brief   Inst_DebFin::toString()
 * @return  Retour la chaine à afficher
 * @see     Item
 */
QString Inst_DebFin::toString()
{
    QString Retour ;

    if(this->bDebut)
    {
        Retour = "Début" ;
    }
    else
    {
        Retour = "Fin" ;
    }

    return Retour ;
}

/**
 * Exécute l'instruction
 * @brief Inst_DebFin::Executer
 */
void Inst_DebFin::Executer()
{
    //C'est c'est le début
    if(this->bDebut)
    {
        //On passe au suivant
        IdentifiantInst Identifiant;

        Identifiant.nIdRoutine      = this->pRoutine->getId() ;
        Identifiant.nIdInstruction  = this->nIdSuivant[0] ;

        this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
    }

    //Sinon on arrête l'exécution
}

/**
 * Mutateur générale pour ouverture de fichier
 * @brief Inst_DebFin::setAttr
 * @param NomAttr   Nom de l'attributs
 * @param Valeur    Sa valeur
 */
void Inst_DebFin::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "bDebut")
    {
        QString sValeur (Valeur.toString());
        bool    bValeur (false);

        if(sValeur == "true")
        {
            bValeur = true ;
        }

        this->bDebut = bValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur);
}

/**
* Enregistre son contenu au format XML
* @brief Inst_DebFin::Enregistrer
* @param Parser
*/
void Inst_DebFin::Enregistrer(QXmlStreamWriter* Parser)
{
    //Début commun
    this->DebutBalise(Parser);

    //Attributs bDebut
    Parser->writeEmptyElement("bDebut");

    if(this->bDebut)
    {
        Parser->writeAttribute("valeur", "true");
    }
    else
    {
        Parser->writeAttribute("valeur", "false");
    }

    //Fin de balise
    this->FinBalise(Parser);
}
