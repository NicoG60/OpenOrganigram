#include "Compil_Inst.h"

Compil_Inst::Compil_Inst(Instruction *Inst_Compil) :
    InstructionCompilee(Inst_Compil)
{
}

Compil_Inst::~Compil_Inst()
{

}

void Compil_Inst::AjouterLienVers(IdentifiantInst IdInst, bool Absolu)
{
    LienCompil NouveauLien;
    NouveauLien.Position    = this->OpCodes.size();
    NouveauLien.IdInst      = IdInst;
    NouveauLien.Absolue     = Absolu;

    this->OpCodes.append('\0');
    this->OpCodes.append('\0');
}

void Compil_Inst::AjouterUnOpCode(char opcode)
{
    this->OpCodes.append(opcode);
}

void Compil_Inst::AjouterUnOpCode(QByteArray opcode)
{
    this->OpCodes.append(opcode);
}
