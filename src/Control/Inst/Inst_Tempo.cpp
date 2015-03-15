//-------------------------------------------------------------------------------
/**
 * @file        Inst_Tempo.cpp
 * @brief       Instruction de temporisation (pause dans le programme)
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       01/04/14
 * @version     1.0
 * @date        01/04/14
 *
 * Représente une temlporisation dans le programme
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
#include "Inst_Tempo.h"
#include "../ModInst/f_ModInst_Tempo.h"
#include "../Compil_Inst.h"

/**
 * constructeur
 *
 * @brief Inst_Tempo::Inst_Tempo
 * @param nId       Identifiant de l'instruction
 * @param pRoutine  Routine parente
 */
Inst_Tempo::Inst_Tempo(unsigned int nId, Routine* pRoutine):
    Instruction     (nId, pRoutine),
    nMillisecondes  (0)
{
}

Compil_Inst *Inst_Tempo::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    char Val1   (this->nMillisecondes >> 8);
    char Val2   (this->nMillisecondes & 0x00FF);

    Retour->AjouterUnOpCode('\x11');
    Retour->AjouterUnOpCode(Val1);
    Retour->AjouterUnOpCode(Val2);

    return Retour;
}

/**
 * Execute la tempo
 *
 * @brief   Inst_Tempo::Executer
 * @param   Manager   Manager à utiliser pour la tempo (a priori, pas utile dans cette instruction)
 * @return  Identifiant de l'instruction suivante
 */
void Inst_Tempo::Executer()
{
    //Modifie la temporisation de base
    this->pRoutine->getManagerParent()->setTemporisation(this->nMillisecondes);

    //Ajoute sa suivante dans la pile
    IdentifiantInst Identifiant;

    Identifiant.nIdRoutine      = this->pRoutine->getId() ;
    Identifiant.nIdInstruction  = this->nIdSuivant[0] ;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Renvois une chaine de caractère affichable
 *
 * @brief   Inst_Tempo::toString
 * @return  La chaine affichable
 */
QString Inst_Tempo::toString()
{
    QString     Retour;
    QVariant    NbMillisec(this->nMillisecondes) ;

    Retour += NbMillisec.toString() ;
    Retour += " ms";

    return Retour;
}

/**
 * Ouvre une fenetre de modification
 *
 * @brief Inst_Tempo::Modifier
 */
bool Inst_Tempo::Modifier()
{
    bool            Retour          (true);
    f_ModInst_Tempo FenetreModif    (this) ;
    int             reponse         (0) ;

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
 * Mutateur général pour ouverture de fichier
 * @brief Inst_Tempo::setAttr
 * @param NomAttr   Nom de l'attribut
 * @param Valeur    Sa valeur
 */
void Inst_Tempo::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "nMillisecondes")
    {
        unsigned int nValeur    (Valeur.toInt());

        this->nMillisecondes = nValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur) ;
}

/**
 * Mutateur de description
 * @brief Inst_Tempo::setDesc
 * @param Desc
 */
void Inst_Tempo::setDesc(DescTempo Desc)
{
    this->nMillisecondes = Desc.nMilliseconde ;
}

/**
 * Ascesseur de description
 * @brief Inst_Tempo::getDesc
 * @return
 */
DescTempo Inst_Tempo::getDesc()
{
    f_ModInst_Tempo f_Desc(this);
    return f_Desc.getDescription();
}

/**
 * Enregistre au format XML
 * @brief Inst_Tempo::Enregistrer
 * @param Parser    Le parseur à utiliser
 */
void Inst_Tempo::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeEmptyElement("nMillisecondes");
    Parser->writeAttribute("valeur", QVariant(this->nMillisecondes).toString());

    this->FinBalise(Parser);
}
