#include "virtualinstruction.h"

virtualOper::virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg) :
    operType(operType_arg),
    value(value_arg),
    index(index_arg)
{

}

virtualSingleExpressionStack::virtualSingleExpressionStack()
{

}

void virtualSingleExpressionStack::addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg)
{
    allInst.push_back(virtualOper(operType_arg, value_arg, index_arg));
}

void virtualSingleExpressionStack::addMathInstr(__int8_t op)
{
    allInst.push_back(virtualOper(op, 0, 0));
}

void virtualSingleExpressionStack::addVariableIndex(unsigned int index)
{
    allInst.push_back(virtualOper(VR_VAR_INDEX, 0, index));
}

void virtualSingleExpressionStack::addConstInstr(double value)
{
    allInst.push_back(virtualOper(VR_CONST, value, 0));
}

double virtualSingleExpressionStack::evalStackBased()
{
    std::vector<virtualOper>::const_iterator allInst_end = allInst.end();
    for(std::vector<virtualOper>::const_iterator vo = allInst.begin(); vo != allInst_end; ++vo)
    {
        //swictch by type
        switch (vo->operType) {

        case value:

            break;
        default:
            break;
        }
    }
}
