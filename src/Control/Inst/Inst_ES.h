//  Inst_ES.h   1.0     20/03/14    N. Jarnoux
#ifndef INST_ES_H
#define INST_ES_H

class Inst_ES ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"
#include "../ModInst/f_ModInst_ES.h"

class Inst_ES : public Instruction
{
    public:
                                Inst_ES(unsigned int nId, Routine *pRoutine) ;             //Constructeur

                Compil_Inst*    Compiler();
        inline  TypeCommande    getType()   { return ES ; }                     //Retourne le type d'instruction
                void            Executer() ;                                    //Fonction d'éxecution
                QString         toString() ;                                    //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                                    //Lance la modification
                void            setAttr(QString NomAttr, QStringRef Valeur);    //Mutateur général
                void            setDesc(DescES Desc);                           //Mutateur de description
                DescES          getDesc();                                      //AScesseur de description
                void            Enregistrer(QXmlStreamWriter* Parser);          //Enregistrement

    private:
                ActionArduino   CommandeDemandee ;  //La commande à exécuter
                bool            bAttenteReponse ;   //Attente de réponse de la maquette
    
};

#endif // INST_ES_H
