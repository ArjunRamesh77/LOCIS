#include "virtualinstruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// virtualOper
virtualOper::virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg) :
    operType(operType_arg),
    value(value_arg),
    index(index_arg),
    signal(0)
{

}

virtualOper::virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg, __int8_t signal_arg) :
    operType(operType_arg),
    value(value_arg),
    index(index_arg),
    signal(signal_arg)
{

}

virtualOper::virtualOper(__int8_t operType_arg, double value_arg) :
    virtualOper(operType_arg, value_arg, VR_DUMMY_INDEX)
{

}

virtualOper::virtualOper(__int8_t operType_arg, unsigned int index_arg) :
    virtualOper(operType_arg, VR_DUMMY_DOUBLE, index_arg)
{

}

virtualOper::virtualOper(__int8_t operType_arg) :
    virtualOper(operType_arg, VR_DUMMY_DOUBLE, VR_DUMMY_INDEX)
{

}

virtualOper::virtualOper() :
    operType(VR_CONST),
    value(VR_DUMMY_DOUBLE),
    index(0),
    signal(0)
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// virtualInstructionStack
int virtualInstructionStack::getFunctionType() const
{
    return functionType;
}

void virtualInstructionStack::setFunctionType(int value)
{
    functionType = value;
}

virtualInstructionStack::virtualInstructionStack() :
    bInstructionOwner(false),
    allInst(NULL),
    instStack(),
    instInter()
{

}

virtualInstructionStack::~virtualInstructionStack()
{
    if(bInstructionOwner)
        delete allInst;
}

void virtualInstructionStack::createNewInstructionStack(std::vector<virtualOper> *instStackPointer)
{
    if(instStackPointer == NULL)
    {
        bInstructionOwner = true;
        allInst = new std::vector<virtualOper>;
    }
    else
    {
       allInst = instStackPointer;
    }
}

void virtualInstructionStack::addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg)
{
    allInst->push_back(virtualOper(operType_arg, value_arg, index_arg));
}

void virtualInstructionStack::addGeneralInstrWithSignal(__int8_t operType_arg, double value_arg, unsigned int index_arg, __int8_t signal)
{
    allInst->push_back(virtualOper(operType_arg, value_arg, index_arg, signal));
}

void virtualInstructionStack::addMathInstr(__int8_t op)
{
    allInst->push_back(virtualOper(op, VR_DUMMY_DOUBLE, VR_DUMMY_INDEX));
}

void virtualInstructionStack::addVariable1Index(unsigned int index)
{
    allInst->push_back(virtualOper(VR_VAR1_INDEX, VR_DUMMY_DOUBLE, index));
}

void virtualInstructionStack::addVariable2Index(unsigned int index)
{
    allInst->push_back(virtualOper(VR_VAR2_INDEX, VR_DUMMY_DOUBLE, index));
}

void virtualInstructionStack::addConstInstr(double value)
{
    allInst->push_back(virtualOper(VR_CONST, value, VR_DUMMY_INDEX));
}

void virtualInstructionStack::addInter(unsigned int index)
{
    allInst->push_back(virtualOper(VR_INTER_INDEX, VR_DUMMY_DOUBLE, index));
}

void virtualInstructionStack::addFunctionSISO(unsigned int index)
{
    allInst->push_back(virtualOper(VR_FUNCTION_SISO, VR_DUMMY_DOUBLE, index));
}

void virtualInstructionStack::clearVirtualMachineStack()
{
    while(!instStack.empty())
    {
        instStack.pop();
    }
}

void virtualInstructionStack::clearVisrtualMachineInter()
{
    instInter.clear();
}

void virtualInstructionStack::addFunctionDISO(unsigned int index)
{
    allInst->push_back(virtualOper(VR_FUNCTION_DISO, VR_DUMMY_DOUBLE, index));
}

void virtualInstructionStack::addCopyInstruction(virtualOper* cpy)
{
    allInst->push_back(virtualOper(cpy->operType, cpy->value, cpy->index, cpy->signal));
}

void virtualInstructionStack::makeLast()
{
    (allInst->end() - 1)->signal = VR_SIGNAL_LAST;
}

std::vector<virtualOper> *virtualInstructionStack::getAllInst()
{
    return allInst;
}
