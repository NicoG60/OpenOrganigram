//  Inst_DebFin.h   1.0     03/03/14        N.Jarnoux
#ifndef INST_DEBFIN_H
#define INST_DEBFIN_H

class Inst_DebFin ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"

class Inst_DebFin : public Instruction
{
    public:
                                Inst_DebFin(unsigned int nId, Routine *pRoutine) ;              //Constructeur
                                Inst_DebFin(unsigned int nId, Routine *pRoutine, bool bDebut) ; //Constructeur avec choix

                Compil_Inst*    Compiler();
        inline  bool            estDebut()  { return bDebut ; }             //Accesseur pour savoir si c'est un debut ou non
        inline  TypeCommande    getType()   { return DEBFIN ; }             //Accesseur de typer d'instruction
                void            Executer() ;                                //Exécute l'élément
                QString         toString() ;                                //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
        inline  bool            Modifier()  { return true ; }               //La modification n'est pas active sur cette instruction
                void            setAttr(QString NomAttr, QStringRef Valeur);//Mutateur général
                void            Enregistrer(QXmlStreamWriter* Parser);      //Enregistrement

    private:
        bool    bDebut;  //Indique si c'est un début ou une fin.
};

#endif // INST_DEBFIN_H
