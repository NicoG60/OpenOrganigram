//  Inst_Tempo.h    1.0 28/03/14    N.Jarnoux
#ifndef INST_TEMPO_H
#define INST_TEMPO_H

class Inst_Tempo ;

#include "../Instruction.h"

class Inst_Tempo : public Instruction
{
    public:
                                Inst_Tempo(unsigned int nId, Routine* pRoutine);//Constructeur

        inline  unsigned short  getMillisecondes()  { return nMillisecondes ; } //ccesseur des milliseconde
        inline  TypeCommande    getType()   { return TEMPO ; }                  //Retourne le type d'instruction
                Compil_Inst*    Compiler();
                void            Executer() ;                                    //Fonction d'éxecution
                QString         toString() ;                                    //Génère la chaine de caractère à afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                                    //Lance la modification
                void            setAttr(QString NomAttr, QStringRef Valeur);    //Mutateur général
                void            setDesc(DescTempo Desc);                        //Mutateur de description
                DescTempo       getDesc();                                      //Ascesseur de description
                void            Enregistrer(QXmlStreamWriter* Parser);          //Enregistrement

        private:
                unsigned short  nMillisecondes ;    //Le temps de pause du programme
};

#endif // INST_TEMPO_H
