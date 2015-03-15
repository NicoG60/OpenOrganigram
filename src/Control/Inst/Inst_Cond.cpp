//-------------------------------------------------------------------------------
/**
 * @file        Inst_Cond.cpp
 * @brief       Instruction de condition
 *
 * @author      N.Jarnoux
 * @author      STS IRIS, LycÃ©e Nicolas APPERT, ORVAULT (FRANCE)
 * @since       17/03/14
 * @version     1.0
 * @date        17/03/14
 *
 * ReprÃ©sente une opÃ©ration sur une variable dans l'organigramme. Les opÃ©rations acceptÃ©es sont l'affectation,
 * l'incrementation, la dÃ©crÃ©mentation, l'addition et la soustraction.
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
#include "Inst_Cond.h"
#include "../Compil_Inst.h"

bool Inst_Cond::AttenteReponse = false;

/**
 * Constructeur.
 *
 * @brief   Inst_Cond::Inst_Cond
 * @param   nId     Id de l'instruction
 * @param   nParent Id de la condition parente
 * @see     Instruction
 */
Inst_Cond::Inst_Cond(unsigned int nId, Routine *pRoutine) :
    Instruction     (nId, pRoutine),
    nIndiceVar      (0),
    TypeComp        (EGUAL),
    nValeur         (0)
{
    this->nIdSuivant.append(0);
}

/**
 * Constructeur.
 *
 * @brief   Inst_Cond::Inst_Cond
 * @param   Dzscription Le descripteur de la condition
 * @see     Instruction
 */
Inst_Cond::Inst_Cond(DescCondition Description) :
    Instruction     (0, 0),
    TCondition      (Description.TCondition),
    nIndiceVar      (Description.nIndiceVar),
    TypeComp        (Description.TypeComp),
    nValeur         (Description.nValeur),
    TestMat         (Description.TestMat)
{
}

Compil_Inst *Inst_Cond::Compiler()
{
    Compil_Inst* Retour(new Compil_Inst(this));

    QByteArray Condition    (this->CompilationStatiqueSansLiens(this->getDesc()));

    Retour->AjouterUnOpCode(Condition);
    Retour->AjouterLienVers(this->pRoutine->getInstruction(this->nIdSuivant[0])->getIdentifiantInst());

    return Retour;
}

/**
 * Fonction d'exécution de l'instruction. Elle choisis le type d'opération à effectuer et déclenche les bonnes méthodes en fonction
 *
 * @brief   Inst_Cond::Executer(ModuleManager *    Manager)
 * @param   Manager pointeur vers le manager de module contenant l'instruction
 * @see     ModuleManager
 */
void Inst_Cond::Executer()
{
    unsigned int    Retour          (0) ;
    IdentifiantInst Identifiant ;
    bool            ReponseCondition(false) ;

    switch (this->TCondition) {
    case LOG:
        ReponseCondition = this->ComparaisonLogicielle(this->pRoutine->getManagerParent()) ;
        break ;

    case MAT:
        ReponseCondition = this->ComparaisonMaterielle(this->pRoutine->getManagerParent()) ;
        break ;
    }
    if(!AttenteReponse)
    {
        if(ReponseCondition)
        {
            Retour = this->nIdSuivant[0] ;
        }
        else
        {
            Retour = this->nIdSuivant[1] ;
        }
    }
    else
    {
        Retour = this->getId() ;
    }


    Identifiant.nIdRoutine = this->pRoutine->getId() ;
    Identifiant.nIdInstruction = Retour;

    this->pRoutine->getManagerParent()->AjouterALaPile(Identifiant);
}

/**
 * Génération du texte à afficher dans l'organigramme.
 *
 * @brief   Inst_Cond::toString()
 * @return  Retour la chaine à afficher
 * @see     Item
 */
QString Inst_Cond::toString()
{
    QString     Retour ;
    if(this->TCondition == LOG)
    {
        char        NomVariable (this->nIndiceVar + 65) ;
        QVariant    ValeurToStr (this->nValeur) ;

        Retour += NomVariable ;
        Retour += " " ;

        switch(this->TypeComp)
        {
        case EGUAL :
            Retour += "= " ;
            break ;

        case SUP :
            Retour += "> " ;
            break ;

        case INF :
            Retour += "< " ;
            break ;

        case SUP_EG :
            Retour +=  ">= " ;
            break ;

        case INF_EG :
            Retour += "<= " ;
            break ;

        case DIFF :
            Retour += "<> " ;
            break ;
        }

        Retour += ValeurToStr.toString() ;
        Retour += " ?" ;
    }
    else
    {
        Retour = this->TestMat.sNomTest;
    }

    return Retour ;
}

