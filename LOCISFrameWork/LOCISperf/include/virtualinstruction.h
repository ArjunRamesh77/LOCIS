#pragma once

#include <stdio.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// operation/operand types
enum
{
    VR_BIN_ADD,
    VR_BIN_SUB,
    VR_BIN_MUL,
    VR_BIN_DIV,
    VR_UNA_ADD,
    VR_UNA_SUB,
    VR_BIN_RAISE,
    VR_VAR_INDEX,
    VR_CONST
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A single operation or operand
struct virtualOper
{
   __int8_t operType;
   double value;
   unsigned int index;

   virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg);
};

class virtualSingleExpressionStack
{
    std::vector<virtualOper> allInst;

public:
    virtualSingleExpressionStack();

    void addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg);
    void addMathInstr(__int8_t op);
    void addVariableIndex(unsigned int index);
    void addConstInstr(double value);
    double evalStackBased();
};
