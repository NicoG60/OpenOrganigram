#include <QStack>

#include "Compil_Rout.h"
#include "Compil_Inst.h"

Compil_Rout::Compil_Rout(Routine* Rout_Compil) :
    RoutineCompilee(Rout_Compil)
{
    Instruction* InstCourante = (Rout_Compil->getInstruction(0));
    QStack<Instruction*> DerniereCondition;

    //Tant qu'on est pas à la fin de l'algo
    do
    {
        this->InstCompilees.append(InstCourante->Compiler());

        if(InstCourante->getType() == COND)
            DerniereCondition.push(InstCourante);

        Instruction* InstSuiv(Rout_Compil->getInstruction(InstCourante->getIdSuivantParSortie().last()));

        if(InstSuiv->getIdPrec() == InstCourante->getId())
            InstCourante = InstSuiv;
        else
        {
            InstCompilees.last()->AjouterUnOpCode('\x12');
            InstCompilees.last()->AjouterLienVers(InstSuiv->getIdentifiantInst(), true);
            InstCourante = Rout_Compil->getInstruction(DerniereCondition.pop()->getIdSuivantParSortie().at(0));
        }



    } while(InstCourante->getId() != 1);
}

Compil_Rout::~Compil_Rout()
{
    for(register int i = 0; i < this->InstCompilees.length(); i++)
    {
        delete this->InstCompilees[i];
    }
}

int Compil_Rout::getTaille()
{
    int Retour(0);

    for(register int i = 0; i < this->InstCompilees.length(); i++)
    {
        Retour += this->InstCompilees[i]->getTaille();
    }

    return Retour;
}

int Compil_Rout::getAdresseRelative(Compil_Inst* Inst)
{
    int             Retour(0);
    int             i(0);
    Compil_Inst*    InstCourante(0);

    while(InstCourante != Inst)
    {
        InstCourante = this->InstCompilees[i];

        if(InstCourante != Inst)
        {
            Retour += InstCourante->getTaille();
        }
    }

    return Retour;
}

int Compil_Rout::get2DernieresAdresses()
{
    return this->getTaille() - 2;
}