/**
 * Lance la modification de cet instruction
 *
 * @brief   Inst_Cond::Modifier()
 * @see     ModInst_Ope
 */
bool Inst_Cond::Modifier()
{
    bool            Retour          (true);
    f_ModInst_Cond  FenetreModif    (this) ;
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

    return Retour ;
}

/**
 * Mutateur du type de condition (matérielle ou logicielle)
 *
 * @brief   Inst_Cond::setTCondition(TypeCondition TCondition)
 */
void Inst_Cond::setTCondition(TypeCondition TCondition)
{
    this->TCondition = TCondition ;
}

/**
 * Mutateur de l'indice de la variable visée
 *
 * @brief   Inst_Cond::setIndiceVar(unsigned int nIndiceVar)
 */
void Inst_Cond::setIndiceVar(unsigned int nIndiceVar)
{
    if(nIndiceVar < 26)
    {
        this->nIndiceVar = nIndiceVar ;
    }
}

/**
 * Mutateur du type de comparaison voulu (egalité, superiorité... )
 *
 * @brief   Inst_Cond::setTypeComp(TypeComparaison TypeComp)
 */
void Inst_Cond::setTypeComp(TypeComparaison TypeComp)
{
    this->TypeComp = TypeComp ;
}

/**
 * Mutateur de l'indice de la variable
 *
 * @brief   Inst_Cond::setValeur(unsigned short nValeur)
 */
void Inst_Cond::setValeur(unsigned short nValeur)
{
    if(nValeur <= 65025)
    {
        this->nValeur = nValeur ;
    }
}

void Inst_Cond::setAttr(QString NomAttr, QStringRef Valeur)
{
    if(NomAttr == "nIdSuivant")
    {
        unsigned int nValeur    (Valeur.toInt()) ;

        if( this->nIdSuivant[0] == 0)
        {
            this->nIdSuivant[0] = nValeur ;
        }

    }

    if(NomAttr == "TCondition")
    {
        int             nValeur     (Valeur.toInt()) ;
        TypeCondition   tValeur     (static_cast<TypeCondition>(nValeur));

        this->TCondition = tValeur;
    }

    if(NomAttr == "nIndiceVar")
    {
        unsigned int nValeur    (Valeur.toInt()) ;

        this->nIndiceVar = nValeur ;
    }

    if(NomAttr == "TypeComp")
    {
        int             nValeur (Valeur.toInt()) ;
        TypeComparaison tValeur (static_cast<TypeComparaison>(nValeur));

        this->TypeComp = tValeur ;
    }

    if(NomAttr == "nValeur")
    {
        unsigned short nValeur  (Valeur.toUShort()) ;

        this->nValeur = nValeur ;
    }

    if(NomAttr == "TestMat")
    {
        TestArduino NouveauTest;

        NouveauTest.sNomTest    = "";
        NouveauTest.sCommande   = "";
        NouveauTest.bActive     = false ;

        this->TestMat = NouveauTest;
    }

    if(NomAttr == "sNomTest")
    {
        QString sValeur (Valeur.toString());

        this->TestMat.sNomTest = sValeur ;
    }

    if(NomAttr == "sCommande")
    {
        QString sValeur (Valeur.toString());

        this->TestMat.sCommande = sValeur ;
    }

    if(NomAttr == "bActive")
    {
        QString sValeur (Valeur.toString());
        bool    bValeur (false);

        if(sValeur == "true")
        {
            bValeur = true;
        }

        this->TestMat.bActive = bValeur ;
    }

    Instruction::setAttr(NomAttr, Valeur);
}

void Inst_Cond::setDesc(DescCondition Desc)
{
    this->TCondition    = Desc.TCondition ;
    this->nIndiceVar    = Desc.nIndiceVar ;
    this->TypeComp      = Desc.TypeComp ;
    this->nValeur       = Desc.nValeur ;
    this->TestMat       = Desc.TestMat ;
}

DescCondition Inst_Cond::getDesc()
{
    f_ModInst_Cond f_Desc(this);
    return f_Desc.getDescription();
}

