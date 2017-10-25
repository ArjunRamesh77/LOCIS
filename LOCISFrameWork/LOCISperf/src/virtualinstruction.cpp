#include "virtualinstruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// virtualOper
virtualOper::virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg) :
    operType(operType_arg),
    value(value_arg),
    index(index_arg),
    last(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// virtualInstructionStack
virtualInstructionStack::virtualInstructionStack() :
    allInst()
{

}

virtualInstructionStack::~virtualInstructionStack()
{

}

void virtualInstructionStack::addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg)
{
    allInst.push_back(virtualOper(operType_arg, value_arg, index_arg));
}

void virtualInstructionStack::addMathInstr(__int8_t op)
{
    allInst.push_back(virtualOper(op, 0, 0));
}

void virtualInstructionStack::addVariable1Index(unsigned int index)
{
    allInst.push_back(virtualOper(VR_VAR1_INDEX, 0, index));
}

void virtualInstructionStack::addConstInstr(double value)
{
    allInst.push_back(virtualOper(VR_CONST, value, 0));
}

void virtualInstructionStack::addFunctionSISO(unsigned int index)
{
    allInst.push_back(virtualOper(VR_FUNCTION_SISO, 0, index));
}

void virtualInstructionStack::addFunctionDISO(unsigned int index)
{
    allInst.push_back(virtualOper(VR_FUNCTION_DISO, 0, index));
}

void virtualInstructionStack::makeLast()
{
    (allInst.end() - 1)->last = 1;
}

std::vector<virtualOper> *virtualInstructionStack::getAllInst()
{
    return &allInst;
}
