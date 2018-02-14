#pragma once

#include "virtualinstruction.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implements a residual based on the virtualInstruction
// the residual maybe evaluated by stack
// or by JIT
class genericResidual : public virtualInstructionStack
{

public:
    genericResidual();
    virtual ~genericResidual();

    //Steady State residual
    int evalResidual1StackBased(double* xOrig, double* x, double* r);
    int evalResidual2StackBased(double* yyOrig, double* yy, double* ypOrig, double* yp, double t, double* r);
};