Instruction* Inst_Cond::getDernierInstBrancheEcartee()
{
    unsigned int    Retour          (0);
    Instruction*    InstCourante    (this->pRoutine->getInstruction(this->nIdSuivant[1]));
    Instruction*    InstSuivante    (this->pRoutine->getInstruction(InstCourante->getIdSuivantParSortie().at(0)));
    bool            SortieBoucle    (false);

    while(!SortieBoucle && InstCourante->getId() != 1)
    {
        if(InstSuivante->getIdPrec() != InstCourante->getId())
        {
            Retour = InstCourante->getId();
            SortieBoucle = true;
        }
        else
        {
            InstCourante = InstSuivante;
            InstSuivante = this->pRoutine->getInstruction(InstCourante->getIdSuivantParSortie().at(0));
        }
    }

    return this->pRoutine->getInstruction(Retour) ;
}

void Inst_Cond::Enregistrer(QXmlStreamWriter* Parser)
{
    this->DebutBalise(Parser);

    Parser->writeEmptyElement("TCondition");
    Parser->writeAttribute("valeur", QVariant((int)this->TCondition).toString());

    Parser->writeEmptyElement("nIndiceVar");
    Parser->writeAttribute("valeur", QVariant(this->nIndiceVar).toString());

    Parser->writeEmptyElement("nValeur");
    Parser->writeAttribute("valeur", QVariant(this->nValeur).toString());

    Parser->writeStartElement("TestMat");

        Parser->writeEmptyElement("bActive");

        if(this->TestMat.bActive)
        {
            Parser->writeAttribute("valeur", "true");
        }
        else
        {
            Parser->writeAttribute("valeur", "false");
        }

        Parser->writeEmptyElement("sCommande");
        Parser->writeAttribute("valeur", this->TestMat.sCommande);

        Parser->writeEmptyElement("sNomTest");
        Parser->writeAttribute("valeur", this->TestMat.sNomTest);

    Parser->writeEndElement();

    this->FinBalise(Parser);
}

/**
 * Methode statique, effectue la comparaison avec les élément fournient dans la description
 *
 * @brief   Inst_Cond::ComparaisonStatique
 * @param   Manager     Manager de module parent
 * @param   Description Description de la condition
 * @return  Le retour de la condition
 */
bool Inst_Cond::ComparaisonStatique(ModuleManager * Manager, DescCondition Description)
{
    Inst_Cond   Condition(Description) ;

    bool        ReponseCondition(false) ;

    switch (Condition.getTCondition()) {
    case LOG:
        ReponseCondition = Condition.ComparaisonLogicielle(Manager) ;
        break ;

    case MAT:
        ReponseCondition = Condition.ComparaisonMaterielle(Manager) ;
        break ;
    }

    return ReponseCondition ;
}

QByteArray Inst_Cond::CompilationStatiqueSansLiens(DescCondition Description)
{
    QByteArray Retour;

    if(Description.TCondition == MAT)
    {
        if(Description.TestMat.sCommande[0] == 'R')
        {
            Retour += '\xA';

            char Broche (Description.TestMat.sCommande.mid(1, 2).toInt());

            Retour += Broche;

            char Val (Description.TestMat.sCommande.right(1).toInt());

            Retour += Val;
        }
        else if(Description.TestMat.sCommande[0] == 'I')
        {
            Retour += '\x13';

            QByteArray BoutonTel;
            BoutonTel += Description.TestMat.sCommande.mid(4, 8).leftJustified(8, ' ');

            Retour += BoutonTel;
        }
    }
    else if(Description.TCondition == LOG)
    {
        switch(Description.TypeComp)
        {
        case EGUAL :
            Retour += '\xB';
            break ;

        case INF :
            Retour += '\xC';
            break ;

        case SUP :
            Retour += '\xD';
            break ;

        case INF_EG :
            Retour += '\xE';
            break ;

        case SUP_EG :
            Retour += '\xF';
            break ;

        case DIFF :
            Retour += '\x10';
            break ;
        }

        char Variable   (Description.nIndiceVar+1);
        Retour += Variable;

        char MsbVal     (Description.nValeur >> 8);
        char LsbVal     (Description.nValeur & 0x00FF);

        Retour += MsbVal;
        Retour += LsbVal;
    }

    return Retour;
}

