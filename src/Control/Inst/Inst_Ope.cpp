//-------------------------------------------------------------------------------
/**
 * @file        Inst_Ope.cpp
 * @brief       Instruction d'opération sur une variable.
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, Lycée Nicolas APPERT, ORVAULT (FRANCE)
 * @since       01/03/14
 * @version     1.0
 * @date        03/03/14
 *
 * Représente une opération sur une variable dans l'organigramme. Les opérations acceptées sont l'affectation,
 * l'incrementation, la décrémentation, l'addition et la soustraction.
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
#include "Inst_Ope.h"
#include "../ModInst/f_ModInst_Ope.h"
#include "../Compil_Inst.h"

/**
 * Constructeur.
 *
 * @brief   Inst_Ope::Inst_Ope(int nId, int nParent = 0, int nNiveau = 0)
 * @param   nId     Id de l'instruction
 * @param   nParent Id de la condition parente
 * @see     Instruction
 */
Inst_Ope::Inst_Ope(unsigned int nId, Routine *pRoutine) :
    Instruction     (nId, pRoutine),
    nIndiceVariable (0),
    Operation       (AFFECTER),
    nValeur         (0)
{

}

Compil_Inst *Inst_Ope::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    char Var    (this->nIndiceVariable);
    char Val1   (this->nValeur >> 8);
    char Val2   (this->nValeur & 0x00FF);

    switch(this->Operation)
    {
    case AFFECTER :
        Retour->AjouterUnOpCode('\x1');
        Retour->AjouterUnOpCode(Var);
        Retour->AjouterUnOpCode(Val1);
        Retour->AjouterUnOpCode(Val2);
        break ;

    case INCREMENTER :
        Retour->AjouterUnOpCode('\x2');
        Retour->AjouterUnOpCode(Var);
        break ;

    case DECREMENTER :
        Retour->AjouterUnOpCode('\x3');
        Retour->AjouterUnOpCode(Var);
        break ;

    case AJOUTER :
        for(register int i = 0; i < this->nValeur; i++)
        {
            Retour->AjouterUnOpCode('\x2');
            Retour->AjouterUnOpCode(Var);
        }
        break;

    case SOUSTRAIRE :
        for(register int i = 0; i < this->nValeur; i++)
        {
            Retour->AjouterUnOpCode('\x3');
            Retour->AjouterUnOpCode(Var);
        }
        break ;
    }

    return Retour;
}

/**
 * Fonction d'exécution de l'instruction. Elle choisis le type d'opération à effectuer et déclenche les bonnes méthodes en fonction
 *
 * @brief   Inst_Ope::Executer(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Executer()
{
    //En fonction du type d'opération
    switch(this->Operation)
    {
    case AFFECTER :
        this->Affecter(this->pRoutine->getManagerParent()) ;
        break ;

    case INCREMENTER :
        this->Incrementer(this->pRoutine->getManagerParent()) ;
        break ;

    case DECREMENTER :
        this->Decrementer(this->pRoutine->getManagerParent()) ;
        break ;

    case AJOUTER :
        this->Ajouter(this->pRoutine->getManagerParent()) ;
        break;

    case SOUSTRAIRE :
        this->Soustraire(this->pRoutine->getManagerParent()) ;
        break ;
    }

    //On place l'instruction suviante dans la pile
    IdentifiantInst Identifiant;

    Identifiant.nIdRoutine = this->pRoutine->getId() ;
    Identifiant.nIdInstruction = this->nIdSuivant[0] ;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Génération du texte à afficher dans l'organigramme.
 *
 * @brief   Inst_Ope::toString()
 * @return  Retour la chaine à afficher
 * @see     Item
 */
QString Inst_Ope::toString()
{
    QString     Retour ;
    char        NomVariable (this->nIndiceVariable + 65) ;
    QVariant    ValeurToStr (this->nValeur) ;

    Retour += NomVariable ;
    Retour += " <- " ;

    //En fonction du type d'opération
    switch(this->Operation)
    {
    case AFFECTER :
        Retour += ValeurToStr.toString();
        break ;

    case INCREMENTER :
        Retour += NomVariable ;
        Retour += " + 1" ;
        break ;

    case DECREMENTER :
        Retour += NomVariable ;
        Retour += " - 1" ;
        break;

    case AJOUTER :
        Retour += NomVariable ;
        Retour += " + " ;
        Retour += ValeurToStr.toString() ;
        break;

    case SOUSTRAIRE :
        Retour += NomVariable ;
        Retour += " - " ;
        Retour += ValeurToStr.toString() ;
        break;
    }

    return Retour;
}

/**
 * Lance la modification de cet instruction
 *
 * @brief   Inst_Ope::Modifier()
 * @see     ModInst_Ope
 */
