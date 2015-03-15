//  Routine.h   1.0     21/02/14    N.Jarnoux+
#ifndef ROUTINE_H
#define ROUTINE_H

class Routine ;

//===== Headers standards =====
#include <QObject>
#include <QString>
#include <QList>
#include <QStandardItem>

//=====   Headers Peros   =====
#include "Instruction.h"
#include "TypeCommun.h"
#include "ModuleManager.h"

class Routine : public QObject
{
    Q_OBJECT
    public:
        explicit                    Routine(unsigned int nId, QObject *   pParent = 0) ;              //Constructeur  par def
                                    Routine(unsigned int nId, QString Nom,
                                            ModuleManager * pManagerParent,
                                            bool bSansDepart = false,
                                            QObject *  pParent = 0) ;               //Constructeur  avec nom
                                    ~Routine();                                     //Destructeur

        inline  unsigned int        getId()                 { return nId ; }            //Accesseur d'Id
        inline  QString             getNom() const          { return sNom ; }           //Accesseur de nom
        inline  QList<Instruction*> getListe() const        { return ListeInst ; }      //Accesseur de liste
        inline  Instruction*        getInstruction(int Id)  { return ListeInst[Id] ; }  //ACcesseur d'un inst
        inline  ModuleManager *     getManagerParent()      { return pManagerParent ; } //Accesseur du manager contenant cette routine
        inline  int                 getNbreInst()           { return ListeInst.length() ; }//accesseur du nombre d'instruction dontenue

                void                AjouterInstruction(TypeCommande Type, unsigned int nIdPrecedent = 0, bool Ecartement = false, bool RetourCondition = false, bool Deplacement = false) ; //Ajoute une instruction
                void                AjouterInstruction(Instruction * Inst); //Ajoute une instruction à partir du pointeur
                void                Supprimer(Instruction * Inst);  //Met une instruction dans une corbeille qui sera vidée ulterieurement
                QList<Instruction*> getInstructionPrecedentDe(unsigned int nId);    //Retourne la liste des instruction pointant sur celle dont l'(id est passé en paramètre
                void                RecalculerId(unsigned int Seuil);               //Recalcul l'Id des instructions
                void                Enregistrer(QXmlStreamWriter* Parser);          //Enregistre son contenu en XML

    private:
        void Constructeur(bool bSansDepart = false);    //Action commune au deux constructeur
        unsigned int        nId ;                       //Identifiant de la routine
        QString             sNom ;                      //Nom de la routine
        QList<Instruction*> ListeInst ;                 //Liste des instructions
        QList<Instruction*> Corbeille ;                 //Corbeille d'instruction
        ModuleManager *     pManagerParent ;            //Manager de module contenant cette routine
};

#endif // ROUTINE_H
