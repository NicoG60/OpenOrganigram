// Inst_Boucle.h  1.0     01/04/14    N. Jarnoux
#ifndef INST_BOUCLE_H
#define INST_BOUCLE_H

class Inst_Loop ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"
#include "Inst_Cond.h"

class Inst_Boucle : public Instruction
{
    public:
                Inst_Boucle(unsigned int nId, Routine *pRoutine) ;                              //Constructeur

        inline  TypeCommande    getType()   { return BOUCLE ; }                                 //Retourne le type d'instruction
                Compil_Inst*    Compiler() ;
                void            Executer() ;                                                    //Fonction d'éxecution
                QString         toString() ;                                                    //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                                                    //Lance la modification

        inline  DescCondition   getDescriptionCondition()   { return DescriptionCondition ; }   //Ascesseur de condition
        inline  TypeBoucle      getTypeDeBoucle()           { return TypeDeBoucle ; }           //Ascesseur type de boucle
                void            setDesc(DescBoucle Description) ;                               //Mutateur de description
                DescBoucle      getDesc();                                                      //Ascesseur de description
        inline  unsigned int    getIdPaire()                { return pInstPaire->getId() ; }    //Ascesseur d'identifiant d'instruction paire
        inline  Inst_Boucle *   getInstPaire()              { return pInstPaire ; }             //Ascesseur d'instruction paire
                void            setInstPaire(Inst_Boucle * pInstPaire) ;                        //Mutateur d'instruction paire
                bool            EstLePremier();                                                 //Vérifie la position de l'élément par rapport à sa paire
                void            setAttr(QString NomAttr, QStringRef Valeur);                    //Mutateur générale pour ouverture à partir d'un fichier
                void            setActivation(bool Activ);                                      //Mutateur d'activation
                bool            getEstActive();                                                 //Ascesseur d'activation
                void            Enregistrer(QXmlStreamWriter* Parser);                          //Enregistre l'objet en XML

    private:
                Inst_Boucle *   pInstPaire ;            //Pointeur vers l'instruction paire
                TypeBoucle      TypeDeBoucle ;          //Type de boucle
                DescCondition   DescriptionCondition ;  //Condition à exécuter
                bool            EstActive;              //Activation de la boucle

                bool            LancerModification() ;  //Lance la modification
};

#endif // INST_BOUCLE_H
