#ifndef COMPIL_ROUT_H
#define COMPIL_ROUT_H

class Compil_Rout;

#include "Routine.h"

class Compil_Rout
{
public:
    Compil_Rout(Routine* Rout_Compil);
    ~Compil_Rout();

    int         getTaille();
    int         getAdresseRelative(Compil_Inst* Inst);
    int         get2DernieresAdresses();
    int inline  getIdRout() { return RoutineCompilee->getId(); }

private:
    Routine*            RoutineCompilee;
    QList<Compil_Inst*> InstCompilees;
};

#endif // COMPIL_ROUT_H
