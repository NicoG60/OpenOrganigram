//-------------------------------------------------------------------------------
/**
 * @file        Inst_Rout.cpp
 * @brief       Instruction d'appel d'une autre routine
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       20/03/14
 * @version     1.0
 * @date        20/03/14
 *
 * Représente un appel à une routine
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
#include "Inst_Rout.h"
#include "../ModInst/f_ModInst_Rout.h"
#include "../Compil_Inst.h"

/**
 * Constructeur
 *
 * @brief Inst_Rout::Inst_Rout
 * @param nId
 * @param pRoutine
 */
Inst_Rout::Inst_Rout(unsigned int nId, Routine *pRoutine):
    Instruction         (nId, pRoutine),
    sNomRoutine         ("Routine")
{

}

Compil_Inst *Inst_Rout::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    IdentifiantInst AppelRoutine;

    AppelRoutine.nIdInstruction = 0;
    AppelRoutine.nIdRoutine = this->nIdRoutine;

    Retour->AjouterUnOpCode("\x27");
    Retour->AjouterUnOpCode("\x0");
    Retour->AjouterUnOpCode("\x0");
    Retour->AjouterUnOpCode("\x0");

    Retour->AjouterUnOpCode("\x27");
    Retour->AjouterUnOpCode("\x0");
    Retour->AjouterUnOpCode("\x0");
    Retour->AjouterUnOpCode("\x0");

    Retour->AjouterUnOpCode("\x12");
    Retour->AjouterLienVers(AppelRoutine, true);

    return Retour;
}

/**
 * Execute la routine
 *
 * @brief Inst_Rout::Executer
 * @param Manager
 * @return
 */
void Inst_Rout::Executer()
{
    //L'appel de routine va placer deux identifiants : Sa suivante en dessous et au dessus la premiere instruction de la routine appelée
    IdentifiantInst Identifiant;

    Identifiant.nIdRoutine = this->pRoutine->getId() ;
    Identifiant.nIdInstruction = this->nIdSuivant[0] ;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);

    Identifiant.nIdRoutine = this->nIdRoutine ;
    Identifiant.nIdInstruction = 0;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Retourne une chaine de caractère affichable
 *
 * @brief Inst_Rout::toString
 * @return La chaine affichable
 */
QString Inst_Rout::toString()
{
    return this->sNomRoutine ;
}

/**
 * Ouvre une fenetre de modification
 *
 * @brief Inst_Rout::Modifier
 */
bool Inst_Rout::Modifier()
{
    bool            Retour      (true);
    f_ModInst_Rout  FenetreModif(this) ;
    int             reponse     (0) ;

    reponse = FenetreModif.exec() ;
    if(reponse == QDialog::Accepted)
    {
        this->setDesc(FenetreModif.getDescription()) ;
    }
    else
    {
        Retour = false;
    }

    return Retour;
}

/**
 * Mutateur général
 * @brief Inst_Rout::setAttr
 * @param NomAttr   Nom de l'attribut
 * @param Valeur    Sa valeur
 */
void Inst_Rout::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "nIdRoutine")
    {
        int nValeur (Valeur.toInt());

        this->nIdRoutine = nValeur ;
    }

    if(NomAttr == "sNom")
    {
        QString sValeur (Valeur.toString());

        this->sNomRoutine = sValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur) ;
}


/**
 * Mu(tateur de description
 * @brief Inst_Rout::setDesc
 * @param Desc
 */
void Inst_Rout::setDesc(DescRoutine Desc)
{
    this->nIdRoutine    = Desc.nIdRoutine ;
    this->sNomRoutine   = Desc.sNom ;
}

/**
 * Ascesseur de description
 * @brief Inst_Rout::getDesc
 * @return
 */
DescRoutine Inst_Rout::getDesc()
{
    DescRoutine Retour;
    Retour.nIdRoutine   = this->nIdRoutine;
    Retour.sNom         = this->sNomRoutine;
    return Retour;
}

/**
 * Enregistrement au format XML
 * @brief Inst_Rout::Enregistrer
 * @param Parser Le parseur à utiliser
 */
void Inst_Rout::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeEmptyElement("nIdRoutine");
    Parser->writeAttribute("valeur", QVariant(this->nIdRoutine).toString());

    Parser->writeEmptyElement("sNom");
    Parser->writeAttribute("valeur", this->sNomRoutine);

    this->FinBalise(Parser);
}