QByteArray Inst_Cond::CompilationStatiqueInverseeSansLiens(DescCondition Description)
{
    QByteArray Retour;

    if(Description.TCondition == MAT)
    {
        if(Description.TestMat.sCommande[0] == 'R')
        {
            Retour += '\xA';

            char Broche (Description.TestMat.sCommande.mid(1, 2).toInt());

            Retour += Broche;

            char Val (Description.TestMat.sCommande.right(1).toInt());

            if(Val == 1)
            {
                Retour += '\x0';
            }
            else
            {
                Retour += '\x1';
            }
        }
        else if(Description.TestMat.sCommande[0] == 'I')
        {
            Retour += '\x13';

            QByteArray BoutonTel;
            BoutonTel += QString("-1").leftJustified(8, ' ');

            Retour += BoutonTel;
        }
    }
    else if(Description.TCondition == LOG)
    {
        switch(Description.TypeComp)
        {
        case DIFF :
            Retour += '\xB';
            break ;

        case SUP :
            Retour += '\xC';
            break ;

        case INF :
            Retour += '\xD';
            break ;

        case SUP_EG :
            Retour += '\xE';
            break ;

        case INF_EG :
            Retour += '\xF';
            break ;

        case EGUAL :
            Retour += '\x10';
            break ;
        }

        char Variable   (Description.nIndiceVar+1);
        Retour += Variable;

        char MsbVal     (Description.nValeur >> 8);
        char LsbVal     (Description.nValeur & 0x00FF);

        Retour += MsbVal;
        Retour += LsbVal;
    }

    return Retour;
}

/**
 * Methode statique effectuant l'équivalent de toString à partir de élément de la description
 *
 * @brief Inst_Cond::toStringStatique
 * @param Description   Description à convertir
 * @return La chaine à afficher
 * @see toString
 */
QString Inst_Cond::toStringStatique(DescCondition Description)
{
    Inst_Cond Condition(Description) ;

    return Condition.toString() ;
}


/**
 * Effectue une comparaison avec les donnée logicielle fournies (variable, valeur... ) et retourne vrai si la condition est vérifiée et faux sinon.
 *
 * @brief   Inst_Cond::ComparaisonMaterielle()
 * @param   Manager Le manager à utiliser pour la comparaison
 * @return  bool    Vrai si condition vérifiée / Faux sinon
 */
bool Inst_Cond::ComparaisonLogicielle(ModuleManager *   Manager)
{
    bool Retour (false) ;

    unsigned short nValeurVariable  (Manager->getValVar(this->nIndiceVar)) ;

    switch(this->TypeComp)
    {
    case EGUAL :
        Retour = (nValeurVariable == this->nValeur) ;
        break ;

    case SUP :
        Retour = (nValeurVariable > this->nValeur) ;
        break ;

    case INF :
        Retour = (nValeurVariable < this->nValeur) ;
        break ;

    case SUP_EG :
        Retour = (nValeurVariable >= this->nValeur) ;
        break ;

    case INF_EG :
        Retour = (nValeurVariable <= this->nValeur) ;
        break ;

    case DIFF :
        Retour = (nValeurVariable != this->nValeur) ;
        break ;
    }

    this->AttenteReponse = false;

    return Retour ;
}

/**
 * Effectue une comparaison avec les donnée materielle fournies (appuie sur un BP, valeur d'un potar...) et retourne vrai si la condition est vérifiée et faux sinon.
 *
 * @brief   Inst_Cond::ComparaisonMaterielle()
 * @param   Manager à utiliser pour la comparaison
 * @return  bool    Vrai si condition vérifiée / Faux sinon
 */
bool Inst_Cond::ComparaisonMaterielle(ModuleManager *   Manager)
{
    bool Retour (false);

    QString Commande        (this->TestMat.sCommande);
    int     PositionEgual   (0);

    for(register int i = 0; i < Commande.length(); i++)
    {
        if(Commande[i] == '=')
        {
            PositionEgual = i;
        }
    }

    if(!AttenteReponse)
    {
        Manager->EnvoyerCommande(Commande.left(PositionEgual));
        AttenteReponse = true;
    }
    else
    {
        QByteArray  Reponse (Manager->getDerniereReponse());

        if(Reponse.mid(6) == Commande.mid(PositionEgual + 1))
        {
            Retour = true;
        }

        AttenteReponse = false;
    }

    return Retour;
}