bool Inst_Ope::Modifier()
{
    bool            Retour          (true);
    f_ModInst_Ope   FenetreModif    (this) ;
    int             reponse         (0) ;

    //Exécution de la fenêtre
    reponse = FenetreModif.exec() ;

    //Si l'utilisateur clic sur Ok
    if(reponse == QDialog::Accepted)
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
 * Mutateur de l'indice de la variable
 *
 * @brief   Inst_Ope::setIndiceVariable(unsigned int nIndiceVariable
 */
void Inst_Ope::setIndiceVariable(unsigned int nIndiceVariable)
{
    if(nIndiceVariable < 26)
    {
        this->nIndiceVariable = nIndiceVariable ;
    }
}

/**
 * Mutateur de l'indice de la variable
 *
 * @brief   Inst_Ope::setOperation(TypeOperation Operation)
 */
void Inst_Ope::setOperation(TypeOperation Operation)
{
    this->Operation = Operation ;
}

/**
 * Mutateur de l'indice de la variable
 *
 * @brief   Inst_Ope::setValeur(unsigned short nValeur)
 */
void Inst_Ope::setValeur(unsigned short nValeur)
{
    if(nValeur <= 65025)
    {
        this->nValeur = nValeur ;
    }
}

/**
 * Mutateur général pour ouverture de fichier
 * @brief Inst_Ope::setAttr
 * @param NomAttr   Nom de l'attribut
 * @param Valeur    Sa valeur
 */
void Inst_Ope::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "nIndiceVariable")
    {
        unsigned int nValeur    (Valeur.toInt()) ;

        this->nIndiceVariable = nValeur ;
    }

    if(NomAttr == "Operation")
    {
        int             nValeur (Valeur.toInt()) ;
        TypeOperation   tValeur (static_cast<TypeOperation>(nValeur));

        this->Operation = tValeur ;
    }

    if(NomAttr == "nValeur")
    {
        unsigned short nValeur  (Valeur.toUShort()) ;

        this->nValeur = nValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur);
}

/**
 * Mutateur de description
 * @brief Inst_Ope::setDesc
 * @param Desc
 */
void Inst_Ope::setDesc(DescOperation Desc)
{
    this->setIndiceVariable(Desc.nIndiceVariable) ;
    this->setOperation(Desc.Operation) ;
    this->setValeur(Desc.nValeur) ;
}

/**
 * Ascesseur de description
 * @brief Inst_Ope::getDesc
 * @return
 */
DescOperation Inst_Ope::getDesc()
{
    f_ModInst_Ope f_Desc(this);
    return f_Desc.getDescription();
}

/**
 * Enregistrement au format XML
 * @brief Inst_Ope::Enregistrer
 * @param Parser    Le parseur à utiliser
 */
void Inst_Ope::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeEmptyElement("Operation");
    Parser->writeAttribute("valeur", QVariant((int)this->Operation).toString());

    Parser->writeEmptyElement("nIndiceVariable");
    Parser->writeAttribute("valeur", QVariant(this->nIndiceVariable).toString());

    Parser->writeEmptyElement("nValeur");
    Parser->writeAttribute("valeur", QVariant(this->nValeur).toString());

    this->FinBalise(Parser);
}

/**
 * Fonction d'affectation.
 *
 * @brief   Inst_Ope::Affecter(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Affecter(ModuleManager *    Manager)
{
    Manager->setValVar(this->nIndiceVariable, this->nValeur) ;
}

/**
 * Fonction d'incrementation.
 *
 * @brief   Inst_Ope::Incrementer(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Incrementer(ModuleManager *    Manager)
{
    unsigned short nVariable (Manager->getValVar(this->nIndiceVariable)) ;

    nVariable++ ;

    Manager->setValVar(this->nIndiceVariable, nVariable) ;
}

/**
 * Fonction d'décrémentation.
 *
 * @brief   Inst_Ope::Decrementer(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Decrementer(ModuleManager *    Manager)
{
    unsigned short nVariable (Manager->getValVar(this->nIndiceVariable)) ;

    nVariable-- ;

    Manager->setValVar(this->nIndiceVariable, nVariable) ;
}

/**
 * Fonction d'addition.
 *
 * @brief   Inst_Ope::Ajouter(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Ajouter(ModuleManager *    Manager)
{
    for(register unsigned short i = 0; i < this->nValeur; i++ )
    {
        this->Incrementer(Manager) ;
    }
}

/**
 * Fonction de soustraction.
 *
 * @brief   Inst_Ope::Soustraire(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Ope::Soustraire(ModuleManager *    Manager)
{
    for(register unsigned short i = 0; i < this->nValeur; i++ )
    {
        this->Decrementer(Manager) ;
    }
}
