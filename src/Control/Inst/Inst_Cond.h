//  Inst_Cond.h  17/03/14    1.0     N. Jarnoux
#ifndef INST_COND_H
#define INST_COND_H

class Inst_Cond ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"
#include "../ModInst/f_ModInst_Cond.h"

class Inst_Cond : public Instruction
{
    public:
                                Inst_Cond(unsigned int nId, Routine *pRoutine) ;           //Constructeur
                                Inst_Cond(DescCondition Description) ;  //Constructeur privé pour construire un objet a partie d'une description (util pour les methodes statiques)

        inline  TypeCommande    getType()   { return COND ; }               //Accesseur de typer d'instruction
                Compil_Inst*    Compiler() ;
                void            Executer() ;                                //Execute l'instruction
                QString         toString() ;                                //Génére la chaine de caractÃ¨re Ã  afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                                //Lance la fenetre de modification

        inline  TypeCondition   getTCondition() { return TCondition ; }     //Accesseur du type de condition
        inline  unsigned int    getIndiceVar()  { return nIndiceVar ; }     //Accesseur de l'indice de variable
        inline  TypeComparaison getTypeComp()   { return TypeComp ; }       //Accesseur du type de comparaison
        inline  unsigned short  getValeur()     { return nValeur ; }        //Accesseur de la valeur à comparer
                void            setTCondition(TypeCondition TCondition) ;   //Mutateur du type de condition
                void            setIndiceVar(unsigned int nIndiceVar) ;     //Mutateur de l'indice de variable
                void            setTypeComp(TypeComparaison TypeComp) ;     //Mutateur du type de comparaison
                void            setValeur(unsigned short nValeur) ;         //Mutateur de la valeur à comparer
                void            setAttr(QString NomAttr, QStringRef Valeur);
                void            setDesc(DescCondition Desc);
                DescCondition   getDesc();
                Instruction*    getDernierInstBrancheEcartee();
                void            Enregistrer(QXmlStreamWriter* Parser);

        static  bool            ComparaisonStatique(ModuleManager * Manager, DescCondition Description) ;   //Effectue une comparaison avec les donnée fournie en description
        static  QString         toStringStatique(DescCondition Description) ;                               //Effectue la conversion en chaine de caractère de la description fournie
        static  QByteArray      CompilationStatiqueSansLiens(DescCondition Description) ;
        static  QByteArray      CompilationStatiqueInverseeSansLiens(DescCondition Description) ;
                bool            ComparaisonLogicielle(ModuleManager *    Manager) ;                         //Comparaison logicielle entre les parametre fournis dans l'objet
                bool            ComparaisonMaterielle(ModuleManager *    Manager) ;                         //------------materielle-----------------------------------------
                static  bool    AttenteReponse;


    private:


                TypeCondition   TCondition ;    //Indique s'il s'agit d'une comparaison logicielle ou matérielle

                unsigned int    nIndiceVar ;    //Indice de la variable Ã  comparer
                TypeComparaison TypeComp ;      //Type de comparaison
                unsigned short  nValeur ;       //Valeur Ã  laquelle comparer

                //Attributs pour la comp matérielle
                TestArduino     TestMat ;
};


#endif // INST_COND_H

