#ifndef COMPIL_PROG_H
#define COMPIL_PROG_H

class Compil_Prog;

#include "Compil_Rout.h"
#include "Compil_Inst.h"
#include "Routine.h"
#include "Instruction.h"


class Compil_Prog
{
public:
    Compil_Prog();
    ~Compil_Prog();

    void AjouterRoutine(Routine* Rout);
    void EtablirLiens();

private:

    QList<Compil_Rout*> ListeRoutineCompil;
};

#endif // COMPIL_PROG_H
