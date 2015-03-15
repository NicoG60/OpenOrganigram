//  Inst_Rout.h 1.0 20/03/14    N.Jarnoux
#ifndef INST_ROUT_H
#define INST_ROUT_H

class Inst_Rout ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"

class Inst_Rout : public Instruction
{
    public:
                            Inst_Rout(unsigned int nId, Routine *pRoutine) ;             //Constructeur

    inline  TypeCommande    getType()       { return ROUT ; }           //Retourne le type d'instruction
    inline  int             getIdRoutine()  { return nIdRoutine ; }
            Compil_Inst*    Compiler() ;
            void            Executer() ;                                //Fonction d'éxecution
            QString         toString() ;                                //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
            bool            Modifier() ;                                //Lance la modification
            void            setAttr(QString NomAttr, QStringRef Valeur);//Mutateur général
            void            setDesc(DescRoutine Desc);                  //Mutateur de description
            DescRoutine     getDesc();                                  //Ascesseur de description
            void            Enregistrer(QXmlStreamWriter* Parser);      //Enregistrement

    private:
            int     nIdRoutine ;    //Identifiant de la routine
            QString sNomRoutine ;   //Nm de la routine
};

#endif // INST_ROUT_H
