#include "Compil_Prog.h"

Compil_Prog::Compil_Prog()
{

}

Compil_Prog::~Compil_Prog()
{

}

void Compil_Prog::AjouterRoutine(Routine* Rout)
{
    this->ListeRoutineCompil.append(new Compil_Rout(Rout));
}

void Compil_Prog::EtablirLiens()
{

}
