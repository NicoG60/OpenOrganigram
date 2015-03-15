#ifndef COMPIL_INST_H
#define COMPIL_INST_H

class Compil_Inst;

#include "Instruction.h"

class Compil_Inst
{
public:
    Compil_Inst(Instruction* Inst_Compil);
    ~Compil_Inst();

    int             inline  getTaille() { return OpCodes.size(); }
    IdentifiantInst inline  getIdentifiantInst() { return InstructionCompilee->getIdentifiantInst(); }
    void                    AjouterLienVers(IdentifiantInst IdInst, bool Absolu = false);
    void                    AjouterUnOpCode(char opcode);
    void                    AjouterUnOpCode(QByteArray opcode);

private:
    Instruction*        InstructionCompilee;
    QByteArray          OpCodes;
    QList<LienCompil>   Liens;
};

#endif // COMPIL_INST_H
