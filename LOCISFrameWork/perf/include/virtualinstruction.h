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
    VR_ASSIGN,
    VR_VAR1_INDEX,
    VR_VAR2_INDEX,
    VR_CONST_VAR1,
    VR_CONST_VAR2,
    VR_INTER_INDEX,
    VR_DERIV_INDEX,
    VR_VAR1_VECTOR,
    VR_VAR2_VECTOR,
    VR_VAR_VECTOR,
    VR_CONST,
    VR_TIME,
    VR_LOAD_VAR1_INDEX,
    VR_LOAD_VAR2_INDEX,
    VR_LOAD_CONST_VAR1,
    VR_LOAD_CONST_VAR2,
    VR_LOAD_CONST,
    VR_CONST_VECTOR,
    VR_FUNCTION_SISO,
    VR_FUNCTION_DISO,
    VR_NOOP
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
    VR_FUNC_LOG,
    VR_FUNC_LOG10,
    VR_FUNC_SQRT
};

#define CONST_LOG10 2.30258509299

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DISO functions
enum
{
    VR_FUNC_POW
};
typedef double (*TD_fpSISO)(double);
typedef double (*TD_fpDISO)(double, double);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VR_DUMMY_DOUBLE 0.0
#define VR_DUMMY_INDEX 0
#define VR_SIGNAL_LAST 1
#define VR_SIGNAL_GRADIENT_COMPLETE 4
#define FUNCTION_TYPE_ALG 0
#define FUNCTION_TYPE_DAE 1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A single operation or operand
struct virtualOper
{
   __int8_t operType;
   double value;
   unsigned int index;
   __int8_t signal;

   virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg);
   virtualOper(__int8_t operType_arg, double value_arg, unsigned int index_arg, __int8_t signal);
   virtualOper(__int8_t operType_arg, double value_arg);
   virtualOper(__int8_t operType_arg, unsigned int index_arg);
   virtualOper(__int8_t operType_arg);
   virtualOper();
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
// a full set of instructions and its execution environment
class virtualInstructionStack
{
protected:
    int functionType;
    bool bInstructionOwner;
    std::vector<virtualOper>* allInst;
    std::stack<double> instStack;
    std::vector<double> instInter;
    double val1;
    double val2;

public:
    virtualInstructionStack();
    ~virtualInstructionStack();

    //control
    void createNewInstructionStack(std::vector<virtualOper>* instStackPointer);

    //scalar operations
    int getFunctionType() const;
    void setFunctionType(int value);
    void addGeneralInstr(__int8_t operType_arg, double value_arg, unsigned int index_arg);
    void addGeneralInstrWithSignal(__int8_t operType_arg, double value_arg, unsigned int index_arg, __int8_t signal);
    void addMathInstr(__int8_t op);
    void addVariable1Index(unsigned int index);
    void addVariable2Index(unsigned int index);
    void addConstInstr(double value);
    void addInter(unsigned int index);
    void addFunctionSISO(unsigned int index);
    void addFunctionDISO(unsigned int index);
    void addCopyInstruction(virtualOper *cpy);
    void clearVirtualMachineStack();
    void clearVisrtualMachineInter();
    void setInstInter(std::vector<double>* instInter);
    void makeLast();

    //evaluation routines
    std::vector<virtualOper>* getAllInst();
    void evalStackBased(std::vector<virtualOper>::const_iterator oper);
};
