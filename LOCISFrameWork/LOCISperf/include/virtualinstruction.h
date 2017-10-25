#pragma once

#include <stdio.h>
#include <vector>
#include <stack>
#include <cmath>

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
    VR_VAR1_INDEX,
    VR_CONST,
    VR_FUNCTION_SISO,
    VR_FUNCTION_DISO
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SISO functions
enum
{
    VR_FUNC_COS,
    VR_FUNC_SIN,
    VR_FUNC_TAN,
    VR_FUNC_ACOS,
    VR_FUNC_ASIN,
    VR_FUNC_ATAN,
    VR_FUNC_COSH,
    VR_FUNC_SINH,
    VR_FUNC_TANH,
    VR_FUNC_ACOSH,
    VR_FUNC_ASINH,
    VR_FUNC_ATANH,
    VR_FUNC_EXP,
    VR_FUNC,LOG,
    VR_FUNC_LOG10,
    VR_FUNC_SQRT
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DISO functions
enum
{
    VR_FUNC_POW
};
typedef double (*TD_fpSISO)(double);
typedef double (*TD_fpDISO)(double, double);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A single operation or operand
struct virtualOper
{
   __int8_t operType;
   double value;
   unsigned int index;
   __int8_t last;

   virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// loads all the functions for the virtual machine
class virtualFunctionLoader
{

public:
    //trigonometric functions
    TD_fpSISO fpSISOarr[20];
    TD_fpDISO fpDISOarr[3];

public:
    virtualFunctionLoader();

    void loadAllSISO();
    void loadAllDISO();
};

extern virtualFunctionLoader g_virtualFunctionLoader;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a full set of instructions
class virtualInstructionStack
{
    std::vector<virtualOper> allInst;

public:
    virtualInstructionStack();
    ~virtualInstructionStack();

    void addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg);
    void addMathInstr(__int8_t op);
    void addVariable1Index(unsigned int index);
    void addConstInstr(double value);
    void addFunctionSISO(unsigned int index);
    void addFunctionDISO(unsigned int index);
    void makeLast();

    //evaluation routines
    std::vector<virtualOper>* getAllInst();
    void evalStackBased(std::vector<virtualOper>::const_iterator oper, std::stack<double> &stackPointer);
};
