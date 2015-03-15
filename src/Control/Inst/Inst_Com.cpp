//-------------------------------------------------------------------------------
/**
 * @file        Inst_Com.cpp
 * @brief       Commentaire
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, LycÃ©e Nicolas APPERT, ORVAULT (FRANCE)
 * @since       17/03/14
 * @version     1.0
 * @date        17/03/14
 *
 * ReprÃ©sente un commentaire inséré dans l'organigramme
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
#include "Inst_Com.h"
#include "../Compil_Inst.h"

/**
 * Constructeur, construit une instruction de commentaire avec les valeurs donnée en paramettre et les valeurs par défauts pour les autres.
 *
 * @brief   Inst_Com::Inst_Com
 * @param   nId         Id de l'instruction
 * @param   pRoutine    Pointeur vers la routine contenant l'instruction
 * @see     Instruction
 */
Inst_Com::Inst_Com(unsigned int nId, Routine *pRoutine) :
    Instruction (nId, pRoutine),
    sCommentaire("Commentaire")
{
}

Compil_Inst *Inst_Com::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    Retour->AjouterUnOpCode('\x0');

    return Retour;
}

/**
 * Renvois une chaine de caractère lisible, descriptive de l'instruction pour l'afficher dans un item
 *
 * @brief   Inst_Com::toString
 * @return  Chaine descriptive
 */
QString Inst_Com::toString()
{
    QString Retour ;

    if(this->sCommentaire.length() > 50)    //Si on a plus de 50 caractères, on le tronque
    {
        Retour = this->sCommentaire.left(47) ;
        Retour += "..." ;
    }
    else    //Sinon on renvois directe le commentaire
    {
        Retour = this->sCommentaire ;
    }

    return Retour ;
}

/**
 * Ouvre une fenetre de modification pour l'instruction
 *
 * @brief Inst_Com::Modifier
 */
bool Inst_Com::Modifier()
{
    bool            Retour      (true);
    f_ModInst_Com   FenetreModif(this) ;
    int             reponse     (0) ;

    //Exécution de la fenêtre
    reponse = FenetreModif.exec() ;

    //Si l'utilisateur a cliqué sur Ok
    if(reponse == QDialog::Accepted)
    {
        //On met en place les nouvelles propriétés
        this->setDesc(FenetreModif.getDescription()) ;
    }
    else
    {
        Retour = false;
    }

    return Retour;
}

/**
 * Mutateur générale pour ouverture à partir d'un fichier
 * @brief Inst_Com::setAttr
 * @param NomAttr   Le nom de l'attribut à modifier
 * @param Valeur    Sa valeur
 */
void Inst_Com::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "sCommentaire")
    {
        QString nValeur     (Valeur.toString()) ;

        this->sCommentaire = nValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur);
}

/**
 * Mutateur de la description
 * @brief Inst_Com::setDesc
 * @param Desc
 */
void Inst_Com::setDesc(DescCommentaire Desc)
{
    this->sCommentaire = Desc.sCommentaire ;
}

/**
 * Ascesseur de la description
 * @brief Inst_Com::getDesc
 * @return
 */
DescCommentaire Inst_Com::getDesc()
{
    f_ModInst_Com f_Desc(this);
    return f_Desc.getDescription();
}

/**
 * Enregistre au format XML
 * @brief Inst_Com::Enregistrer
 * @param Parser    Le parseur à utiliser pour l'enregistrement
 */
void Inst_Com::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeEmptyElement("sCommentaire");
    Parser->writeAttribute("valeur", this->sCommentaire);

    this->FinBalise(Parser);
}
