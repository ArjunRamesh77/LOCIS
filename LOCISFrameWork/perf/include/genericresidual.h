#pragma once

#include "virtualinstruction.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// implements a residual based on the virtualInstruction
// the residual maybe evaluated by stack
// or by JIT
class genericResidual : public virtualInstructionStack
{
    unsigned int numEquations;
public:
    genericResidual();
    virtual ~genericResidual();

    void setNumEquations(unsigned int numEquations_arg);
    void countNumEquations();
    unsigned int getNumEquations();

    //Steady State residual
    int evalResidual1StackBased(double* xOrig, double* x, double* r);
    int evalResidual2StackBased(double* yyOrig, double* yy, double* ypOrig, double* yp, double t, double* r);
};
