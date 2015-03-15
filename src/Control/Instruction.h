//  Instruction.h   1.0     21/02/14    N.Jarnoux
#ifndef INSTRUCTION_H
#define INSTRUCTION_H

class Instruction ;
class Compil_Inst ;

//===== Headers standards =====
#include <QList>

//=====   Headers Peros   =====
#include "TypeCommun.h"
#include "ModuleManager.h"
#include "Routine.h"
//#include "Compil_Inst.h"

class Instruction : public QObject
{
    public:
                                Instruction(unsigned int nId, Routine* pRoutine) ;  //Constructeur
        virtual                 ~Instruction() ;                                    //Destructeur

        static  Instruction *   NouvelleInst(TypeCommande Type, unsigned int nId, Routine * pRoutine) ; //Crée une nouvelle instruction

        static  QString         ConvertirVersChaine(DescCommentaire Desc);      //Conversion d'une description en chaine de caractère pour transfert en drag'n drop
        static  QString         ConvertirVersChaine(DescCondition   Desc);      //|
        static  QString         ConvertirVersChaine(DescES          Desc);      //|
        static  QString         ConvertirVersChaine(DescBoucle      Desc);      //|
        static  QString         ConvertirVersChaine(DescOperation   Desc);      //|
        static  QString         ConvertirVersChaine(DescRoutine     Desc);      //|
        static  QString         ConvertirVersChaine(DescTempo       Desc);      //|

        static  void            ConvertirVersDesc(QString Chaine, DescCommentaire*  Desc);  //Conversion de chaine vers description pour appel de config
        static  void            ConvertirVersDesc(QString Chaine, DescCondition*    Desc);  //|
        static  void            ConvertirVersDesc(QString Chaine, DescES*           Desc);  //|
        static  void            ConvertirVersDesc(QString Chaine, DescBoucle*       Desc);  //|
        static  void            ConvertirVersDesc(QString Chaine, DescOperation*    Desc);  //|
        static  void            ConvertirVersDesc(QString Chaine, DescRoutine*      Desc);  //|
        static  void            ConvertirVersDesc(QString Chaine, DescTempo*        Desc);  //|

                void            setSuivant(unsigned int nIdSuivant, unsigned int nIdSortie = 0) ;   //Mutateur d'Id suivant
                void            RemplacerIdSuivant(unsigned int Ancien, unsigned int Nouveau);      //Remplace l'ancien id par le nouveau
                void            setPrec(int nIdPred) ;                                              //Mutateur d'Id précédent
        virtual void            setAttr(QString NomAttr, QStringRef Valeur) ;                       //Mutateur général

        virtual inline void     setDesc(DescCommentaire /*Desc*/) {}    //Déclaration de fonction virtuelle pour simplifier
        virtual inline void     setDesc(DescCondition   /*Desc*/) {}    //|
        virtual inline void     setDesc(DescES          /*Desc*/) {}    //|
        virtual inline void     setDesc(DescBoucle      /*Desc*/) {}    //|
        virtual inline void     setDesc(DescOperation   /*Desc*/) {}    //|
        virtual inline void     setDesc(DescRoutine     /*Desc*/) {}    //|
        virtual inline void     setDesc(DescTempo       /*Desc*/) {}    //|

        inline  unsigned int        getId()                 { return nId ; }        //Accesseur d'Id
        inline  QList<unsigned int> getIdSuivantParSortie() { return nIdSuivant ; } //Accesseur d'Id suivant
        inline  unsigned int        getIdPrec()             { return nIdPrec ; }    //Accesseur d'Id précédent
        inline  Routine *           getRoutine()            { return pRoutine ; }   //Ascesseur de routine

        virtual Compil_Inst*    Compiler() = 0 ;
        virtual TypeCommande    getType() = 0 ;                                     //Accesseur du type de commande
        virtual QString         toString() = 0 ;                                    //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
        virtual void            Executer() ;                                        //Exécute l'instruction
        virtual bool            Modifier() = 0 ;                                    //Lance la modification d'un item
                void            Supprimer();                                        //Se supprime de l'organigramme
                void            RecalculerId(unsigned int Seuil);                   //Recalcul son identifiant
        virtual void            Enregistrer(QXmlStreamWriter* Parser) = 0;          //Enregistrement
                IdentifiantInst getIdentifiantInst();

    protected:
                void        DebutBalise(QXmlStreamWriter* Parser);  //Ecrit le début de la balise instruction dans le fichier XML avec quelque propriétés communes
                void        FinBalise(QXmlStreamWriter* Parser);    //Ecrit la fin de la balise instruction dans le fichier XML
        QList<unsigned int> nIdSuivant ;                            //Liste des Ids des instructions suivantes en fonction des sorties
        unsigned int        nIdPrec ;                               //Id de l'item précédent directement (sortie 0)
        unsigned int        nId ;                                   //Id de l'instruction
        Routine *           pRoutine ;                              //Routine contenant cette instruction
};

#endif // INSTRUCTION_H
