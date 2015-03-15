// Inst_Com.h   1.0     01/04/14    N.Jarnoux
#ifndef INST_COM_H
#define INST_COM_H

class Inst_Com ;

//===== Headers standards =====

//=====   Headers Peros   =====
#include "../Instruction.h"
#include "../ModInst/f_ModInst_Com.h"



class Inst_Com : public Instruction
{
    public:
                                Inst_Com(unsigned int nId, Routine *pRoutine);  //Constructeur

        inline  QString         getCommentaire()    { return sCommentaire ; }   //Accesseur du commentaire
                Compil_Inst*    Compiler();
        inline  TypeCommande    getType()   { return COM ; }                    //Accesseur de typer d'instruction
                QString         toString() ;                                    //Génére la chaine de caractÃ¨re Ã  afficher dans dans le dessin de l'organigramme
                bool            Modifier() ;                                    //Lance la fenetre de modification

                void            setAttr(QString NomAttr, QStringRef Valeur);    //Mutateur générale pour ouverture à partir d'un fichier
                void            setDesc(DescCommentaire Desc);                  //Mutateur de description
                DescCommentaire getDesc();                                      //Ascesseur de description
                void            Enregistrer(QXmlStreamWriter* Parser);          //Enregistre au format XML

    private:
                QString         sCommentaire ;  //Chaine de caractère contenant le commentaire
};

#endif // INST_COM_H
