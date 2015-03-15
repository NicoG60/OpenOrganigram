//  Inst_Ope.h  1.0     03/03/14    N. Jarnoux
#ifndef INST_OPE_H
#define INST_OPE_H

class Inst_Ope ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"

class Inst_Ope : public Instruction
{
    public:
                                Inst_Ope(unsigned int nId, Routine *pRoutine) ;                     //Constructeur

        inline  TypeCommande    getType()   { return OPE ; }            //Retourne le type d'instruction
                Compil_Inst*    Compiler();
                void            Executer() ;                            //Fonction d'éxecution
                QString         toString() ;                            //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                            //Lance la modification
                
        inline  unsigned int    getIndiceVariable() { return nIndiceVariable ; }    //Acesseur de l'indice de la variable
        inline  TypeOperation   getOperation()      { return Operation ; }          //Accesseur du type d'operation
        inline  unsigned short  getValeur()         { return nValeur ; }            //Acesseur de la valeur à prendre pour l'opération
                void            setIndiceVariable(unsigned int nIndiceVariable) ;   //Mutateur de l'indice variable
                void            setOperation(TypeOperation Operation) ;             //Mutateur du type d'operation
                void            setValeur(unsigned short nValeur) ;                 //Mutateur de la valeur
                void            setAttr(QString NomAttr, QStringRef Valeur);        //Mutateur général
                void            setDesc(DescOperation Desc);                        //Mutateur de description
                DescOperation   getDesc();                                          //Ascesseur de description
                void            Enregistrer(QXmlStreamWriter* Parser);              //Enregistrement

    private:
        unsigned int    nIndiceVariable ;       //La variable
        TypeOperation   Operation ;             //Type d'opération à effectuer
        unsigned short  nValeur ;               //La valeur avec laquelle faire l'opération

        void            Affecter(ModuleManager *    Manager) ;      //Affecte nValeur dans la variable donnée
        void            Incrementer(ModuleManager *    Manager) ;   //Incremente la variable
        void            Decrementer(ModuleManager *    Manager) ;   //Decremente la variable
        void            Ajouter(ModuleManager *    Manager) ;       //Ajoute la valeur à celle de la variable
        void            Soustraire(ModuleManager *    Manager) ;    //Soustrait la valeur à celle de la variable

};

#endif // INST_OPE_H
